/* ============================================================
   DAA Lab-06, Question 4
   Sorting a permutation p of {1,...,n} using only reverse(p,i,j),
   which reverses the closed range p_i..p_j.

   Part A: prove/validate that O(n) reversals always suffice.
   Part B: when cost(reverse(p,i,j)) = j-i+1 (the length), design and
           validate an algorithm whose TOTAL cost is O(n log^2 n).

   Input representation: the permutation is stored as an array
   p[0..n-1] of distinct integers 1..n (0-indexed array standing in
   for the 1-indexed p of the problem statement).
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

static long long g_reversalCount = 0;
static long long g_totalCost = 0;

/* The one primitive operation we are allowed to use to mutate p.
   Reverses the CLOSED range p[i..j] (0-indexed). Cost = j-i+1.   */
void reverseOp(int p[], int i, int j) {
    g_reversalCount++;
    g_totalCost += (j - i + 1);
    while (i < j) {
        int t = p[i]; p[i] = p[j]; p[j] = t;
        i++; j--;
    }
}

/* ============================================================
   PART A: sort with O(n) reversals (no regard to cost).
   Selection-sort style: at step i, find where value (i+1) currently
   sits, at index j, then reverseOp(p, i, j) brings it to position i
   while leaving positions [0, i) untouched. After n-1 such steps the
   whole array is sorted -- this proves O(n) reversals always suffice
   for ANY permutation of size n (see q4_report.pdf for the proof).
   ============================================================ */
void sortWithLinearReversals(int p[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int target = i + 1;
        int j = i;
        while (p[j] != target) j++;
        if (j != i) reverseOp(p, i, j);
    }
}

/* ============================================================
   PART B: sort with total reversal-length cost O(n log^2 n).
   Standard merge sort, where "merging" two adjacent sorted runs
   p[lo,mid) and p[mid,hi) is implemented purely with reverseOp,
   using the classic 3-reversal ROTATE and a recursive, binary-
   search-driven in-place merge (see q4_report.pdf for the full
   cost analysis: O(s log s) per merge of total size s, and
   O(n log^2 n) summed over the O(log n) merge-sort levels).
   ============================================================ */

/* rotate(p, l, m, r): rotates the half-open range [l, r) so that
   the block [m, r) ends up before the block [l, m).
   Implemented with exactly three reverseOp calls; total elements
   touched = (m-l) + (r-m) + (r-l) = 2*(r-l), i.e. O(r-l).        */
void rotate(int p[], int l, int m, int r) {
    if (l == m || m == r) return;
    reverseOp(p, l, m - 1);
    reverseOp(p, m, r - 1);
    reverseOp(p, l, r - 1);
}

/* first index in [lo,hi) with p[idx] > val  (p[lo..hi) sorted ascending) */
int upperBound(int p[], int lo, int hi, int val) {
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (p[mid] <= val) lo = mid + 1; else hi = mid;
    }
    return lo;
}

/* first index in [lo,hi) with p[idx] >= val (p[lo..hi) sorted ascending) */
int lowerBound(int p[], int lo, int hi, int val) {
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (p[mid] < val) lo = mid + 1; else hi = mid;
    }
    return lo;
}

/* Recursive in-place merge of sorted runs p[lo,mid) and p[mid,hi),
   half-open indices, using only rotate()/reverseOp.               */
void mergeInPlace(int p[], int lo, int mid, int hi) {
    if (lo >= mid || mid >= hi) return;
    if (p[mid - 1] <= p[mid]) return;      /* already merged: 0 cost */
    if (hi - lo == 2) { reverseOp(p, lo, hi - 1); return; }

    int m1, m2;
    if (mid - lo <= hi - mid) {
        m2 = mid + (hi - mid) / 2;
        int pivot = p[m2];
        m1 = upperBound(p, lo, mid, pivot);
    } else {
        m1 = lo + (mid - lo) / 2;
        int pivot = p[m1];
        m2 = lowerBound(p, mid, hi, pivot);
    }

    rotate(p, m1, mid, m2);
    int newMid = m1 + (m2 - mid);
    mergeInPlace(p, lo, m1, newMid);
    mergeInPlace(p, newMid, m2, hi);
}

