/* =====================================================================
   hanoi4.c
   ---------------------------------------------------------------------
   Problem: n disks, 4 pegs. Move all n disks from peg 0 to another
   peg, one disk at a time, never placing a larger disk on a smaller
   one. Solve in the minimum number of moves (33 for n=8), and
   generalize to n disks.

   ALGORITHM (Frame-Stewart)
   --------------------------
   FS(n, src, aux1, aux2, dst):
     if n == 0: return
     choose a split point k (0<=k<=n) that minimizes
         2*FS_moves(k) + Hanoi3_moves(n-k)     where Hanoi3_moves(m)=2^m-1
     FS(k, src, dst, aux2, aux1)        // move top k disks out of the way,
                                         // using all 4 pegs, park them on aux1
     Hanoi3(n-k, src, aux2, dst)        // move remaining n-k disks with the
                                         // classic 3-peg algorithm (aux1 is
                                         // occupied/unavailable as a spare)
     FS(k, aux1, src, aux2, dst)        // bring the k parked disks onto dst,
                                         // again using all 4 pegs

   The optimal split point k for each n is found once via dynamic
   programming (not guessed/hard-coded), so the algorithm is fully
   general in n. For n=8 this reproduces the well-known optimal total
   of 33 moves.

   VALIDATION
   ----------
   Every generated move is replayed against an explicit array-based
   model of the 4 pegs; the validator checks (a) only the top disk of
   a peg is ever moved, and (b) a disk is never placed on a smaller
   one. At the end it checks that all n disks ended on the target peg
   in correct order. It also cross-checks the *move count* against the
   DP-computed optimal value.

   COMPLEXITY
   ----------
   DP precompute of optimal move counts/splits for disks 1..n: O(n^2)
   time (for each n, try every split k), O(n) space.
   Move generation: produces Theta(f(n)) moves, where for 4 pegs
   f(n) = Theta(2^(sqrt(2n))) (sub-exponential, dramatically better
   than the Theta(2^n) of the 3-peg case). Generating the sequence
   takes time linear in its own length, i.e. O(f(n)).
   ===================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 40
static long long fs_moves[MAXN + 1];   /* optimal move count for k disks, 4 pegs */
static int fs_split[MAXN + 1];         /* optimal split point k for n disks      */

typedef struct { int from, to; } Move;
static Move moves[1 << 20];
static int move_count = 0;

static long long hanoi3_moves(int m) { return (1LL << m) - 1; } /* 2^m - 1 */

static void precompute(int n) {
    fs_moves[0] = 0;
    for (int m = 1; m <= n; m++) {
        long long best = -1;
        int bestk = 0;
        for (int k = 0; k < m; k++) {   /* k must be < m: fs_moves[m] is not yet known */
            long long cost = 2 * fs_moves[k] + hanoi3_moves(m - k);
            if (best == -1 || cost < best) { best = cost; bestk = k; }
        }
        fs_moves[m] = best;
        fs_split[m] = bestk;
    }
}

/* classic 3-peg Hanoi, using only pegs {src,spare,dst} (aux4 untouched) */
static void hanoi3(int m, int src, int spare, int dst) {
    if (m == 0) return;
    hanoi3(m - 1, src, dst, spare);
    moves[move_count++] = (Move){src, dst};
    hanoi3(m - 1, spare, src, dst);
}

static void frame_stewart(int n, int src, int aux1, int aux2, int dst) {
    if (n == 0) return;
    int k = fs_split[n];
    frame_stewart(k, src, dst, aux2, aux1);   /* park k disks on aux1, using dst as helper */
    hanoi3(n - k, src, aux2, dst);            /* move n-k disks, aux1 unavailable */
    frame_stewart(k, aux1, src, aux2, dst);   /* bring k disks onto dst */
}

/* ---- validator: replay moves on an explicit peg model ---- */
static int validate(int n, int npegs, int start_peg, int target_peg) {
    int pegs[4][MAXN];
    int height[4] = {0, 0, 0, 0};
    for (int d = n; d >= 1; d--) pegs[start_peg][height[start_peg]++] = d; /* n..1, n at bottom */

    for (int i = 0; i < move_count; i++) {
        int f = moves[i].from, t = moves[i].to;
        if (f < 0 || f >= npegs || t < 0 || t >= npegs) { printf("bad peg index\n"); return 0; }
        if (height[f] == 0) { printf("move %d: source peg %d empty\n", i, f); return 0; }
        int disk = pegs[f][height[f] - 1];
        if (height[t] > 0 && pegs[t][height[t] - 1] < disk) {
            printf("move %d: illegal, placing disk %d on smaller disk %d\n",
                   i, disk, pegs[t][height[t] - 1]);
            return 0;
        }
        height[f]--;
        pegs[t][height[t]++] = disk;
    }
    if (height[target_peg] != n) { printf("not all disks on target peg\n"); return 0; }
    for (int i = 0; i < n; i++)
        if (pegs[target_peg][i] != n - i) { printf("wrong final order\n"); return 0; }
    for (int p = 0; p < npegs; p++)
        if (p != target_peg && height[p] != 0) { printf("stray disks left on peg %d\n", p); return 0; }
    return 1;
}

int main(int argc, char **argv) {
    int n = 8;
    if (argc > 1) n = atoi(argv[1]);
    if (n < 1 || n > MAXN) { fprintf(stderr, "n must be in [1,%d]\n", MAXN); return 1; }

    precompute(n);

    printf("4-peg Tower of Hanoi (Frame-Stewart), n=%d disks\n", n);
    printf("=================================================\n");
    printf("Optimal split points k(m) for m=1..%d: ", n);
    for (int m = 1; m <= n; m++) printf("%d%s", fs_split[m], m < n ? "," : "");
    printf("\n");
    printf("DP-predicted optimal move count for n=%d: %lld\n", n, fs_moves[n]);

    move_count = 0;
    frame_stewart(n, 0, 1, 2, 3);   /* pegs 0=src,1,2 spare,3=dst */
    printf("Moves actually generated by algorithm: %d\n", move_count);

    int ok = validate(n, 4, 0, 3);
    printf("Move-by-move legality & final-state check: %s\n", ok ? "PASS" : "FAIL");
    printf("Move count matches DP optimum: %s\n", (move_count == fs_moves[n]) ? "yes" : "no");

    if (n <= 8) {
        printf("\nFull move list (peg 0..3, 0-indexed):\n");
        for (int i = 0; i < move_count; i++)
            printf("  %3d: peg %d -> peg %d\n", i + 1, moves[i].from, moves[i].to);
    }

    printf("\n=================================================\n");
    printf(ok && move_count == fs_moves[n] ? "RESULT: algorithm VALIDATED.\n"
                                            : "RESULT: VALIDATION FAILED.\n");

    /* quick summary table for a range of n, like the textbook table */
    printf("\nSummary table (n : optimal moves) for n=1..%d:\n", n < 16 ? 16 : n);
    int top = n < 16 ? 16 : n;
    precompute(top);
    for (int m = 1; m <= top; m++) printf("  n=%2d -> %lld moves\n", m, fs_moves[m]);

    return (ok && move_count == fs_moves[n]) ? 0 : 1;
}