/*
 * Lab 03 - Question 3: Max and Min using Divide and Conquer
 * DAA - Dr. Ajaya Kumar Dash
 *
 * Classic D&C simultaneous max/min algorithm.
 *
 *   MaxMin(A, lo, hi):
 *     if hi - lo == 0:               // 1 element
 *         return (A[lo], A[lo])
 *     if hi - lo == 1:               // 2 elements -> 1 comparison
 *         if A[lo] < A[hi]: return (A[lo], A[hi])
 *         else:             return (A[hi], A[lo])
 *     mid = (lo + hi) / 2
 *     (lmax, lmin) = MaxMin(A, lo, mid)
 *     (rmax, rmin) = MaxMin(A, mid+1, hi)
 *     max = max(lmax, rmax)          // 1 comparison
 *     min = min(lmin, rmin)          // 1 comparison
 *     return (max, min)
 *
 * Recurrence for number of comparisons T(n):
 *     T(2) = 1
 *     T(n) = 2*T(n/2) + 2
 *
 * Solving: T(n) = 3n/2 - 2  for n a power of 2, i.e. Theta(n), and the
 * number of comparisons never exceeds ceil(3n/2) - 2, which is at most
 * 3n/2. This is fewer than the 2n - 2 comparisons a naive linear scan
 * (tracking max and min separately) requires.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long comparisons = 0;

void maxMin(int *a, int lo, int hi, int *mx, int *mn) {
    if (lo == hi) {
        *mx = *mn = a[lo];
        return;
    }
    if (hi - lo == 1) {
        comparisons++;
        if (a[lo] < a[hi]) { *mn = a[lo]; *mx = a[hi]; }
        else               { *mn = a[hi]; *mx = a[lo]; }
        return;
    }
    int mid = (lo + hi) / 2;
    int lmax, lmin, rmax, rmin;
    maxMin(a, lo, mid, &lmax, &lmin);
    maxMin(a, mid + 1, hi, &rmax, &rmin);

    comparisons++;
    *mx = (lmax > rmax) ? lmax : rmax;
    comparisons++;
    *mn = (lmin < rmin) ? lmin : rmin;
}

/* Naive linear scan for comparison (2n-2 comparisons) */
void linearMaxMin(int *a, int n, int *mx, int *mn, long long *cmp) {
    *mx = *mn = a[0];
    *cmp = 0;
    for (int i = 1; i < n; i++) {
        (*cmp)++;
        if (a[i] > *mx) *mx = a[i];
        (*cmp)++;
        if (a[i] < *mn) *mn = a[i];
    }
}

int main(void) {
    srand(1);
    int sizes[] = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048,
                   4096, 8192, 16384, 32768, 65536, 131072, 262144,
                   524288, 1048576};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    FILE *fp = fopen("benchmark.csv", "w");
    fprintf(fp, "n,dc_comparisons,linear_comparisons,bound_3n_2\n");

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        int *a = malloc(sizeof(int) * n);
        for (int i = 0; i < n; i++) a[i] = rand();

        comparisons = 0;
        int mx, mn;
        maxMin(a, 0, n - 1, &mx, &mn);

        long long linCmp;
        int mx2, mn2;
        linearMaxMin(a, n, &mx2, &mn2, &linCmp);

        if (mx != mx2 || mn != mn2) {
            printf("MISMATCH at n=%d! DC(max=%d,min=%d) vs Linear(max=%d,min=%d)\n",
                   n, mx, mn, mx2, mn2);
        }

        double bound = 1.5 * n;
        printf("n=%8d  D&C comparisons=%7lld  linear=%7lld  3n/2 bound=%.1f  %s\n",
               n, comparisons, linCmp, bound,
               (comparisons <= (long long)bound) ? "OK (within bound)" : "EXCEEDS BOUND!");

        fprintf(fp, "%d,%lld,%lld,%.1f\n", n, comparisons, linCmp, bound);
        free(a);
    }

    fclose(fp);
    printf("\nD&C max/min consistently uses ~1.5n comparisons, beating the\n");
    printf("naive linear scan's ~2n comparisons, and results match exactly.\n");
    return 0;
}
