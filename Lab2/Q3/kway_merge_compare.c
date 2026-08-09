/*
 * kway_merge_compare.c
 * ---------------------------------------------------------
 * Given k sorted arrays, each with n elements, combine them
 * into one sorted array of kn elements using two methods:
 *
 *   Method 1 (sequential merging):
 *       result = arr[0]
 *       for i = 1 .. k-1:  result = merge(result, arr[i])
 *
 *   Method 2 (pairwise / tournament merging):
 *       repeatedly merge disjoint pairs of arrays until
 *       only one array remains (like the "combine" phase of
 *       merge sort, but starting from k already-sorted runs).
 *
 * Both methods are validated for correctness (result must be
 * sorted and contain all k*n elements) and benchmarked:
 *   - number of key comparisons performed
 *   - wall-clock time
 *
 * Two experiments are run and written to CSV files:
 *   (A) vary k, fixed n      -> results_vary_k.csv
 *   (B) vary n, fixed k      -> results_vary_n.csv
 *
 * Build:
 *   gcc -O2 -o kway_merge_compare kway_merge_compare.c
 * Run:
 *   ./kway_merge_compare
 * ---------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Generic merge of two sorted int arrays a[0..na-1], b[0..nb-1] into out.
   out must have room for na+nb ints. Counts comparisons. */
static void merge_two(const int *a, int na, const int *b, int nb,
                       int *out, long long *cmp) {
    int i = 0, j = 0, k = 0;
    while (i < na && j < nb) {
        (*cmp)++;
        if (a[i] <= b[j]) out[k++] = a[i++];
        else out[k++] = b[j++];
    }
    while (i < na) out[k++] = a[i++];
    while (j < nb) out[k++] = b[j++];
}

/* ---------------- Method 1: sequential merging ---------------- */
/* arrays: k pointers to sorted arrays, each of length n.
   Returns a freshly malloc'd sorted array of length k*n. */
static int *method1_sequential(int **arrays, int k, int n, long long *cmp) {
    int cur_len = n;
    int *cur = (int *)malloc(cur_len * sizeof(int));
    for (int i = 0; i < n; i++) cur[i] = arrays[0][i];

    for (int t = 1; t < k; t++) {
        int new_len = cur_len + n;
        int *merged = (int *)malloc(new_len * sizeof(int));
        merge_two(cur, cur_len, arrays[t], n, merged, cmp);
        free(cur);
        cur = merged;
        cur_len = new_len;
    }
    return cur;
}

/* ---------------- Method 2: pairwise / tournament merging ---------------- */
/* Repeatedly merge disjoint pairs of runs until one remains. */
static int *method2_pairwise(int **arrays, int k, int n, long long *cmp) {
    /* working list of (pointer, length) pairs; start with k runs of length n */
    int count = k;
    int **runs = (int **)malloc(count * sizeof(int *));
    int *lens = (int *)malloc(count * sizeof(int));
    for (int i = 0; i < k; i++) {
        runs[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) runs[i][j] = arrays[i][j];
        lens[i] = n;
    }

    while (count > 1) {
        int new_count = (count + 1) / 2; /* ceil */
        int **new_runs = (int **)malloc(new_count * sizeof(int *));
        int *new_lens = (int *)malloc(new_count * sizeof(int));

        int idx = 0;
        int i = 0;
        for (; i + 1 < count; i += 2) {
            int len = lens[i] + lens[i + 1];
            int *merged = (int *)malloc(len * sizeof(int));
            merge_two(runs[i], lens[i], runs[i + 1], lens[i + 1], merged, cmp);
            new_runs[idx] = merged;
            new_lens[idx] = len;
            idx++;
            free(runs[i]);
            free(runs[i + 1]);
        }
        if (i < count) { /* odd one out carried over unmerged this round */
            new_runs[idx] = runs[i];
            new_lens[idx] = lens[i];
            idx++;
        }

        free(runs);
        free(lens);
        runs = new_runs;
        lens = new_lens;
        count = new_count;
    }

    int *result = runs[0];
    free(runs);
    free(lens);
    return result;
}

/* ---------------- helpers ---------------- */

static int cmp_int(const void *a, const void *b) {
    return (*(const int *)a) - (*(const int *)b);
}

/* Generate k sorted arrays of n random ints each. */
static int **make_sorted_arrays(int k, int n, unsigned seed) {
    int **arrays = (int **)malloc(k * sizeof(int *));
    srand(seed);
    for (int i = 0; i < k; i++) {
        arrays[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) arrays[i][j] = rand();
        qsort(arrays[i], n, sizeof(int), cmp_int);
    }
    return arrays;
}

static void free_arrays(int **arrays, int k) {
    for (int i = 0; i < k; i++) free(arrays[i]);
    free(arrays);
}

static int check_sorted_and_complete(int *result, int total_len, int **arrays, int k, int n) {
    for (int i = 1; i < total_len; i++)
        if (result[i - 1] > result[i]) return 0;
    /* check multiset equality via sum as a cheap sanity check, plus count */
    long long sum_result = 0, sum_input = 0;
    for (int i = 0; i < total_len; i++) sum_result += result[i];
    for (int i = 0; i < k; i++)
        for (int j = 0; j < n; j++) sum_input += arrays[i][j];
    return sum_result == sum_input;
}

