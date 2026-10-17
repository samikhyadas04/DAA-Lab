/* Q2: Dynamic connected components in a grid (Number of Islands II).
 * Representation: R x C grid flattened to 1-D id = r*C + c;
 *   land[id] marks land cells, DSU arrays parent[] / sz[] over flattened ids.
 * Input : R C K, then K lines "r c" (0-indexed).
 */
#include <stdio.h>
#include <stdlib.h>

static int *parent, *sz;

static int find(int x) {
    while (parent[x] != x) { parent[x] = parent[parent[x]]; x = parent[x]; } /* path halving */
    return x;
}
static int unite(int a, int b) {
    a = find(a); b = find(b);
    if (a == b) return 0;
    if (sz[a] < sz[b]) { int t = a; a = b; b = t; }
    parent[b] = a; sz[a] += sz[b];
    return 1;
}

int main(void) {
    int R, C, K;
    if (scanf("%d %d %d", &R, &C, &K) != 3) return 1;
    int N = R * C;
    parent = malloc(N * sizeof(int));
    sz = malloc(N * sizeof(int));
    char *land = calloc(N, 1);
    for (int i = 0; i < N; i++) { parent[i] = i; sz[i] = 1; }

    int dr[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
    int count = 0;
    for (int i = 0; i < K; i++) {
        int r, c;
        scanf("%d %d", &r, &c);
        if (r < 0 || r >= R || c < 0 || c >= C) { printf("(%d,%d) out of range -> %d\n", r, c, count); continue; }
        int id = r * C + c;
        if (!land[id]) {
            land[id] = 1;
            count++;
            for (int d = 0; d < 4; d++) {
                int nr = r + dr[d], nc = c + dc[d];
                if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
                int nid = nr * C + nc;
                if (land[nid] && unite(id, nid)) count--;
            }
        }
        printf("After adding (%d,%d): islands = %d\n", r, c, count);
    }
    free(parent); free(sz); free(land);
    return 0;
}