void mergeSortViaReversal(int p[], int lo, int hi) {  /* half-open [lo,hi) */
    if (hi - lo <= 1) return;
    int mid = lo + (hi - lo) / 2;
    mergeSortViaReversal(p, lo, mid);
    mergeSortViaReversal(p, mid, hi);
    mergeInPlace(p, lo, mid, hi);
}

/* ---------------------------- helpers ---------------------------- */
void printArray(const char *label, int p[], int n) {
    printf("%s: [", label);
    for (int i = 0; i < n; i++) printf("%d%s", p[i], (i == n - 1) ? "" : ", ");
    printf("]\n");
}

int isSorted(int p[], int n) {
    for (int i = 1; i < n; i++) if (p[i - 1] > p[i]) return 0;
    return 1;
}

void shufflePermutation(int p[], int n) {
    for (int i = 0; i < n; i++) p[i] = i + 1;
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int t = p[i]; p[i] = p[j]; p[j] = t;
    }
}

int main(void) {
    srand(42);

    /* ---- small illustrative example (matches the lab sheet) ---- */
    int example[] = {1, 4, 3, 2, 5};
    int nEx = sizeof(example) / sizeof(example[0]);
    printArray("Example permutation", example, nEx);
    g_reversalCount = 0; g_totalCost = 0;
    reverseOp(example, 1, 3); /* reverse the 2nd..4th elements: 4,3,2 -> 2,3,4 */
    printArray("After reverse(p,1,3) (0-indexed)", example, nEx);
    printf("Sorted with 1 reversal, cost = %lld\n\n", g_totalCost);

    /* ---- Part A demo: O(n) reversals, unweighted ---- */
    {
        int n = 12;
        int p[12];
        shufflePermutation(p, n);
        printArray("\nPart A input", p, n);
        g_reversalCount = 0; g_totalCost = 0;
        sortWithLinearReversals(p, n);
        printArray("Part A sorted ", p, n);
        printf("Part A: reversals used = %lld (<= n-1 = %d), sorted = %s\n",
               g_reversalCount, n - 1, isSorted(p, n) ? "yes" : "NO (bug)");
    }

    /* ---- Part B demo + empirical cost growth ---- */
    printf("\nPart B (cost-bounded, target O(n log^2 n)):\n");
    {
        int n = 16;
        int p[16];
        shufflePermutation(p, n);
        printArray("Part B input ", p, n);
        g_reversalCount = 0; g_totalCost = 0;
        mergeSortViaReversal(p, 0, n);
        printArray("Part B sorted", p, n);
        printf("n=%d: reversals used = %lld, total cost = %lld, sorted = %s\n",
               n, g_reversalCount, g_totalCost, isSorted(p, n) ? "yes" : "NO (bug)");
    }

    printf("\nEmpirical cost growth (average of 20 random permutations per n):\n");
    printf("%8s %14s %14s %16s\n", "n", "total cost", "n*log2(n)", "n*(log2 n)^2");
    for (int n = 16; n <= 4096; n *= 2) {
        long long sumCost = 0;
        int trials = 20;
        for (int t = 0; t < trials; t++) {
            int *p = malloc(n * sizeof(int));
            shufflePermutation(p, n);
            g_reversalCount = 0; g_totalCost = 0;
            mergeSortViaReversal(p, 0, n);
            if (!isSorted(p, n)) printf("  !! sort failed for n=%d\n", n);
            sumCost += g_totalCost;
            free(p);
        }
        double avgCost = (double) sumCost / trials;
        double log2n = 0.0; { int t = n; while (t > 1) { t >>= 1; log2n += 1.0; } }
        printf("%8d %14.1f %14.1f %16.1f\n", n, avgCost, n * log2n, n * log2n * log2n);
    }

    return 0;
}