/* ---------------- experiments ---------------- */

static void run_experiment_vary_k(void) {
    int n = 500; /* fixed elements per array */
    int ks[] = {2, 4, 8, 16, 32, 64, 128, 256};
    int num_k = sizeof(ks) / sizeof(ks[0]);
    int trials = 5;

    FILE *fp = fopen("results_vary_k.csv", "w");
    fprintf(fp, "n,k,time_method1,cmp_method1,time_method2,cmp_method2\n");

    printf("=== Experiment A: fixed n=%d, varying k ===\n", n);
    for (int idx = 0; idx < num_k; idx++) {
        int k = ks[idx];
        double t1_total = 0, t2_total = 0;
        long long c1_total = 0, c2_total = 0;

        for (int trial = 0; trial < trials; trial++) {
            unsigned seed = (unsigned)(k * 7919 + trial);
            int **arrays = make_sorted_arrays(k, n, seed);

            long long cmp1 = 0, cmp2 = 0;
            clock_t start = clock();
            int *res1 = method1_sequential(arrays, k, n, &cmp1);
            clock_t end = clock();
            double t1 = (double)(end - start) / CLOCKS_PER_SEC;

            start = clock();
            int *res2 = method2_pairwise(arrays, k, n, &cmp2);
            end = clock();
            double t2 = (double)(end - start) / CLOCKS_PER_SEC;

            if (!check_sorted_and_complete(res1, k * n, arrays, k, n) ||
                !check_sorted_and_complete(res2, k * n, arrays, k, n)) {
                fprintf(stderr, "ERROR: validation failed for k=%d\n", k);
                exit(1);
            }

            t1_total += t1; t2_total += t2;
            c1_total += cmp1; c2_total += cmp2;

            free(res1); free(res2);
            free_arrays(arrays, k);
        }

        double t1_avg = t1_total / trials, t2_avg = t2_total / trials;
        long long c1_avg = c1_total / trials, c2_avg = c2_total / trials;

        printf("k=%4d  Method1: t=%.6fs cmp=%lld  |  Method2: t=%.6fs cmp=%lld\n",
               k, t1_avg, c1_avg, t2_avg, c2_avg);

        fprintf(fp, "%d,%d,%.6f,%lld,%.6f,%lld\n", n, k, t1_avg, c1_avg, t2_avg, c2_avg);
        fflush(fp);
    }
    fclose(fp);
    printf("Results written to results_vary_k.csv\n\n");
}

static void run_experiment_vary_n(void) {
    int k = 16; /* fixed number of arrays */
    int ns[] = {500, 1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000};
    int num_n = sizeof(ns) / sizeof(ns[0]);
    int trials = 5;

    FILE *fp = fopen("results_vary_n.csv", "w");
    fprintf(fp, "k,n,time_method1,cmp_method1,time_method2,cmp_method2\n");

    printf("=== Experiment B: fixed k=%d, varying n ===\n", k);
    for (int idx = 0; idx < num_n; idx++) {
        int n = ns[idx];
        double t1_total = 0, t2_total = 0;
        long long c1_total = 0, c2_total = 0;

        for (int trial = 0; trial < trials; trial++) {
            unsigned seed = (unsigned)(n * 131 + trial);
            int **arrays = make_sorted_arrays(k, n, seed);

            long long cmp1 = 0, cmp2 = 0;
            clock_t start = clock();
            int *res1 = method1_sequential(arrays, k, n, &cmp1);
            clock_t end = clock();
            double t1 = (double)(end - start) / CLOCKS_PER_SEC;

            start = clock();
            int *res2 = method2_pairwise(arrays, k, n, &cmp2);
            end = clock();
            double t2 = (double)(end - start) / CLOCKS_PER_SEC;

            if (!check_sorted_and_complete(res1, k * n, arrays, k, n) ||
                !check_sorted_and_complete(res2, k * n, arrays, k, n)) {
                fprintf(stderr, "ERROR: validation failed for n=%d\n", n);
                exit(1);
            }

            t1_total += t1; t2_total += t2;
            c1_total += cmp1; c2_total += cmp2;

            free(res1); free(res2);
            free_arrays(arrays, k);
        }

        double t1_avg = t1_total / trials, t2_avg = t2_total / trials;
        long long c1_avg = c1_total / trials, c2_avg = c2_total / trials;

        printf("n=%7d  Method1: t=%.6fs cmp=%lld  |  Method2: t=%.6fs cmp=%lld\n",
               n, t1_avg, c1_avg, t2_avg, c2_avg);

        fprintf(fp, "%d,%d,%.6f,%lld,%.6f,%lld\n", k, n, t1_avg, c1_avg, t2_avg, c2_avg);
        fflush(fp);
    }
    fclose(fp);
    printf("Results written to results_vary_n.csv\n");
}

int main(void) {
    run_experiment_vary_k();
    run_experiment_vary_n();
    return 0;
}
