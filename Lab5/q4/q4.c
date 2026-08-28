/*
 * heapsort.c
 * ----------
 * Generates N random integers, writes them to "input.txt",
 * sorts them using Heap Sort (in-place, ascending order),
 * and writes the sorted result to "output.txt".
 *
 * Usage:
 *   ./heapsort <N> [max_value] [seed]
 *
 *   N          - number of elements to generate (required)
 *   max_value  - upper bound (exclusive) for random values (default 100000)
 *   seed       - RNG seed for reproducibility (default: current time)
 *
 * Example:
 *   ./heapsort 20
 *   ./heapsort 1000000 500 42
 *
 * Files produced:
 *   input.txt  - the N randomly generated elements (unsorted)
 *   output.txt - the same elements after Heap Sort (ascending)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INPUT_FILE  "input.txt"
#define OUTPUT_FILE "output.txt"

/* ---------------------------------------------------------------
 * heapify
 * Restores the max-heap property for the subtree rooted at index i,
 * within an array of size n. Assumes the subtrees below i are
 * already valid max-heaps (standard "sift-down").
 * ------------------------------------------------------------- */
void heapify(int arr[], int n, int i) {
    while (1) {
        int largest = i;
        int left    = 2 * i + 1;
        int right   = 2 * i + 2;

        if (left < n && arr[left] > arr[largest])
            largest = left;

        if (right < n && arr[right] > arr[largest])
            largest = right;

        if (largest == i)
            break; /* heap property already satisfied */

        int tmp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = tmp;

        i = largest; /* continue sifting down */
    }
}

/* ---------------------------------------------------------------
 * heapSort
 * Sorts arr[0..n-1] in ascending order using Heap Sort.
 * Phase 1: build a max-heap out of the array (bottom-up).
 * Phase 2: repeatedly swap the root (max) with the last element
 *          of the unsorted region, shrink the heap, and re-heapify.
 * ------------------------------------------------------------- */
void heapSort(int arr[], int n) {
    /* Phase 1: Build max heap - O(n) */
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    /* Phase 2: Extract elements one by one - O(n log n) */
    for (int end = n - 1; end > 0; end--) {
        int tmp = arr[0];
        arr[0] = arr[end];
        arr[end] = tmp;

        heapify(arr, end, 0); /* heap size shrinks by one each time */
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

    /* Step 2: time and run heap sort */
    clock_t start = clock();
    heapSort(arr, (int)n);
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