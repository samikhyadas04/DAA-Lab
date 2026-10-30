/*
 * Q1: Shortest path on parity-constrained graphs via 0-1 BFS
 *
 * Idea: Cost(P) = W(P) + (k1(P) odd ? C : 0). Because weights are 0/1,
 * W(P) == k1(P), so "k1 odd" is the same as "W odd".
 * Build a layered state graph with states (v, p), p = parity of W so far.
 *   weight-0 edge (u,v): (u,p) -> (v,p)     cost 0
 *   weight-1 edge (u,v): (u,p) -> (v,1-p)   cost 1
 * Run 0-1 BFS from (S,0). Then
 *   answer[v] = min( dist[v][0], dist[v][1] + C ).
 * (Paths are treated as walks: vertices/edges may repeat.)
 *
 * Input : n m S C            (vertices are 1..n)
 *         m lines: u v w     (directed edge u->v, w in {0,1})
 * Output: for each vertex, min cost or UNREACHABLE
 * Time  : O(V + E)   Space: O(V + E)
 */
#include <stdio.h>
#include <stdlib.h>

#define INF 1000000000LL

int main(void) {
    int n, m, s;
    long long C;
    if (scanf("%d %d %d %lld", &n, &m, &s, &C) != 4) return 1;

    int *head = malloc((n + 1) * sizeof(int));
    int *nxt = malloc((m > 0 ? m : 1) * sizeof(int));
    int *to = malloc((m > 0 ? m : 1) * sizeof(int));
    int *wt = malloc((m > 0 ? m : 1) * sizeof(int));
    for (int i = 0; i <= n; i++) head[i] = -1;

    for (int i = 0; i < m; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) return 1;
        to[i] = v; wt[i] = w;
        nxt[i] = head[u]; head[u] = i;
    }

    int states = 2 * (n + 1);
    long long *dist = malloc(states * sizeof(long long));
    char *done = calloc(states, 1);
    for (int i = 0; i < states; i++) dist[i] = INF;

    /* deque stored as an array; at most 2m+1 pushes to each end */
    int cap = 2 * (2 * m + 2);
    int *dq = malloc(cap * sizeof(int));
    int front = cap / 2, back = cap / 2;       /* elements in [front, back) */

    dist[s * 2] = 0;
    dq[back++] = s * 2;

    while (front < back) {
        int x = dq[front++];
        if (done[x]) continue;                 /* stale entry */
        done[x] = 1;
        int u = x / 2, p = x % 2;
        for (int e = head[u]; e != -1; e = nxt[e]) {
            int ns = to[e] * 2 + (p ^ wt[e]);
            long long nd = dist[x] + wt[e];
            if (nd < dist[ns]) {
                dist[ns] = nd;
                if (wt[e] == 0) dq[--front] = ns;
                else            dq[back++] = ns;
            }
        }
    }

    printf("Vertex : MinCost\n");
    for (int v = 1; v <= n; v++) {
        long long d0 = dist[v * 2], d1 = dist[v * 2 + 1];
        long long best = INF;
        if (d0 < INF) best = d0;
        if (d1 < INF && d1 + C < best) best = d1 + C;
        if (best >= INF) printf("%d : UNREACHABLE\n", v);
        else             printf("%d : %lld\n", v, best);
    }

    free(head); free(nxt); free(to); free(wt); free(dist); free(done); free(dq);
    return 0;
}