/* Q1: Online bipartiteness testing using DSU with parity.
 * Representation: edge stream (edges arrive online, no adjacency list needed).
 *   parent[v] : DSU parent        rnk[v] : rank (union by rank)
 *   par[v]    : parity (colour difference) of v relative to parent[v]
 * Input : V E, then E lines "u v"  (vertices are 1..V)
 */
#include <stdio.h>
#include <stdlib.h>

static int *parent, *rnk;
static unsigned char *par;

/* returns root of x; *p = parity of x relative to root (path compression) */
static int find(int x, int *p) {
    if (parent[x] == x) { *p = 0; return x; }
    int pp;
    int r = find(parent[x], &pp);
    par[x] ^= pp;
    parent[x] = r;
    *p = par[x];
    return r;
}

int main(void) {
    int V, E;
    if (scanf("%d %d", &V, &E) != 2) return 1;
    parent = malloc((V + 1) * sizeof(int));
    rnk    = calloc(V + 1, sizeof(int));
    par    = calloc(V + 1, 1);
    for (int i = 1; i <= V; i++) parent[i] = i;

    int accepted = 0, rejected = 0;
    for (int i = 0; i < E; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        if (u < 1 || u > V || v < 1 || v > V) { printf("Edge (%d,%d): invalid vertex\n", u, v); continue; }
        int pu, pv;
        int ru = find(u, &pu), rv = find(v, &pv);
        if (ru == rv) {
            if (pu == pv) {            /* same colour -> odd cycle */
                printf("Edge (%d,%d): REJECTED (creates odd cycle)\n", u, v);
                rejected++;
            } else {
                printf("Edge (%d,%d): ADDED (already same component, consistent)\n", u, v);
                accepted++;
            }
        } else {
            /* attach smaller rank under larger; set parity so u and v differ */
            if (rnk[ru] < rnk[rv]) { int t = ru; ru = rv; rv = t; }
            parent[rv] = ru;
            par[rv] = pu ^ pv ^ 1;
            if (rnk[ru] == rnk[rv]) rnk[ru]++;
            printf("Edge (%d,%d): ADDED (merged components)\n", u, v);
            accepted++;
        }
    }
    printf("\nAccepted: %d, Rejected: %d. Graph of accepted edges is bipartite.\n", accepted, rejected);
    printf("2-colouring (vertex:colour): ");
    for (int i = 1; i <= V; i++) { int p; find(i, &p); printf("%d:%d ", i, p); }
    printf("\n");
    free(parent); free(rnk); free(par);
    return 0;
}
