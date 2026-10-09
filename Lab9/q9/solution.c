/*
 * Q9  Hu-Tucker algorithm: optimal alphabetic binary tree (leaves keep the given left-to-right order)
 * minimising  sum w_i * depth(i).
 *
 * Phase 1 (combination): nodes in a list; original leaves are SQUARES, merged nodes are CIRCLES.
 *   Two nodes are a compatible pair if no square lies strictly between them.
 *   Repeatedly take the compatible pair with minimum weight sum (ties: leftmost i, then leftmost j),
 *   replace the pair by one circle of weight w_i + w_j placed at the position of the LEFT node
 *   (right node deleted).  This "free" (non-alphabetic) tree only gives the leaf LEVELS.
 * Phase 2 (levels): depth of every leaf in the phase-1 tree.
 * Phase 3 (recombination): with leaves in order and these depths, rebuild an alphabetic tree with a stack:
 *   push leaf; while the top two stack entries have equal level, merge them into one node of level-1.
 * Check : Knuth-style O(n^3) interval DP gives the true optimum cost; they must be equal.
 *
 * Input : n, then n positive weights.   Build: gcc -O2 -o q9 solution.c   Run: ./q9 < sample_input.txt | ./q9 --selftest
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 512
static int Lc[2*MAXN], Rc[2*MAXN];                 /* tree nodes of phase 1 */
static int depth1[2*MAXN];

/* returns cost; fills level[0..n-1] */
static long long hu_tucker_levels(const long long *w, int n, int *level) {
    long long wt[MAXN]; int sq[MAXN], id[MAXN], m = n, next = n;
    for (int i = 0; i < n; i++) { wt[i] = w[i]; sq[i] = 1; id[i] = i; Lc[i] = Rc[i] = -1; }
    while (m > 1) {
        int bi = -1, bj = -1; long long bs = 0;
        for (int i = 0; i < m; i++)
            for (int j = i + 1; j < m; j++) {
                long long s = wt[i] + wt[j];
                if (bi < 0 || s < bs) { bi = i; bj = j; bs = s; }
                if (sq[j]) break;                         /* a square blocks every farther partner */
            }
        Lc[next] = id[bi]; Rc[next] = id[bj];
        wt[bi] = bs; sq[bi] = 0; id[bi] = next++;         /* circle takes the LEFT position */
        for (int k = bj; k < m - 1; k++) { wt[k] = wt[k+1]; sq[k] = sq[k+1]; id[k] = id[k+1]; }
        m--;
    }
    int root = next - 1; depth1[root] = 0;
    for (int k = root; k >= n; k--) depth1[Lc[k]] = depth1[Rc[k]] = depth1[k] + 1;
    long long cost = 0;
    for (int i = 0; i < n; i++) { level[i] = depth1[i]; cost += w[i] * level[i]; }
    return cost;
}

/* phase 3: build alphabetic tree from levels; returns 1 on success and prints it in bracket form */
static int build_tree(const int *level, int n, char *out, size_t cap) {
    char *st[MAXN]; int sl[MAXN], top = 0;
    for (int i = 0; i < n; i++) {
        char *s = malloc(16); snprintf(s, 16, "%d", i + 1); st[top] = s; sl[top++] = level[i];
        while (top >= 2 && sl[top-1] == sl[top-2]) {
            size_t len = strlen(st[top-2]) + strlen(st[top-1]) + 4; char *t = malloc(len);
            snprintf(t, len, "(%s %s)", st[top-2], st[top-1]); free(st[top-1]); free(st[top-2]);
            top -= 2; st[top] = t; sl[top] = sl[top+1] - 1; top++;
        }
    }
    int ok = (top == 1 && sl[0] == 0);
    if (ok) snprintf(out, cap, "%s", st[0]);
    for (int i = 0; i < top; i++) free(st[i]);
    return ok;
}

static long long dp_optimum(const long long *w, int n) {          /* O(n^3) reference */
    static long long c[MAXN][MAXN], pre[MAXN + 1];
    pre[0] = 0; for (int i = 0; i < n; i++) pre[i+1] = pre[i] + w[i];
    for (int i = 0; i < n; i++) c[i][i] = 0;
    for (int len = 2; len <= n; len++)
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1; long long best = -1;
            for (int k = i; k < j; k++) { long long v = c[i][k] + c[k+1][j]; if (best < 0 || v < best) best = v; }
            c[i][j] = best + pre[j+1] - pre[i];
        }
    return c[0][n-1];
}

int main(int argc, char **argv) {
    if (argc > 1 && !strcmp(argv[1], "--selftest")) {
        srand(8); int bad = 0, T = 30000;
        for (int tc = 0; tc < T; tc++) {
            int n = 1 + rand() % 12; long long w[MAXN]; int lv[MAXN]; int mx = (rand() % 2) ? 4 : 30;   /* small range => many ties */
            for (int i = 0; i < n; i++) w[i] = 1 + rand() % mx;
            long long c = (n == 1) ? 0 : hu_tucker_levels(w, n, lv);
            char buf[4096]; int ok = (n == 1) ? 1 : build_tree(lv, n, buf, sizeof buf);
            if (n > 1 && (c != dp_optimum(w, n) || !ok)) { bad++; if (bad < 5) printf("FAIL tc=%d n=%d cost=%lld dp=%lld ok=%d\n", tc, n, c, dp_optimum(w, n), ok); }
        }
        printf("selftest: %d cases, %d failures\n", T, bad); return bad != 0;
    }
    int n; if (scanf("%d", &n) != 1 || n < 1 || n > MAXN) return 1;
    long long w[MAXN]; int lv[MAXN];
    for (int i = 0; i < n; i++) if (scanf("%lld", &w[i]) != 1) return 1;
    if (n == 1) { printf("Single leaf, cost 0\n"); return 0; }
    long long c = hu_tucker_levels(w, n, lv);
    printf("Leaf levels (depths):"); for (int i = 0; i < n; i++) printf(" %d", lv[i]);
    char buf[1 << 16];
    if (build_tree(lv, n, buf, sizeof buf)) printf("\nAlphabetic tree: %s\n", buf);
    printf("Optimal cost sum w_i*depth_i (Hu-Tucker) = %lld\nOptimal cost (interval-DP check)         = %lld\n", c, dp_optimum(w, n));
    return 0;
}
