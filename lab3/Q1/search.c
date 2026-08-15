/*
 * Lab 03 - Question 1: Binary vs Ternary Search
 * DAA - Dr. Ajaya Kumar Dash
 *
 * Compares binary search (splits n elements into ~2 halves) against
 * ternary search (splits n elements into ~3 parts) on a sorted array,
 * counting the number of KEY COMPARISONS (comparisons between an array
 * element and the search key x) each algorithm performs.
 *
 * For every array size n in a chosen list, we run a search for EVERY
 * element in the array (guaranteed hits) plus a few misses, and record
 * the average and worst-case number of comparisons for both algorithms.
 * Results are written to benchmark.csv for plotting with Python.
 */

#include <stdio.h>
#include <stdlib.h>

/* ---------- Binary Search ---------- */
/* Returns index of x in arr[0..n-1], or -1 if not found.
   *comparisons is incremented by the number of comparisons made
   between an array element and x. */
int binarySearch(int *arr, int n, int x, long long *comparisons) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;

        (*comparisons)++;               /* arr[mid] == x ? */
        if (arr[mid] == x) return mid;

        (*comparisons)++;               /* arr[mid] < x ? */
        if (arr[mid] < x) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

/* ---------- Ternary Search ---------- */
int ternarySearch(int *arr, int n, int x, long long *comparisons) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int third = (hi - lo) / 3;
        int mid1 = lo + third;
        int mid2 = hi - third;

        (*comparisons)++;               /* arr[mid1] == x ? */
        if (arr[mid1] == x) return mid1;

        (*comparisons)++;               /* arr[mid2] == x ? */
        if (arr[mid2] == x) return mid2;

        (*comparisons)++;               /* x < arr[mid1] ? */
        if (x < arr[mid1]) {
            hi = mid1 - 1;
        } else {
            (*comparisons)++;           /* x > arr[mid2] ? */
            if (x > arr[mid2]) {
                lo = mid2 + 1;
            } else {
                lo = mid1 + 1;
                hi = mid2 - 1;
            }
        }
    }
    return -1;
}

int main(void) {
    int sizes[] = {100, 500, 1000, 5000, 10000, 50000, 100000,
                    500000, 1000000, 5000000};
    int numSizes = sizeof(sizes) / sizeof(sizes[0]);

    FILE *fp = fopen("benchmark.csv", "w");
    if (!fp) { perror("fopen"); return 1; }
    fprintf(fp, "n,avg_binary,avg_ternary,worst_binary,worst_ternary,log2n,log3n\n");

    for (int s = 0; s < numSizes; s++) {
        int n = sizes[s];
        int *arr = malloc(sizeof(int) * n);
        for (int i = 0; i < n; i++) arr[i] = i * 2;   /* sorted, unique */

        long long totalBinary = 0, totalTernary = 0;
        long long worstBinary = 0, worstTernary = 0;

        /* Search for every element present (guaranteed hits) */
        for (int i = 0; i < n; i++) {
            long long cBin = 0, cTer = 0;
            binarySearch(arr, n, arr[i], &cBin);
            ternarySearch(arr, n, arr[i], &cTer);
            totalBinary += cBin;
            totalTernary += cTer;
            if (cBin > worstBinary) worstBinary = cBin;
            if (cTer > worstTernary) worstTernary = cTer;
        }
        /* A handful of guaranteed misses (odd numbers aren't in arr[]) */
        int missTrials = (n > 1000) ? 1000 : n;
        for (int i = 0; i < missTrials; i++) {
            long long cBin = 0, cTer = 0;
            int missVal = (i * 2) + 1; /* odd -> never in arr */
            binarySearch(arr, n, missVal, &cBin);
            ternarySearch(arr, n, missVal, &cTer);
            if (cBin > worstBinary) worstBinary = cBin;
            if (cTer > worstTernary) worstTernary = cTer;
        }

        double avgBinary = (double) totalBinary / n;
        double avgTernary = (double) totalTernary / n;
        double log2n = 0.0, log3n = 0.0;
        {
            double val = n;
            double l2 = 0, l3 = 0, x2 = val, x3 = val;
            while (x2 > 1) { x2 /= 2.0; l2 += 1.0; }
            while (x3 > 1) { x3 /= 3.0; l3 += 1.0; }
            log2n = l2; log3n = l3;
        }

        printf("n=%8d  avgBinary=%.3f  avgTernary=%.3f  worstBinary=%lld  worstTernary=%lld\n",
               n, avgBinary, avgTernary, worstBinary, worstTernary);

        fprintf(fp, "%d,%.4f,%.4f,%lld,%lld,%.4f,%.4f\n",
                n, avgBinary, avgTernary, worstBinary, worstTernary, log2n, log3n);

        free(arr);
    }

    fclose(fp);
    printf("\nDone. Results written to benchmark.csv\n");
    printf("Observation: ternary search needs UP TO 4 comparisons per level over\n");
    printf("log3(n) levels (~4*log3 n ~= 2.52*log2 n comparisons), while binary\n");
    printf("search needs up to 2 comparisons per level over log2(n) levels\n");
    printf("(~2*log2 n comparisons). Hence binary search performs fewer total\n");
    printf("key comparisons than ternary search, despite ternary having fewer levels.\n");
    return 0;
}
