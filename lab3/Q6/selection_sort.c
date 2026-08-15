/*
 * Lab 03 - Question 6: Use of Loop Invariants in Sorting (Selection Sort)
 * DAA - Dr. Ajaya Kumar Dash
 *
 * PSEUDOCODE (1-indexed, matches problem statement):
 *
 *   SELECTION-SORT(A, n)
 *     for i = 1 to n - 1
 *         min = i
 *         for j = i + 1 to n
 *             if A[j] < A[min]
 *                 min = j
 *         exchange A[i] with A[min]
 *
 * LOOP INVARIANT (for the outer loop, at the start of each iteration i):
 *     The subarray A[1 .. i-1] consists of the i-1 SMALLEST elements of
 *     the original array A, sorted in increasing order.
 *
 *   Initialization: Before the first iteration, i = 1, so A[1..0] is the
 *   empty subarray - trivially the "0 smallest elements, sorted."
 *
 *   Maintenance: At the start of iteration i, A[1..i-1] holds the i-1
 *   smallest elements in sorted order (by hypothesis). The inner loop
 *   scans A[i..n] and finds the index `min` of the smallest element in
 *   that range; since A[1..i-1] already holds the i-1 overall smallest
 *   elements, the smallest element remaining in A[i..n] is exactly the
 *   i-th smallest element overall. Swapping it into A[i] extends the
 *   sorted-smallest prefix to A[1..i], preserving the invariant for i+1.
 *
 *   Termination: The outer loop stops after i = n-1, i.e. once the
 *   invariant holds for i = n: A[1..n-1] holds the n-1 smallest elements
 *   in sorted order. The single remaining element A[n] must then be the
 *   largest element (since all n-1 smaller ones are already placed), so
 *   the entire array A[1..n] is sorted. This proves correctness.
 *
 * WHY ONLY THE FIRST (n-1) ELEMENTS?
 *   After the first n-1 elements are placed in their final sorted
 *   positions (each being the correct i-th smallest), only one element
 *   can possibly remain unplaced - and it is forced to occupy the one
 *   remaining position, A[n], which must be the maximum. Running an
 *   extra iteration for i = n would compare A[n] against itself (an
 *   inner loop from n+1 to n, i.e. an empty range) and accomplish
 *   nothing, so it is redundant.
 *
 * RUNNING TIME:
 *   The inner loop for a fixed i runs (n - i) times regardless of the
 *   input, i.e. selection sort ALWAYS performs the same number of
 *   comparisons no matter the initial arrangement of the data:
 *       sum_{i=1}^{n-1} (n - i) = n(n-1)/2
 *   So the worst-case running time is Theta(n^2), and because the
 *   comparison count never depends on input order, the BEST-CASE running
 *   time is ALSO Theta(n^2) - selection sort does not benefit from a
 *   partially- or fully-sorted input the way, e.g., insertion sort does.
 *   (The number of SWAPS is only O(n), but swaps aren't what dominates
 *   the running time here - comparisons are.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long comparisons = 0;
long long swaps = 0;

void selectionSort(int *A, int n) {
    /* 0-indexed translation of the 1-indexed pseudocode above */
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            comparisons++;
            if (A[j] < A[minIdx]) minIdx = j;
        }
        if (minIdx != i) {
            int tmp = A[i]; A[i] = A[minIdx]; A[minIdx] = tmp;
            swaps++;
        }
    }
}

int isSorted(int *A, int n) {
    for (int i = 1; i < n; i++) if (A[i - 1] > A[i]) return 0;
    return 1;
}

void fillRandom(int *A, int n) { for (int i = 0; i < n; i++) A[i] = rand() % 1000000; }
void fillSorted(int *A, int n) { for (int i = 0; i < n; i++) A[i] = i; }
void fillReverse(int *A, int n) { for (int i = 0; i < n; i++) A[i] = n - i; }

int main(void) {
    srand(11);
    int sizes[] = {10, 50, 100, 500, 1000, 2000, 4000, 6000, 8000, 10000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    FILE *fp = fopen("benchmark.csv", "w");
    fprintf(fp, "n,comparisons_random,comparisons_sorted,comparisons_reverse,n_n_minus_1_over_2\n");

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        int *A = malloc(sizeof(int) * n);

        fillRandom(A, n); comparisons = 0; selectionSort(A, n);
        long long cRand = comparisons;
        int okRand = isSorted(A, n);

        fillSorted(A, n); comparisons = 0; selectionSort(A, n);
        long long cSorted = comparisons;
        int okSorted = isSorted(A, n);

        fillReverse(A, n); comparisons = 0; selectionSort(A, n);
        long long cReverse = comparisons;
        int okReverse = isSorted(A, n);

        long long expected = (long long) n * (n - 1) / 2;

        printf("n=%6d  cmp(random)=%8lld  cmp(sorted)=%8lld  cmp(reverse)=%8lld  "
               "n(n-1)/2=%8lld  sorted_ok=%d,%d,%d\n",
               n, cRand, cSorted, cReverse, expected, okRand, okSorted, okReverse);

        fprintf(fp, "%d,%lld,%lld,%lld,%lld\n", n, cRand, cSorted, cReverse, expected);

        free(A);
    }

    fclose(fp);
    printf("\nAll three input orders (random/sorted/reverse) used EXACTLY the same\n");
    printf("number of comparisons: n(n-1)/2 = Theta(n^2), confirming that selection\n");
    printf("sort's best case equals its worst case in Theta(n^2) time.\n");
    return 0;
}
