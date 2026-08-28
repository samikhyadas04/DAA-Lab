/*
 * q3.c
 * ----
 * Generates N random integers, writes them to "input.txt",
 * sorts them using Quick Sort (in-place, ascending order),
 * and writes the sorted result to "output.txt".
 *
 * Usage:
 *   ./q3 <N> [max_value] [seed]
 *
 *   N          - number of elements to generate (required)
 *   max_value  - upper bound (exclusive) for random values (default 100000)
 *   seed       - RNG seed for reproducibility (default: current time)
 *
 * Example:
 *   ./q3 20
 *   ./q3 1000000 500 42
 *
 * Files produced:
 *   input.txt  - the N randomly generated elements (unsorted)
 *   output.txt - the same elements after Quick Sort (ascending)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INPUT_FILE  "input.txt"
#define OUTPUT_FILE "output.txt"

/* ---------------------------------------------------------------
 * swap
 * Exchanges two integers in place.
 * ------------------------------------------------------------- */
void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/* ---------------------------------------------------------------
 * medianOfThreePivot
 * Picks a pivot using median-of-three (first, middle, last) and
 * moves it to the end of the range, where the partition step
 * expects it. This avoids the classic O(n^2) worst case that a
 * fixed "always pick last element" pivot hits on already-sorted
 * or reverse-sorted input.
 * ------------------------------------------------------------- */
void medianOfThreePivot(int arr[], int low, int high) {
    int mid = low + (high - low) / 2;

    if (arr[mid] < arr[low])   swap(&arr[mid], &arr[low]);
    if (arr[high] < arr[low])  swap(&arr[high], &arr[low]);
    if (arr[high] < arr[mid])  swap(&arr[high], &arr[mid]);

    /* arr[mid] is now the median of the three; move it to arr[high]
       so the standard Lomuto partition below can use it as pivot */
    swap(&arr[mid], &arr[high]);
}

/* ---------------------------------------------------------------
 * partition
 * Lomuto partition scheme using arr[high] as the pivot (already
 * chosen via median-of-three). Rearranges arr[low..high] so that
 * everything <= pivot comes before it and everything > pivot comes
 * after it, then returns the pivot's final index.
 * ------------------------------------------------------------- */
int partition(int arr[], int low, int high) {
    medianOfThreePivot(arr, low, high);
    int pivot = arr[high];
    int i = low - 1; /* boundary of the "<= pivot" region */

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

/* ---------------------------------------------------------------
 * quickSort
 * Sorts arr[low..high] in ascending order using Quick Sort.
 * Recurses into the smaller partition first and loops on the
 * larger one (tail-call style) to keep worst-case recursion
 * depth at O(log n) instead of O(n).
 * ------------------------------------------------------------- */
void quickSort(int arr[], int low, int high) {
    while (low < high) {
        int pi = partition(arr, low, high);

        if (pi - low < high - pi) {
            quickSort(arr, low, pi - 1);
            low = pi + 1;       /* loop on the larger right side */
        } else {
            quickSort(arr, pi + 1, high);
            high = pi - 1;      /* loop on the larger left side */
        }
    }
}

/* ---------------------------------------------------------------
 * generateAndWriteInput
 * Generates n random integers in [0, maxVal) and writes them to
 * INPUT_FILE, one per line. Also returns them via the arr[] buffer.
 * ------------------------------------------------------------- */
void generateAndWriteInput(int arr[], int n, int maxVal) {
    FILE *fp = fopen(INPUT_FILE, "w");
    if (!fp) {
        perror("Error opening " INPUT_FILE);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        arr[i] = rand() % maxVal;
        fprintf(fp, "%d\n", arr[i]);
    }

    fclose(fp);
}

/* ---------------------------------------------------------------
 * writeOutput
 * Writes the sorted array to OUTPUT_FILE, one value per line.
 * ------------------------------------------------------------- */
void writeOutput(int arr[], int n) {
    FILE *fp = fopen(OUTPUT_FILE, "w");
    if (!fp) {
        perror("Error opening " OUTPUT_FILE);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++)
        fprintf(fp, "%d\n", arr[i]);

    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <N> [max_value] [seed]\n", argv[0]);
        return EXIT_FAILURE;
    }

    long n = atol(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "N must be a positive integer.\n");
        return EXIT_FAILURE;
    }

    int maxVal = (argc >= 3) ? atoi(argv[2]) : 100000;
    if (maxVal <= 0) maxVal = 100000;

    unsigned int seed = (argc >= 4) ? (unsigned int)atol(argv[3])
                                     : (unsigned int)time(NULL);
    srand(seed);

    int *arr = (int *)malloc((size_t)n * sizeof(int));
    if (!arr) {
        fprintf(stderr, "Memory allocation failed for N = %ld\n", n);
        return EXIT_FAILURE;
    }

    /* Step 1: generate N random elements and store them in input.txt */
    generateAndWriteInput(arr, (int)n, maxVal);
    printf("Generated %ld random elements (0-%d, seed=%u) -> %s\n",
           n, maxVal - 1, seed, INPUT_FILE);

    /* Step 2: time and run quick sort */
    clock_t start = clock();
    quickSort(arr, 0, (int)n - 1);
    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    /* Step 3: write sorted elements to output.txt */
    writeOutput(arr, (int)n);
    printf("Sorted %ld elements in %.6f seconds -> %s\n",
           n, elapsed, OUTPUT_FILE);

    /* Step 4: sanity check that the array is actually sorted */
    int sorted = 1;
    for (long i = 1; i < n; i++) {
        if (arr[i - 1] > arr[i]) { sorted = 0; break; }
    }
    printf("Sorted correctly: %s\n", sorted ? "YES" : "NO");

    free(arr);
    return EXIT_SUCCESS;
}