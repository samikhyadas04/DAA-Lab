

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { int x, y; } Point;

/* ---- closed-form formula, derived & verified by search (see report) --- */
static long formulaMoves(long n) {
    return (n * (n + 1)) / 6; /* integer division = floor, n*(n+1) >= 0 */
}

static int T_coins(int n) { return n * (n + 1) / 2; }

/* Build the "up" (apex-top) triangle of n rows into pts[] (size T(n)) */
static void buildUp(int n, Point *pts) {
    int k = 0;
    for (int Y = 0; Y < n; Y++)
        for (int j = 0; j <= Y; j++)
            pts[k++] = (Point){ 2 * j - Y, Y };
}

/* Build the "down" (apex-bottom) triangle of n rows, unshifted */
static void buildDown(int n, Point *pts) {
    int k = 0;
    for (int Y = 0; Y < n; Y++) {
        int cnt = n - Y;
        for (int j = 0; j < cnt; j++)
            pts[k++] = (Point){ 2 * j - (cnt - 1), Y };
    }
}

int main(int argc, char *argv[]) {
    int n = (argc > 1) ? atoi(argv[1]) : 4;   /* default: Figure 1's 4-row, 10-coin case */
    int T = T_coins(n);

    Point *up   = (Point *)malloc(T * sizeof(Point));
    Point *down = (Point *)malloc(T * sizeof(Point));
    buildUp(n, up);
    buildDown(n, down);

    /* --- direct-address occupancy grid for the "up" triangle --- */
    int XOFF = 2 * n + 2, YOFF = 2 * n + 2;
    int W = 4 * n + 5, H = 4 * n + 5;
    char *grid = (char *)calloc((size_t)W * H, 1);
    for (int i = 0; i < T; i++)
        grid[(up[i].y + YOFF) * W + (up[i].x + XOFF)] = 1;

    /* --- search all shifts (dx,dy) of the down triangle --- */
    int bestOverlap = -1, bestDx = 0, bestDy = 0;
    for (int dy = 0; dy <= n; dy++) {
        for (int dx = -n; dx <= n; dx++) {
            int overlap = 0;
            for (int i = 0; i < T; i++) {
                int gx = down[i].x + dx + XOFF;
                int gy = down[i].y + dy + YOFF;
                if (gx >= 0 && gx < W && gy >= 0 && gy < H && grid[gy * W + gx])
                    overlap++;
            }
            if (overlap > bestOverlap) {
                bestOverlap = overlap;
                bestDx = dx; bestDy = dy;
            }
        }
    }

    int moves = T - bestOverlap;
    long formula = formulaMoves(n);

    printf("n = %d rows, coins = %d\n", n, T);
    printf("Best placement of inverted triangle: shift (dx=%d, dy=%d)\n", bestDx, bestDy);
    printf("Max overlap = %d  =>  minimum moves (search) = %d\n", bestOverlap, moves);
    printf("Closed-form floor(n(n+1)/6)            = %ld   %s\n",
           formula, (formula == moves) ? "(MATCH)" : "(MISMATCH!)");

    /* --- reconstruct one explicit optimal move sequence --- */
    char *bestGrid = (char *)calloc((size_t)W * H, 1);
    for (int i = 0; i < T; i++) {
        int gx = down[i].x + bestDx + XOFF;
        int gy = down[i].y + bestDy + YOFF;
        bestGrid[gy * W + gx] = 1;
    }

    Point *from = (Point *)malloc(T * sizeof(Point));
    Point *to   = (Point *)malloc(T * sizeof(Point));
    int nf = 0, nt = 0;
    for (int i = 0; i < T; i++) {
        int gx = up[i].x + XOFF, gy = up[i].y + YOFF;
        if (!bestGrid[gy * W + gx]) from[nf++] = up[i];       /* must vacate */
    }
    for (int i = 0; i < T; i++) {
        int gx = down[i].x + bestDx + XOFF, gy = down[i].y + bestDy + YOFF;
        if (!grid[gy * W + gx]) to[nt++] = (Point){ down[i].x + bestDx, down[i].y + bestDy };
    }

    printf("\nExplicit move list (%d moves), (X,Y) in doubled-lattice coords:\n", nf);
    for (int i = 0; i < nf; i++)
        printf("  move coin (%2d,%2d)  ->  (%2d,%2d)\n", from[i].x, from[i].y, to[i].x, to[i].y);

    free(up); free(down); free(grid); free(bestGrid); free(from); free(to);
    return 0;
}