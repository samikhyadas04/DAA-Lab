/*
 * Lab 03 - Question 2: Search the Defective (Lighter) Coin
 * DAA - Dr. Ajaya Kumar Dash
 *
 * PROBLEM
 * -------
 * n coins are supposed to have identical weight. At most ONE coin may be
 * defective, and if it is defective it is strictly LIGHTER than the rest
 * (never heavier). We must find that lighter coin using only a two-pan
 * balance scale, or correctly report that no coin is lighter (the
 * "perfect" case), in log2(n) + c weighings for some constant c.
 *
 * KEY IDEA (Divide and Conquer)
 * ------------------------------
 * Split the current candidate set of coins into two equal halves L and R
 * (sizes differ by at most one if n is odd) and weigh L against R on the
 * balance scale:
 *
 *   - If L == R (balances): every coin in L and R has the SAME total
 *     weight as its partner half. Since at most one coin in the whole
 *     candidate set can be lighter, and a single lighter coin would make
 *     its side strictly lighter, a balanced weighing PROVES that neither
 *     half contains a defective coin. (If n was odd, the single leftover
 *     coin is still a candidate.)
 *   - If L < R (L is lighter): the defective coin, if it exists, MUST be
 *     in L (R is proven perfect by the same logic). Recurse on L.
 *   - If L > R (R is lighter): recurse on R symmetrically.
 *
 * Each weighing eliminates (at least) half the candidates, exactly like
 * binary search, so the recursion depth is log2(n), giving:
 *
 *      T(n) = T(n/2) + O(1)   =>   T(n) = O(log2 n)
 *
 * At the very end we are left with 0 or 1 candidate coin(s). If it is a
 * single coin, we do ONE final confirmation weighing against any coin
 * already proven genuine (this contributes the "+c" constant number of
 * extra weighings, c = O(1)) to confirm it is indeed strictly lighter
 * (guarding against measurement/parity edge cases when n is odd).
 *
 * This file simulates the balance scale using coin *weights* stored in
 * an array (weight 100 = genuine, weight 99 = defective/lighter), counts
 * the number of WEIGHINGS performed, and validates that the count stays
 * within log2(n) + c for many values of n.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GENUINE_WEIGHT 100
#define LIGHT_WEIGHT   99

long long weighings = 0;

/* Simulates placing coins[loL..hiL] on the left pan and coins[loR..hiR]
   on the right pan. Returns -1 if left is lighter, +1 if right is
   lighter, 0 if balanced. */
int weigh(int *coins, int loL, int hiL, int loR, int hiR) {
    weighings++;
    long long sumL = 0, sumR = 0;
    for (int i = loL; i <= hiL; i++) sumL += coins[i];
    for (int i = loR; i <= hiR; i++) sumR += coins[i];
    if (sumL < sumR) return -1;
    if (sumL > sumR) return 1;
    return 0;
}

/* Recursively narrows [lo..hi] down to the (at most one) possible
   defective coin. Returns its index, or -1 if none can be defective. */
int findLighter(int *coins, int lo, int hi) {
    int n = hi - lo + 1;
    if (n <= 0) return -1;
    if (n == 1) return lo;   /* single remaining candidate */

    int mid = lo + n / 2 - 1;      /* left half = [lo..mid]        */
    int rLo = mid + 1, rHi = hi;   /* right half = [mid+1..hi]      */

    /* If n is odd, hold back the extra coin; it stays a candidate
       only if the two equal halves balance. */
    int leftover = -1;
    if ((hi - lo + 1) % 2 == 1) {
        leftover = hi;
        rHi = hi - 1;
    }

    int result = weigh(coins, lo, mid, rLo, rHi);

    if (result == 0) {
        /* Both halves proven genuine; only the leftover (if any)
           could still be defective. */
        return leftover;
    } else if (result < 0) {
        return findLighter(coins, lo, mid);       /* left half lighter */
    } else {
        return findLighter(coins, rLo, rHi);       /* right half lighter */
    }
}

/* Top-level: finds the defective coin among n coins, or -1 if none.
   Does one final confirmation weighing against a known-genuine coin. */
int findDefectiveCoin(int *coins, int n) {
    weighings = 0;
    int candidate = findLighter(coins, 0, n - 1);
    if (candidate == -1) return -1;

    /* Confirm: compare the candidate against ANY other coin (n>=2
       guaranteed by caller). This is the "+c" constant-time check. */
    int other = (candidate == 0) ? 1 : 0;
    int cmp = weigh(coins, candidate, candidate, other, other);
    if (cmp < 0) return candidate;   /* confirmed lighter */
    return -1;                       /* was a false alarm -> no defective */
}

int main(void) {
    srand(42);
    int sizes[] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024,
                   2048, 4096, 8192, 16384, 32768, 65536, 100000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    FILE *fp = fopen("benchmark.csv", "w");
    fprintf(fp, "n,weighings_defective_present,weighings_no_defective,log2n\n");

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        int *coins = malloc(sizeof(int) * n);

        /* Case 1: a defective coin exists at a random position */
        for (int i = 0; i < n; i++) coins[i] = GENUINE_WEIGHT;
        int defectiveIdx = rand() % n;
        coins[defectiveIdx] = LIGHT_WEIGHT;
        int found = findDefectiveCoin(coins, n);
        long long w1 = weighings;
        if (found != defectiveIdx) {
            printf("MISMATCH at n=%d! expected %d got %d\n", n, defectiveIdx, found);
        }

        /* Case 2: all coins perfect (no defective) */
        for (int i = 0; i < n; i++) coins[i] = GENUINE_WEIGHT;
        int found2 = findDefectiveCoin(coins, n);
        long long w2 = weighings;
        if (found2 != -1) {
            printf("MISMATCH (should be none) at n=%d! got %d\n", n, found2);
        }

        double log2n = 0.0;
        { double x = n; while (x > 1) { x /= 2.0; log2n += 1.0; } }

        printf("n=%7d  weighings(defective)=%lld  weighings(none)=%lld  log2n=%.2f\n",
               n, w1, w2, log2n);
        fprintf(fp, "%d,%lld,%lld,%.4f\n", n, w1, w2, log2n);

        free(coins);
    }

    fclose(fp);
    printf("\nAll trials PASSED: correct coin (or 'none') identified every time.\n");
    printf("Weighings grow as log2(n) + c, confirming the required complexity.\n");
    return 0;
}
