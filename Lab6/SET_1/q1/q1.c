/* ============================================================
   DAA Lab-06, Question 1
   1D array operations and their worst-case complexities.

   Input representation: an array of n integers stored contiguously
   (int arr[n]) so that random access is O(1) and each procedure
   below can be validated with a single linear/O(n log n) pass as
   analysed in q1_report.pdf.
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <string.h>

/* ---------- (i) Maximum element --------------------------------- O(n) */
int findMax(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++)
        if (arr[i] > max) max = arr[i];
    return max;
}

/* ---------- (ii) First and second largest ------------------------ O(n) */
void firstSecondLargest(int arr[], int n, int *first, int *second) {
    *first = *second = INT_MIN;
    for (int i = 0; i < n; i++) {
        if (arr[i] > *first) {
            *second = *first;
            *first = arr[i];
        } else if (arr[i] > *second && arr[i] != *first) {
            *second = arr[i];
        }
    }
}

/* ---------- (iii) Mean -------------------------------------------- O(n) */
double findMean(int arr[], int n) {
    long long sum = 0;
    for (int i = 0; i < n; i++) sum += arr[i];
    return (double) sum / n;
}

/* ---------- comparator used by qsort (for median/mode/duplicates) ---- */
int cmp(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}

/* ---------- (iv) Median -------------------------- O(n log n) --------
   Sorting-based implementation. NOTE: an O(n) worst-case algorithm
   exists using the median-of-medians (BFPRT) selection algorithm;
   see q1_report.pdf, section (iv). Sorting is used here for clarity
   and because it is reused by mode/duplicate-removal below.        */
double findMedian(int arr[], int n) {
    int *tmp = malloc(n * sizeof(int));
    memcpy(tmp, arr, n * sizeof(int));
    qsort(tmp, n, sizeof(int), cmp);
    double median;
    if (n % 2 == 0)
        median = (tmp[n / 2 - 1] + tmp[n / 2]) / 2.0;
    else
        median = tmp[n / 2];
    free(tmp);
    return median;
}

/* ---------- (v) Standard deviation ---------------------------------- O(n) */
double findStdDev(int arr[], int n, double mean) {
    double sumSq = 0.0;
    for (int i = 0; i < n; i++)
        sumSq += (arr[i] - mean) * (arr[i] - mean);
    return sqrt(sumSq / n);
}

/* ---------- (vi) Mode ------------------------------ O(n log n) --------
   Sort, then scan for the longest run of equal values.
   (A hash table gives O(n) expected time; see report.)             */
int findMode(int arr[], int n) {
    int *tmp = malloc(n * sizeof(int));
    memcpy(tmp, arr, n * sizeof(int));
    qsort(tmp, n, sizeof(int), cmp);

    int mode = tmp[0], bestCount = 1, curCount = 1;
    for (int i = 1; i < n; i++) {
        if (tmp[i] == tmp[i - 1]) curCount++;
        else curCount = 1;
        if (curCount > bestCount) {
            bestCount = curCount;
            mode = tmp[i];
        }
    }
    free(tmp);
    return mode;
}

/* ---------- (vii) Remove all duplicates ------------ O(n log n) --------
   Sort, then compact equal runs into a single copy in place.
   Returns the new logical length.                                    */
int removeDuplicates(int arr[], int n) {
    if (n == 0) return 0;
    qsort(arr, n, sizeof(int), cmp);
    int j = 0;
    for (int i = 1; i < n; i++) {
        if (arr[i] != arr[j]) {
            j++;
            arr[j] = arr[i];
        }
    }
    return j + 1;
}

/* ---------- (viii) Reverse the array -------------------------------- O(n) */
void reverseArray(int arr[], int n) {
    int i = 0, j = n - 1;
    while (i < j) {
        int t = arr[i]; arr[i] = arr[j]; arr[j] = t;
        i++; j--;
    }
}

/* ---------- (ix) Partition w.r.t. a random pivot -------------------- O(n)
   Standard Lomuto partition puts elements < pivot BEFORE elements
   >= pivot. This variant reverses that: all elements LESS than the
   pivot end up AFTER all elements >= pivot, in a single left-to-right
   pass, O(1) extra space.                                            */
int partitionGE_first(int arr[], int n, int pivotIndex) {
    int pivot = arr[pivotIndex];
    /* move pivot to the end temporarily */
    int t = arr[pivotIndex]; arr[pivotIndex] = arr[n - 1]; arr[n - 1] = t;

    int store = 0; /* boundary: everything before `store` is >= pivot */
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] >= pivot) {
            t = arr[i]; arr[i] = arr[store]; arr[store] = t;
            store++;
        }
    }
    /* place pivot itself at the boundary (pivot is >= pivot) */
    t = arr[store]; arr[store] = arr[n - 1]; arr[n - 1] = t;
    return store; /* final index of the pivot */
}

void printArray(const char *label, int arr[], int n) {
    printf("%s: [", label);
    for (int i = 0; i < n; i++) printf("%d%s", arr[i], (i == n - 1) ? "" : ", ");
    printf("]\n");
}

int main(void) {
    srand((unsigned) time(NULL));

    int arr[] = {12, 4, 7, 4, 9, 12, 1, 7, 4, 15, 9, 4};
    int n = sizeof(arr) / sizeof(arr[0]);

    printArray("Input array", arr, n);

    printf("(i)   Maximum element        = %d\n", findMax(arr, n));

    int first, second;
    firstSecondLargest(arr, n, &first, &second);
    printf("(ii)  First largest          = %d, Second largest = %d\n", first, second);

    double mean = findMean(arr, n);
    printf("(iii) Mean                   = %.4f\n", mean);

    printf("(iv)  Median                 = %.4f\n", findMedian(arr, n));

    printf("(v)   Standard deviation     = %.4f\n", findStdDev(arr, n, mean));

    printf("(vi)  Mode                   = %d\n", findMode(arr, n));

    {
        int copy[sizeof(arr) / sizeof(arr[0])];
        memcpy(copy, arr, sizeof(arr));
        int newLen = removeDuplicates(copy, n);
        printf("(vii) After removing duplicates (sorted): ");
        printArray("     ", copy, newLen);
    }

    {
        int copy[sizeof(arr) / sizeof(arr[0])];
        memcpy(copy, arr, sizeof(arr));
        reverseArray(copy, n);
        printArray("(viii) Reversed array", copy, n);
    }

    {
        int copy[sizeof(arr) / sizeof(arr[0])];
        memcpy(copy, arr, sizeof(arr));
        int pivotIdx = rand() % n;
        int pivotVal = copy[pivotIdx];
        int finalPos = partitionGE_first(copy, n, pivotIdx);
        printf("(ix)  Random pivot value = %d (chosen at original index %d)\n", pivotVal, pivotIdx);
        printArray("      Partitioned array (>=pivot first, <pivot after)", copy, n);
        printf("      Pivot's final index = %d\n", finalPos);
    }

    return 0;
}