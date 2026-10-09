/*
 * Q2  Huffman Coding + canonical codebook
 * Input : n, then n lines "symbol frequency"   (symbol = any token without spaces)
 * Steps : (1) min-heap Huffman tree -> code LENGTHS only
 *         (2) canonical assignment: sort by (length, symbol); first code = 0...0,
 *             next code = (prev + 1) << (len_next - len_prev)
 * Build : gcc -O2 -o q2 solution.c      Run: ./q2 < sample_input.txt | ./q2 --selftest
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { char sym[32]; long long f; int len; char *code; } Sym;
static long long *W; static int *L, *R;          /* tree nodes: 0..n-1 leaves, n.. internal */
static int *heap, hs;

static int less(int a, int b) { return W[a] < W[b] || (W[a] == W[b] && a < b); }  /* deterministic ties */
static void hpush(int x) { int i = hs++; heap[i] = x; while (i && less(heap[i], heap[(i-1)/2])) { int p=(i-1)/2,t=heap[i];heap[i]=heap[p];heap[p]=t;i=p; } }
static int hpop(void) {
    int top = heap[0]; heap[0] = heap[--hs]; int i = 0;
    for (;;) { int l=2*i+1, r=l+1, m=i;
        if (l<hs && less(heap[l],heap[m])) m=l; if (r<hs && less(heap[r],heap[m])) m=r;
        if (m==i) break; int t=heap[i];heap[i]=heap[m];heap[m]=t;i=m; }
    return top;
}

/* returns code lengths in len[] and the cost sum f*len */
static long long huffman_lengths(const long long *f, int n, int *len) {
    if (n == 1) { len[0] = 1; return f[0]; }
    W = malloc(2*n*sizeof(long long)); L = malloc(2*n*sizeof(int)); R = malloc(2*n*sizeof(int));
    heap = malloc(n*sizeof(int)); hs = 0; int *depth = calloc(2*n, sizeof(int));
    for (int i = 0; i < n; i++) { W[i] = f[i]; L[i] = R[i] = -1; hpush(i); }
    int next = n;
    while (hs > 1) { int a = hpop(), b = hpop(); W[next] = W[a]+W[b]; L[next]=a; R[next]=b; hpush(next++); }
    int root = next - 1; depth[root] = 0;
    for (int k = root; k >= n; k--) { depth[L[k]] = depth[R[k]] = depth[k] + 1; }
    long long cost = 0;
    for (int i = 0; i < n; i++) { len[i] = depth[i]; cost += f[i] * len[i]; }
    free(W); free(L); free(R); free(heap); free(depth); return cost;
}

static int cmp_len_sym(const void *a, const void *b) {
    const Sym *x = a, *y = b;
    if (x->len != y->len) return x->len - y->len;
    return strcmp(x->sym, y->sym);
}

static void canonical(Sym *s, int n) {
    qsort(s, n, sizeof(Sym), cmp_len_sym);
    int maxl = s[n-1].len; char *cur = calloc(maxl + 2, 1);
    int curlen = s[0].len; memset(cur, '0', curlen); cur[curlen] = 0;
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            int k = curlen - 1;                               /* binary +1 */
            while (k >= 0 && cur[k] == '1') cur[k--] = '0';
            if (k >= 0) cur[k] = '1';
            while (curlen < s[i].len) cur[curlen++] = '0';
            cur[curlen] = 0;
        }
        s[i].code = strdup(cur);
    }
    free(cur);
}

static int cmp_ll(const void *a, const void *b) { long long x=*(const long long*)a,y=*(const long long*)b; return (x>y)-(x<y); }
static long long two_queue_cost(long long *f, int n) {            /* independent O(n) check after sort */
    long long *a = malloc(n*sizeof(long long)), *q = malloc(n*sizeof(long long)); memcpy(a, f, n*sizeof(long long));
    qsort(a, n, sizeof(long long), cmp_ll);
    int ai = 0, qh = 0, qt = 0; long long cost = 0;
    for (int step = 0; step < n - 1; step++) {
        long long s = 0;
        for (int k = 0; k < 2; k++) {
            if (ai < n && (qh == qt || a[ai] <= q[qh])) s += a[ai++]; else s += q[qh++];
        }
        q[qt++] = s; cost += s;
    }
    free(a); free(q); return cost;
}

static int check(Sym *s, int n, long long cost) {                   /* prefix-free + Kraft + cost */
    double kraft = 0; long long c2 = 0;
    for (int i = 0; i < n; i++) { kraft += 1.0 / (double)(1ULL << (s[i].len > 62 ? 62 : s[i].len)); c2 += s[i].f * s[i].len;
        for (int j = 0; j < n; j++) if (i != j) { int l = s[i].len; if (l <= s[j].len && !strncmp(s[i].code, s[j].code, l)) return 0; } }
    return (n == 1 || (kraft > 0.999999 && kraft < 1.000001)) && c2 == cost;
}

int main(int argc, char **argv) {
    if (argc > 1 && !strcmp(argv[1], "--selftest")) {
        srand(11); int bad = 0;
        for (int tc = 0; tc < 2000; tc++) {
            int n = 1 + rand() % 12; Sym *s = calloc(n, sizeof(Sym)); long long *f = malloc(n*sizeof(long long)); int *len = malloc(n*sizeof(int));
            for (int i = 0; i < n; i++) { sprintf(s[i].sym, "s%02d", i); f[i] = s[i].f = 1 + rand() % 20; }
            long long cost = huffman_lengths(f, n, len);
            for (int i = 0; i < n; i++) s[i].len = len[i];
            canonical(s, n);
            if (!check(s, n, cost) || (n > 1 && cost != two_queue_cost(f, n))) { bad++; printf("FAIL tc=%d\n", tc); }
        }
        printf("selftest: 2000 cases, %d failures\n", bad); return bad != 0;
    }
    int n; if (scanf("%d", &n) != 1) return 1;
    Sym *s = calloc(n, sizeof(Sym)); long long *f = malloc(n*sizeof(long long)); int *len = malloc(n*sizeof(int)); long long tot = 0;
    for (int i = 0; i < n; i++) { if (scanf("%31s %lld", s[i].sym, &s[i].f) != 2) return 1; f[i] = s[i].f; tot += f[i]; }
    long long cost = huffman_lengths(f, n, len);
    for (int i = 0; i < n; i++) s[i].len = len[i];
    canonical(s, n);
    printf("Canonical Huffman codebook (ordered by length, then symbol):\n%-8s %-8s %-4s %s\n", "symbol", "freq", "len", "code");
    for (int i = 0; i < n; i++) printf("%-8s %-8lld %-4d %s\n", s[i].sym, s[i].f, s[i].len, s[i].code);
    printf("Total encoded bits = %lld, expected length = %.4f bits/symbol, valid = %s\n",
           cost, (double)cost / tot, check(s, n, cost) ? "yes" : "NO");
    return 0;
}
