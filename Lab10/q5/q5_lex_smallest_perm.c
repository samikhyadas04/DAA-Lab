/* Q5: Lexicographically smallest permutation using allowed swaps.
 * Representation: edge list of allowed swaps over POSITIONS 1..n, handled by DSU.
 *   Positions in one component can be rearranged arbitrarily, so for every
 *   component: sort its values ascending and put them into its sorted positions.
 *   Implementation: two arrays of (root, pos) and (root, val), each sorted by
 *   (root, key); the k-th entries of both arrays belong to the same component,
 *   so result[pos_k] = val_k.
 * Input : n ; A[1..n] ; m ; m lines "u v" (swap positions u and v allowed)
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct { int root, key; } Pair;
static int *parent, *sz;

static int find(int x) {
    while (parent[x] != x) { parent[x] = parent[parent[x]]; x = parent[x]; }
    return x;
}
static int cmp(const void *a, const void *b) {
    const Pair *x = a, *y = b;
    if (x->root != y->root) return x->root < y->root ? -1 : 1;
    return (x->key > y->key) - (x->key < y->key);
}

int main(void) {
    int n, m;
    if (scanf("%d", &n) != 1) return 1;
    int *A = malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= n; i++) scanf("%d", &A[i]);
    scanf("%d", &m);
    parent = malloc((n + 1) * sizeof(int));
    sz = malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= n; i++) { parent[i] = i; sz[i] = 1; }
    for (int i = 0; i < m; i++) {
        int u, v; scanf("%d %d", &u, &v);
        int a = find(u), b = find(v);
        if (a == b) continue;
        if (sz[a] < sz[b]) { int t = a; a = b; b = t; }
        parent[b] = a; sz[a] += sz[b];
    }
    Pair *P = malloc(n * sizeof(Pair)), *Q = malloc(n * sizeof(Pair));
    for (int i = 1; i <= n; i++) {
        int r = find(i);
        P[i - 1] = (Pair){r, i};        /* positions */
        Q[i - 1] = (Pair){r, A[i]};     /* values    */
    }
    qsort(P, n, sizeof(Pair), cmp);
    qsort(Q, n, sizeof(Pair), cmp);
    int *res = malloc((n + 1) * sizeof(int));
    for (int k = 0; k < n; k++) res[P[k].key] = Q[k].key;
    printf("Lexicographically smallest permutation:\n");
    for (int i = 1; i <= n; i++) printf("%d%c", res[i], i == n ? '\n' : ' ');
    free(A); free(parent); free(sz); free(P); free(Q); free(res);
    return 0;
}
