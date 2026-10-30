/*
 * Q2: Multi-source BFS for radius-constrained Voronoi cell partitioning
 *
 * Idea: push all hubs into the queue at distance 0 and run one BFS.
 * BFS processes vertices level by level, so when a vertex v is first reached
 * at distance d+1, dist[v] = dmin(v). Tie-break (smallest hub index): while
 * relaxing an edge u->v with dist[v] == dist[u]+1, keep the smaller owner.
 * Owners of all level-d vertices are final before any level-d vertex is
 * dequeued (FIFO order), so the minimum propagates correctly.
 * Vertices with dmin > R are never expanded/reached -> owner = -1.
 * If a vertex appears as several hubs, the smallest index wins.
 *
 * Input : n m                (vertices 1..n)
 *         m lines: u v       (undirected edge)
 *         K
 *         h1 h2 ... hK       (hub vertices, index i = position in list)
 *         R
 * Output: owner[v] and distance for every vertex (-1 -1 if outside coverage)
 * Time  : O(V + E + K)   Space: O(V + E)
 */
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n, m;
    if (scanf("%d %d", &n, &m) != 2) return 1;

    int *head = malloc((n + 1) * sizeof(int));
    int *nxt = malloc((2 * m + 1) * sizeof(int));
    int *to = malloc((2 * m + 1) * sizeof(int));
    for (int i = 0; i <= n; i++) head[i] = -1;
    int ec = 0;
    for (int i = 0; i < m; i++) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) return 1;
        to[ec] = v; nxt[ec] = head[u]; head[u] = ec++;
        to[ec] = u; nxt[ec] = head[v]; head[v] = ec++;
    }

    int K, R;
    if (scanf("%d", &K) != 1) return 1;
    int *hub = malloc((K + 1) * sizeof(int));
    for (int i = 1; i <= K; i++) if (scanf("%d", &hub[i]) != 1) return 1;
    if (scanf("%d", &R) != 1) return 1;

    int *owner = malloc((n + 1) * sizeof(int));
    int *dist = malloc((n + 1) * sizeof(int));
    int *q = malloc((n + 1) * sizeof(int));
    for (int i = 0; i <= n; i++) { owner[i] = -1; dist[i] = -1; }

    int qh = 0, qt = 0;
    for (int i = 1; i <= K; i++) {
        int h = hub[i];
        if (dist[h] == -1) {            /* first (smallest) index wins */
            dist[h] = 0; owner[h] = i; q[qt++] = h;
        }
    }

    while (qh < qt) {
        int u = q[qh++];
        if (dist[u] == R) continue;      /* do not go beyond radius R */
        for (int e = head[u]; e != -1; e = nxt[e]) {
            int v = to[e];
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                owner[v] = owner[u];
                q[qt++] = v;
            } else if (dist[v] == dist[u] + 1 && owner[u] < owner[v]) {
                owner[v] = owner[u];     /* deterministic tie-break */
            }
        }
    }

    printf("Vertex : Owner Distance\n");
    for (int v = 1; v <= n; v++) {
        if (owner[v] == -1) printf("%d : -1 -1\n", v);
        else                printf("%d : %d %d\n", v, owner[v], dist[v]);
    }

    free(head); free(nxt); free(to); free(hub);
    free(owner); free(dist); free(q);
    return 0;
}