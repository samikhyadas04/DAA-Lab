

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ---------- Quickselect core ---------- */

static void swap(double *a, double *b) {
    double tmp = *a;
    *a = *b;
    *b = tmp;
}

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

static double quickselect(double *arr, int low, int high, int k) {
    while (1) {
        if (low == high) {
            return arr[low];
        }

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

static double max_of(const double *arr, int len) {
    double m = arr[0];
    for (int i = 1; i < len; i++) {
        if (arr[i] > m) m = arr[i];
    }
    return m;
}

double find_median(const double *numbers, int n) {
    if (n <= 0) {
        fprintf(stderr, "Cannot compute median of an empty array\n");
        exit(EXIT_FAILURE);
    }

    double *arr = malloc((size_t)n * sizeof(double));
    if (!arr) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) arr[i] = numbers[i];

    int mid = n / 2;
    double result;

    if (n % 2 == 1) {
        /* Odd length: median is the single middle element */
        result = quickselect(arr, 0, n - 1, mid);
    } else {
        
        double upper = quickselect(arr, 0, n - 1, mid);
        double lower = max_of(arr, mid);
        result = (lower + upper) / 2.0;
    }

    free(arr);
    return result;
}

/* ---------- Correctness helper: median via full sort (for testing) ---------- */

static int cmp_double(const void *a, const void *b) {
    double da = *(const double *)a, db = *(const double *)b;
    return (da > db) - (da < db);
}

static double median_via_sort(const double *numbers, int n) {
    double *arr = malloc((size_t)n * sizeof(double));
    for (int i = 0; i < n; i++) arr[i] = numbers[i];
    qsort(arr, (size_t)n, sizeof(double), cmp_double);

    double result;
    int mid = n / 2;
    if (n % 2 == 1) {
        result = arr[mid];
    } else {
        result = (arr[mid - 1] + arr[mid]) / 2.0;
    }
    free(arr);
    return result;
}

/* ---------- Tests + benchmark ---------- */

static void run_test(const char *label, double *data, int n) {
    double expected = median_via_sort(data, n);
    double got = find_median(data, n);
    const char *status = (got == expected) ? "OK" : "FAIL";
    printf("%-4s: %-22s n=%-6d got=%-10g expected=%-10g\n",
           status, label, n, got, expected);
}

int main(void) {
    srand((unsigned int)time(NULL));

    /* Fixed small cases */
    double t1[] = {5};
    run_test("single element", t1, 1);

    double t2[] = {3, 1};
    run_test("two elements", t2, 2);

    double t3[] = {7, 1, 3};
    run_test("odd length", t3, 3);

    double t4[] = {9, 5, 1, 3};
    run_test("even length", t4, 4);

    double t5[] = {12, 3, 5, 7, 4, 19, 26};
    run_test("odd length 2", t5, 7);

    double t6[] = {1, 2, 2, 3, 4};
    run_test("duplicates", t6, 5);

    double t7[] = {-5, -1, -3, 0, 8, 2};
    run_test("negatives", t7, 6);

    double t8[] = {5, 5, 5, 5, 5};
    run_test("all same", t8, 5);

    /* Reverse sorted, size 100 */
    {
        int n = 100;
        double *data = malloc((size_t)n * sizeof(double));
        for (int i = 0; i < n; i++) data[i] = (double)(n - i);
        run_test("reverse sorted", data, n);
        free(data);
    }

    /* Random, odd and even sizes */
    {
        int n = 1001;
        double *data = malloc((size_t)n * sizeof(double));
        for (int i = 0; i < n; i++) data[i] = (double)(rand() % 2001 - 1000);
        run_test("random odd", data, n);
        free(data);
    }
    {
        int n = 1000;
        double *data = malloc((size_t)n * sizeof(double));
        for (int i = 0; i < n; i++) data[i] = (double)(rand() % 2001 - 1000);
        run_test("random even", data, n);
        free(data);
    }

    {
        int n = 2000000;
        double *data = malloc((size_t)n * sizeof(double));
        for (int i = 0; i < n; i++) data[i] = (double)rand() / RAND_MAX;

        clock_t start = clock();
        find_median(data, n);
        clock_t end = clock();
        printf("\nquickselect median on %d elems: %.3fs\n",
               n, (double)(end - start) / CLOCKS_PER_SEC);

        start = clock();
        median_via_sort(data, n);
        end = clock();
        printf("sort-based median on %d elems:  %.3fs\n",
               n, (double)(end - start) / CLOCKS_PER_SEC);

        free(data);
    }

    return 0;
}