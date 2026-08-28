

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ---------- Quickselect core ---------- */

static void swap(double *a, double *b) {
    double tmp = *a;
    *a = *b;
    *b = tmp;
}

/*
 * Lomuto partition of arr[low..high] around arr[pivot_index].
 * Returns the pivot's final sorted position.
 */
static int partition(double *arr, int low, int high, int pivot_index) {
    double pivot_value = arr[pivot_index];
    swap(&arr[pivot_index], &arr[high]);  /* move pivot to the end */

    int store_index = low;
    for (int i = low; i < high; i++) {
        if (arr[i] < pivot_value) {
            swap(&arr[store_index], &arr[i]);
            store_index++;
        }
    }
    swap(&arr[store_index], &arr[high]);  /* move pivot to its final place */
    return store_index;
}

/*
 * Returns the element at 0-indexed sorted position `k` of
 * arr[low..high], in expected O(N) time. Mutates arr in place.
 */
static double quickselect(double *arr, int low, int high, int k) {
    while (1) {
        if (low == high) {
            return arr[low];
        }

        /* Random pivot -> expected O(N), avoids worst case O(N^2)
         * on sorted / adversarial inputs. */
        int pivot_index = low + rand() % (high - low + 1);
        pivot_index = partition(arr, low, high, pivot_index);

        if (k == pivot_index) {
            return arr[k];
        } else if (k < pivot_index) {
            high = pivot_index - 1;
        } else {
            low = pivot_index + 1;
        }
    }
}

/*
 * K-th smallest element of numbers[0..n-1], K is 1-indexed
 * (K=1 -> minimum, K=N -> maximum). Copies the input internally so
 * the caller's array is left untouched.
 *
 * Time:  expected O(N), independent of K
 * Space: O(N) for the working copy (O(1) extra if the caller lets
 *        you mutate their array directly instead of copying it)
 */
double kth_smallest(const double *numbers, int n, int k) {
    if (n <= 0) {
        fprintf(stderr, "Cannot select from an empty array\n");
        exit(EXIT_FAILURE);
    }
    if (k < 1 || k > n) {
        fprintf(stderr, "k=%d out of range for n=%d (must be 1..n)\n", k, n);
        exit(EXIT_FAILURE);
    }

    double *arr = malloc((size_t)n * sizeof(double));
    if (!arr) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) arr[i] = numbers[i];

    double result = quickselect(arr, 0, n - 1, k - 1);  /* k -> 0-indexed */

    free(arr);
    return result;
}

/* ---------- Correctness helper: k-th smallest via full sort (for testing) ---------- */

static int cmp_double(const void *a, const void *b) {
    double da = *(const double *)a, db = *(const double *)b;
    return (da > db) - (da < db);
}

static double kth_smallest_via_sort(const double *numbers, int n, int k) {
    double *arr = malloc((size_t)n * sizeof(double));
    for (int i = 0; i < n; i++) arr[i] = numbers[i];
    qsort(arr, (size_t)n, sizeof(double), cmp_double);
    double result = arr[k - 1];
    free(arr);
    return result;
}

/* ---------- Tests + benchmark ---------- */

static void run_test(const char *label, double *data, int n, int k) {
    double expected = kth_smallest_via_sort(data, n, k);
    double got = kth_smallest(data, n, k);
    const char *status = (got == expected) ? "OK" : "FAIL";
    printf("%-4s: %-22s n=%-6d k=%-4d got=%-10g expected=%-10g\n",
           status, label, n, k, got, expected);
}

int main(void) {
    srand((unsigned int)time(NULL));

    double t1[] = {5};
    run_test("single element", t1, 1, 1);

    double t2[] = {9, 2};
    run_test("k = min", t2, 2, 1);
    run_test("k = max", t2, 2, 2);

    double t3[] = {7, 1, 3, 9, 5};
    for (int k = 1; k <= 5; k++) {
        run_test("all ranks", t3, 5, k);
    }

    double t4[] = {1, 2, 2, 3, 4};
    run_test("duplicates", t4, 5, 3);

    double t5[] = {-5, -1, -3, 0, 8, 2};
    run_test("negatives, k=2", t5, 6, 2);

    /* Reverse sorted, size 200 */
    {
        int n = 200;
        double *data = malloc((size_t)n * sizeof(double));
        for (int i = 0; i < n; i++) data[i] = (double)(n - i);
        run_test("reverse sorted, k=50", data, n, 50);
        free(data);
    }

    /* Random array, several k values */
    {
        int n = 1000;
        double *data = malloc((size_t)n * sizeof(double));
        for (int i = 0; i < n; i++) data[i] = (double)(rand() % 2001 - 1000);
        run_test("random, k=1", data, n, 1);
        run_test("random, k=500", data, n, 500);
        run_test("random, k=1000", data, n, 1000);
        free(data);
    }

    /* Timing comparison on a large array: find k=N/4 smallest */
    {
        int n = 2000000;
        int k = n / 4;
        double *data = malloc((size_t)n * sizeof(double));
        for (int i = 0; i < n; i++) data[i] = (double)rand() / RAND_MAX;

        clock_t start = clock();
        kth_smallest(data, n, k);
        clock_t end = clock();
        printf("\nquickselect k=N/4 on %d elems: %.3fs\n",
               n, (double)(end - start) / CLOCKS_PER_SEC);

        start = clock();
        kth_smallest_via_sort(data, n, k);
        end = clock();
        printf("sort-based k=N/4 on %d elems:  %.3fs\n",
               n, (double)(end - start) / CLOCKS_PER_SEC);

        free(data);
    }

    return 0;
}