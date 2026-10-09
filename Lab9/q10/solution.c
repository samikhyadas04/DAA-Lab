/*
 * Q10  Shortest Common Superstring: the GREEDY algorithm vs the exact optimum (Held-Karp)
 *
 * Greedy : remove strings that are substrings of others (and duplicates); then repeatedly merge
 *          the ordered pair (i,j) with the maximum overlap  ov(i,j) = longest suffix of s_i that is
 *          a prefix of s_j  until one string remains.  (Ties: first pair found - tie rules matter!)
 * Exact  : after substring removal, SCS = sum(len) - (max total overlap over Hamiltonian paths);
 *          bitmask DP, only for n <= 16.
 * Modes  : ./q10 < sample_input.txt       solve one instance (input: n, then n strings)
 *          ./q10 --family K               the classic bad family {c(ab)^K, (ba)^K, (ab)^K c}, ratio -> 2
 *          ./q10 --selftest               random instances: validates greedy output is a superstring,
 *                                         greedy >= optimal, and reports the worst ratio observed.
 * Build  : gcc -O2 -o q10 solution.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXS 20
#define MAXL 4096

static int ov(const char *a, const char *b) {
    int la = strlen(a), lb = strlen(b), m = la < lb ? la : lb;
    for (int k = m; k > 0; k--) if (!memcmp(a + la - k, b, k)) return k;
    return 0;
}

/* removes duplicates and strings contained in others; returns new count */
static int reduce(char s[][MAXL], int n) {
    int keep[MAXS]; for (int i = 0; i < n; i++) keep[i] = 1;
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) {
        if (i == j || !keep[i] || !keep[j]) continue;
        if (strstr(s[j], s[i]) && (strlen(s[i]) < strlen(s[j]) || i > j)) { keep[i] = 0; break; }
    }
    int m = 0; for (int i = 0; i < n; i++) if (keep[i]) { if (m != i) strcpy(s[m], s[i]); m++; }
    return m;
}

static void greedy(char s[][MAXL], int n, char *out) {            /* s is destroyed */
    while (n > 1) {
        int bi = 0, bj = 1, bo = -1;
        for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) if (i != j) { int o = ov(s[i], s[j]); if (o > bo) { bo = o; bi = i; bj = j; } }
        strcat(s[bi], s[bj] + bo);
        if (bj != n - 1) strcpy(s[bj], s[n-1]);
        if (bi == n - 1) bi = bj;                                 /* moved */
        n--;
    }
    strcpy(out, s[0]);
}

static int optimum(char s[][MAXL], int n) {                       /* n <= 16, strings already reduced */
    static int dp[1 << 16][16]; int ovm[MAXS][MAXS], tot = 0;
    for (int i = 0; i < n; i++) { tot += strlen(s[i]); for (int j = 0; j < n; j++) ovm[i][j] = (i == j) ? 0 : ov(s[i], s[j]); }
    for (int m = 0; m < (1 << n); m++) for (int i = 0; i < n; i++) dp[m][i] = -1;
    for (int i = 0; i < n; i++) dp[1 << i][i] = 0;
    for (int m = 1; m < (1 << n); m++) for (int i = 0; i < n; i++) if (dp[m][i] >= 0)
        for (int j = 0; j < n; j++) if (!(m >> j & 1)) { int v = dp[m][i] + ovm[i][j]; if (v > dp[m | 1 << j][j]) dp[m | 1 << j][j] = v; }
    int best = 0; for (int i = 0; i < n; i++) if (dp[(1 << n) - 1][i] > best) best = dp[(1 << n) - 1][i];
    return tot - best;
}

static int contains_all(const char *sup, char orig[][MAXL], int n) { for (int i = 0; i < n; i++) if (!strstr(sup, orig[i])) return 0; return 1; }

static void run(char in[][MAXL], int n, int verbose, int *glen, int *olen) {
    static char orig[MAXS][MAXL], w[MAXS][MAXL], w2[MAXS][MAXL], out[MAXL * MAXS];
    for (int i = 0; i < n; i++) { strcpy(orig[i], in[i]); strcpy(w[i], in[i]); }
    int m = reduce(w, n); for (int i = 0; i < m; i++) strcpy(w2[i], w[i]);
    greedy(w, m, out); *glen = strlen(out);
    *olen = (m <= 16) ? optimum(w2, m) : -1;
    if (!contains_all(out, orig, n)) { printf("ERROR: greedy output is not a superstring!\n"); exit(2); }
    if (verbose) {
        printf("Greedy superstring (%d chars): %s\n", *glen, *glen < 200 ? out : "(long)");
        if (*olen >= 0) printf("Optimal length (Held-Karp)   : %d   ratio greedy/opt = %.4f\n", *olen, (double)*glen / *olen);
    }
}

int main(int argc, char **argv) {
    static char s[MAXS][MAXL];
    if (argc > 1 && !strcmp(argv[1], "--family")) {
        int K = argc > 2 ? atoi(argv[2]) : 20; if (K < 1 || K * 2 + 2 >= MAXL) return 1;
        char ab[MAXL] = "", ba[MAXL] = "";
        for (int i = 0; i < K; i++) { strcat(ab, "ab"); strcat(ba, "ba"); }
        snprintf(s[0], MAXL, "c%s", ab); snprintf(s[1], MAXL, "%s", ba); snprintf(s[2], MAXL, "%sc", ab);
        int g, o; run(s, 3, 0, &g, &o);
        printf("K=%d: greedy=%d optimal=%d ratio=%.4f  (-> 2 as K grows)\n", K, g, o, (double)g / o);
        return 0;
    }
    if (argc > 1 && !strcmp(argv[1], "--selftest")) {
        srand(6); double worst = 0; int T = 3000;
        for (int tc = 0; tc < T; tc++) {
            int n = 2 + rand() % 5, alpha = 2 + rand() % 2;
            for (int i = 0; i < n; i++) { int L = 1 + rand() % 7; for (int k = 0; k < L; k++) s[i][k] = 'a' + rand() % alpha; s[i][L] = 0; }
            int g, o; run(s, n, 0, &g, &o);
            if (g < o) { printf("BUG: greedy shorter than optimum?! tc=%d\n", tc); return 1; }
            double r = (double)g / o; if (r > worst) worst = r;
        }
        printf("selftest: %d random instances; greedy output always a valid superstring and >= optimum;\n"
               "          worst ratio seen on random instances = %.4f (random inputs are far from adversarial)\n", T, worst);
        return 0;
    }
    int n; if (scanf("%d", &n) != 1 || n < 1 || n > MAXS) return 1;
    for (int i = 0; i < n; i++) if (scanf("%4095s", s[i]) != 1) return 1;
    int g, o; run(s, n, 1, &g, &o);
    return 0;
}
