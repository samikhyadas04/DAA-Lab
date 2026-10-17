/* Q3: Maximum weight component with size constraint (size <= K), edges added online.
 * Representation: edge stream + DSU storing per-root size sz[] and weight sum[].
 *   A max-heap with LAZY DELETION stores candidate components (sum, root, size).
 *   An entry is valid iff root is still a root and its size is unchanged
 *   (sizes strictly grow on every merge, so size identifies the version).
 *   The heap is needed because the best component may merge into a component
 *   larger than K, after which the answer can DECREASE.
 * Input : V E K ; V vertex weights ; E lines "u v w" (edge weight read, not needed
 *         for the query).  Vertices are 1..V.
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct { long long w; int root, size; } Node;
static Node *h; static int hn = 0;
static int *parent, *sz;
static long long *sum;

static void push(Node x) {
    int i = hn++;
    while (i > 0 && h[(i - 1) / 2].w < x.w) { h[i] = h[(i - 1) / 2]; i = (i - 1) / 2; }
    h[i] = x;
}
static void pop(void) {
    Node x = h[--hn];
    int i = 0;
    while (2 * i + 1 < hn) {
        int c = 2 * i + 1;
        if (c + 1 < hn && h[c + 1].w > h[c].w) c++;
        if (h[c].w <= x.w) break;
        h[i] = h[c]; i = c;
    }
    if (hn > 0) h[i] = x;
}
static int find(int x) {
    while (parent[x] != x) { parent[x] = parent[parent[x]]; x = parent[x]; }
    return x;
}

int main(void) {
    int V, E, K;
    if (scanf("%d %d %d", &V, &E, &K) != 3) return 1;
    parent = malloc((V + 1) * sizeof(int));
    sz = malloc((V + 1) * sizeof(int));
    sum = malloc((V + 1) * sizeof(long long));
    h = malloc((size_t)(V + E + 1) * sizeof(Node));
    for (int i = 1; i <= V; i++) {
        long long w; scanf("%lld", &w);
        parent[i] = i; sz[i] = 1; sum[i] = w;
        if (1 <= K) push((Node){w, i, 1});
    }
    for (int i = 0; i < E; i++) {
        int u, v; long long ew;
        scanf("%d %d %lld", &u, &v, &ew);
        int a = find(u), b = find(v);
        if (a != b) {
            if (sz[a] < sz[b]) { int t = a; a = b; b = t; }
            parent[b] = a; sz[a] += sz[b]; sum[a] += sum[b];
            if (sz[a] <= K) push((Node){sum[a], a, sz[a]});
        }
        while (hn > 0 && !(parent[h[0].root] == h[0].root && sz[h[0].root] == h[0].size)) pop();
        if (hn > 0) printf("After edge %d (%d,%d): max weight = %lld\n", i + 1, u, v, h[0].w);
        else        printf("After edge %d (%d,%d): no component with size <= %d\n", i + 1, u, v, K);
    }
    free(parent); free(sz); free(sum); free(h);
    return 0;
}
