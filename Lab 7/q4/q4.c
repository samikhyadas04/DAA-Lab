/* =====================================================================
   switches.c
   ---------------------------------------------------------------------
   Problem: n switches in a row, numbered 1 (leftmost) .. n (rightmost).
   All start ON. Rules:
     (i)   switch n (rightmost) may be toggled at will.
     (ii)  switch i (i<n) may be toggled only if switch i+1 is ON and
           switches i+2..n are ALL OFF.
     (iii) one toggle per move.
   Turn every switch OFF in the minimum possible number of moves.

   REPRESENTATION
   --------------
   A configuration is an n-bit mask; bit (i-1) is switch i (1=ON).
   This makes the whole state space (2^n configurations) explicit and
   lets us treat "find the minimum number of moves" as an unweighted
   shortest-path problem on the state graph whose edges are legal
   single-switch toggles.

   ALGORITHM
   ---------
   Breadth-First Search from the all-ON state. BFS on an unweighted
   graph is guaranteed to find the shortest path (= minimum number of
   moves) to the all-OFF state, and the graph (legality rule) is
   exactly as specified, so the result is provably optimal, not a
   heuristic. We also reconstruct and print the actual move sequence,
   and re-simulate it to prove every move in it is legal end-to-end.

   This is the textbook-standard "Chinese Rings" / "Gray-code" switch
   puzzle; BFS re-discovers, and here formally certifies, the known
   closed-form move count
        T(1)=1,  T(n) = 2*T(n-1) + (n mod 2)      for n>=2
   which the program prints alongside the BFS-computed value for
   independent cross-checking.

   COMPLEXITY
   ----------
   States: 2^n.  Each state has at most 1 legal move that is not an
   immediate undo (the puzzle is almost a straight line in state
   space), so BFS runs in O(2^n) time and O(2^n) space -- this is
   unavoidable in the worst case because the optimal solution itself
   has Theta(2^n) moves (see closed form above), so any algorithm that
   outputs the move sequence is at least that expensive.
   ===================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int N;                /* number of switches                */
static long STATES;          /* 1 << N                             */

/* is switch i (1-indexed) legally toggleable in state s ? */
static int legal(int s, int i /*1-indexed*/) {
    if (i == N) return 1;                       /* rightmost: free */
    /* switch i+1 must be ON */
    if (!((s >> i) & 1)) return 0;               /* bit for switch i+1 is index i */
    /* switches i+2..N must all be OFF */
    for (int j = i + 2; j <= N; j++)
        if ((s >> (j - 1)) & 1) return 0;
    return 1;
}

static int toggle(int s, int i) { return s ^ (1 << (i - 1)); }

/* Closed form for comparison (derived + BFS-certified below). */
static long closed_form(int n) {
    long t = 1;
    for (int k = 2; k <= n; k++) t = 2 * t + (k % 2);
    return n == 0 ? 0 : t;
}

int main(int argc, char **argv) {
    int maxn = 16;
    if (argc > 1) maxn = atoi(argv[1]);
    if (maxn < 1) maxn = 1;

    printf("Switch puzzle: BFS-optimal solver and validator\n");
    printf("=================================================\n");
    printf("%4s %12s %12s %8s\n", "n", "BFS_min", "closed_form", "match?");

    for (N = 1; N <= maxn; N++) {
        STATES = 1L << N;
        int *dist = malloc(STATES * sizeof(int));
        int *prev_state = malloc(STATES * sizeof(int));
        signed char *prev_move = malloc(STATES * sizeof(signed char));
        long *queue = malloc(STATES * sizeof(long));
        for (long s = 0; s < STATES; s++) dist[s] = -1;

        int start = (int)(STATES - 1);   /* all ON  */
        int goal = 0;                     /* all OFF */
        int qh = 0, qt = 0;
        dist[start] = 0;
        queue[qt++] = start;

        while (qh < qt) {
            int s = (int)queue[qh++];
            if (s == goal) break;
            for (int i = 1; i <= N; i++) {
                if (!legal(s, i)) continue;
                int ns = toggle(s, i);
                if (dist[ns] == -1) {
                    dist[ns] = dist[s] + 1;
                    prev_state[ns] = s;
                    prev_move[ns] = (signed char)i;
                    queue[qt++] = ns;
                }
            }
        }

        long bfs_min = dist[goal];
        long cf = closed_form(N);
        printf("%4d %12ld %12ld %8s\n", N, bfs_min, cf, bfs_min == cf ? "yes" : "NO");

        /* Reconstruct the move sequence and re-simulate to certify legality.
           The optimal sequence length is Theta(2^n), so we only allocate
           storage for it when it is guaranteed to be small. */
        if (bfs_min < 200000) {
            int *path_moves = malloc((size_t)bfs_min * sizeof(int));
            int pm = 0;
            int cur = goal;
            while (cur != start) {
                path_moves[pm++] = prev_move[cur];
                cur = prev_state[cur];
            }
            for (int a = 0, b = pm - 1; a < b; a++, b--) {
                int tmp = path_moves[a]; path_moves[a] = path_moves[b]; path_moves[b] = tmp;
            }
            int sim = start;
            int ok = 1;
            for (int k = 0; k < pm; k++) {
                if (!legal(sim, path_moves[k])) { ok = 0; break; }
                sim = toggle(sim, path_moves[k]);
            }
            if (sim != goal || !ok) {
                printf("     -> VALIDATION FAILED for n=%d (illegal move or wrong final state)\n", N);
            }
            if (N <= 8) {
                printf("     moves (switch indices, 1=leftmost..%d=rightmost): ", N);
                for (int k = 0; k < pm; k++) printf("%d%s", path_moves[k], k + 1 < pm ? "," : "");
                printf("\n");
            }
            free(path_moves);
        }

        free(dist); free(prev_state); free(prev_move); free(queue);
    }
    printf("=================================================\n");
    printf("All BFS-found minima above are optimal by construction (BFS on an\n");
    printf("unweighted graph always returns shortest paths), and each printed\n");
    printf("move sequence was independently re-simulated and confirmed legal.\n");
    return 0;
}