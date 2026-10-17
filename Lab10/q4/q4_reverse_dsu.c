/* Q4: Reverse time-step DSU. Deleting vertices = adding them in reverse order.
 * Representation: adjacency list (arrays head/nxt/to, 2E entries) because we must
 *   iterate over the neighbours of each re-inserted vertex.
 *   DSU with per-root weight sum; active[] marks vertices currently present.
 * Input : V E ; V vertex weights ; E lines "u v" ; deletion order p1..pV
 *         (vertices are 1..V).
 * Output: ans[i] = max component sum after deleting p1..pi (i = 1..V).
 */
#include <stdio.h>
#include <stdlib.h>

static int *parent, *sz;
static long long *sum;

static int find(int x) {
    while (parent[x] != x) { parent[x] = parent[parent[x]]; x = parent[x]; }
    return x;
}

int main(void) {
    int V, E;
    if (scanf("%d %d", &V, &E) != 2) return 1;
    long long *w = malloc((V + 1) * sizeof(long long));
    for (int i = 1; i <= V; i++) scanf("%lld", &w[i]);

    int *head = malloc((V + 1) * sizeof(int));
    int *nxt = malloc(2 * (size_t)E * sizeof(int));
    int *to = malloc(2 * (size_t)E * sizeof(int));
    for (int i = 0; i <= V; i++) head[i] = -1;
    int ec = 0;
    for (int i = 0; i < E; i++) {
        int u, v; scanf("%d %d", &u, &v);
        to[ec] = v; nxt[ec] = head[u]; head[u] = ec++;
        to[ec] = u; nxt[ec] = head[v]; head[v] = ec++;
    }
    int *p = malloc((V + 1) * sizeof(int));
    for (int i = 1; i <= V; i++) scanf("%d", &p[i]);

    parent = malloc((V + 1) * sizeof(int));
    sz = malloc((V + 1) * sizeof(int));
    sum = malloc((V + 1) * sizeof(long long));
    char *active = calloc(V + 1, 1);
    long long *ans = calloc(V + 1, sizeof(long long));
    for (int i = 1; i <= V; i++) { parent[i] = i; sz[i] = 1; sum[i] = w[i]; }

    long long best = 0;
    ans[V] = 0;                         /* everything deleted */
    for (int i = V; i >= 2; i--) {      /* re-insert p_i, giving state after i-1 deletions */
        int v = p[i];
        active[v] = 1;
        for (int e = head[v]; e != -1; e = nxt[e]) {
            int u = to[e];
            if (!active[u]) continue;
            int a = find(v), b = find(u);
            if (a == b) continue;
            if (sz[a] < sz[b]) { int t = a; a = b; b = t; }
            parent[b] = a; sz[a] += sz[b]; sum[a] += sum[b];
        }
        long long s = sum[find(v)];
        if (s > best) best = s;
        ans[i - 1] = best;
    }
    for (int i = 1; i <= V; i++) printf("After deleting %d vertex(es) (last: %d): %lld\n", i, p[i], ans[i]);
    free(w); free(head); free(nxt); free(to); free(p);
    free(parent); free(sz); free(sum); free(active); free(ans);
    return 0;
}
