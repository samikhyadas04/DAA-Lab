

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    long x; /* start */
    long y; /* end   */
} Interval;

/* ---------- comparator: sort by start value ---------- */
static int cmp_interval_by_x(const void *a, const void *b) {
    long xa = ((const Interval *)a)->x;
    long xb = ((const Interval *)b)->x;
    if (xa < xb) return -1;
    if (xa > xb) return 1;
    return 0;
}

/*
 * merge_intervals
 * ----------------
 * in    : array of n input intervals (not modified; a local sorted
 *         copy is made internally)
 * n     : number of input intervals
 * out   : caller-supplied array with capacity >= n; filled with the
 *         merged, disjoint intervals in increasing order of start
 * Returns the number of merged intervals written to `out` (<= n).
 *
 * O(n log n) time, O(n) extra space for the sorted copy.
 */
int merge_intervals(const Interval *in, int n, Interval *out) {
    if (n == 0) return 0;

    Interval *sorted = (Interval *)malloc(sizeof(Interval) * n);
    for (int i = 0; i < n; i++) sorted[i] = in[i];
    qsort(sorted, n, sizeof(Interval), cmp_interval_by_x);   /* O(n log n) */

    int k = 0;                          /* number of intervals emitted */
    long curX = sorted[0].x;
    long curY = sorted[0].y;

    for (int i = 1; i < n; i++) {       /* O(n) sweep */
        if (sorted[i].x <= curY) {
            /* overlaps or touches the current merged interval */
            if (sorted[i].y > curY) curY = sorted[i].y;
        } else {
            /* gap found: current merged interval is complete */
            out[k].x = curX;
            out[k].y = curY;
            k++;
            curX = sorted[i].x;
            curY = sorted[i].y;
        }
    }
    out[k].x = curX;
    out[k].y = curY;
    k++;

    free(sorted);
    return k;
}

/* ---------------------------------------------------------------- */
/*        Brute-force reference: repeatedly merge any overlapping   */
/*        pair until a fixed point is reached, then sort result.    */
/*        (Same overlap rule: touching endpoints count as overlap.) */
/* ---------------------------------------------------------------- */
static int overlaps(Interval a, Interval b) {
    /* they share a point iff neither is entirely to the left of the
     * other, using the "touching counts as overlap" rule */
    return a.x <= b.y && b.x <= a.y;
}

static int brute_force_merge(const Interval *in, int n, Interval *out) {
    Interval *work = (Interval *)malloc(sizeof(Interval) * n);
    int m = n;
    for (int i = 0; i < n; i++) work[i] = in[i];

    int changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < m && !changed; i++) {
            for (int j = i + 1; j < m; j++) {
                if (overlaps(work[i], work[j])) {
                    /* merge j into i, remove j */
                    if (work[j].x < work[i].x) work[i].x = work[j].x;
                    if (work[j].y > work[i].y) work[i].y = work[j].y;
                    work[j] = work[m - 1];
                    m--;
                    changed = 1;
                    break;
                }
            }
        }
    }

    qsort(work, m, sizeof(Interval), cmp_interval_by_x);
    for (int i = 0; i < m; i++) out[i] = work[i];
    free(work);
    return m;
}

/* ---------------------------------------------------------------- */
/*                       Helper / demo utilities                    */
/* ---------------------------------------------------------------- */
static void print_intervals(const char *label, const Interval *arr, int n) {
    printf("%s = {", label);
    for (int i = 0; i < n; i++) {
        printf("(%ld,%ld)%s", arr[i].x, arr[i].y, (i == n - 1) ? "" : ",");
    }
    printf("}\n");
}

static int intervals_equal(const Interval *a, int na, const Interval *b, int nb) {
    if (na != nb) return 0;
    for (int i = 0; i < na; i++) {
        if (a[i].x != b[i].x || a[i].y != b[i].y) return 0;
    }
    return 1;
}

/* Sanity check on the fast algorithm's own output: results must be
 * sorted by start and pairwise non-overlapping/non-touching
 * (i.e. genuinely maximal, no further merge possible). */
static int output_is_well_formed(const Interval *out, int k) {
    for (int i = 0; i + 1 < k; i++) {
        if (out[i].x > out[i + 1].x) return 0;      /* not sorted */
        if (out[i].y >= out[i + 1].x) return 0;      /* still overlapping/touching */
    }
    for (int i = 0; i < k; i++) {
        if (out[i].y <= out[i].x) return 0;          /* degenerate interval */
    }
    return 1;
}

static int run_case(const char *name, const Interval *in, int n, int verbose) {
    Interval *fast_out  = (Interval *)malloc(sizeof(Interval) * n);
    Interval *brute_out = (Interval *)malloc(sizeof(Interval) * n);

    int fast_k  = merge_intervals(in, n, fast_out);
    int brute_k = brute_force_merge(in, n, brute_out);

    int well_formed = output_is_well_formed(fast_out, fast_k);
    int matches      = intervals_equal(fast_out, fast_k, brute_out, brute_k);
    int ok = well_formed && matches;

    if (verbose) {
        printf("---- %s ----\n", name);
        print_intervals("input ", in, n);
        print_intervals("FAST  ", fast_out, fast_k);
        print_intervals("BRUTE ", brute_out, brute_k);
        printf("Result: %s\n\n", ok ? "MATCH (PASS)" : "MISMATCH (FAIL)");
    }

    free(fast_out);
    free(brute_out);
    return ok;
}

int main(void) {
    int all_pass = 1;

    /* ---------------- The example from the problem statement ---------------- */
    {
        Interval in[] = { {1,3}, {2,6}, {8,10}, {7,18} };
        all_pass &= run_case("Problem example: expect {(1,6),(7,18)}", in, 4, 1);
    }

    /* ---------------- Additional fixed, hand-checked examples ---------------- */
    {
        /* Fully disjoint intervals: nothing merges. */
        Interval in[] = { {1,2}, {5,6}, {10,12} };
        all_pass &= run_case("Example 2 (disjoint, nothing merges)", in, 3, 1);
    }
    {
        /* Touching endpoints should merge: (1,4)+(4,7) -> (1,7) */
        Interval in[] = { {1,4}, {4,7}, {9,10} };
        all_pass &= run_case("Example 3 (touching endpoints merge)", in, 3, 1);
    }
    {
        /* Nested intervals: one interval fully contains another. */
        Interval in[] = { {1,20}, {5,7}, {8,9}, {21,22} };
        all_pass &= run_case("Example 4 (nested intervals)", in, 4, 1);
    }
    {
        /* All intervals collapse into a single one. */
        Interval in[] = { {5,10}, {1,6}, {8,15}, {14,20} };
        all_pass &= run_case("Example 5 (everything merges into one)", in, 4, 1);
    }
    {
        /* Single interval. */
        Interval in[] = { {3,9} };
        all_pass &= run_case("Example 6 (single interval)", in, 1, 1);
    }
    {
        /* Input already given out of order and with duplicates. */
        Interval in[] = { {10,12}, {1,3}, {1,3}, {2,4}, {11,14} };
        all_pass &= run_case("Example 7 (unsorted input with duplicates)", in, 5, 1);
    }
    {
        /* Negative coordinates. */
        Interval in[] = { {-10,-5}, {-6,0}, {2,4}, {3,3+1} };
        all_pass &= run_case("Example 8 (negative coordinates)", in, 4, 1);
    }

    /* ---------------- Randomized stress testing ---------------- */
    printf("---- Randomized stress test vs brute force ----\n");
    srand((unsigned)time(NULL));
    int trials = 2000;
    int pass_count = 0;
    for (int t = 0; t < trials; t++) {
        int n = 1 + rand() % 40;
        Interval *in = (Interval *)malloc(sizeof(Interval) * n);
        for (int i = 0; i < n; i++) {
            long x = (rand() % 101) - 50;              /* start in [-50,50] */
            long len = 1 + rand() % 20;                /* length in [1,20]  */
            in[i].x = x;
            in[i].y = x + len;
        }

        int ok = run_case("random", in, n, 0);
        pass_count += ok;
        if (!ok) {
            printf("Mismatch found on trial %d!\n", t);
            all_pass = 0;
        }
        free(in);
    }
    printf("Random trials passed: %d / %d\n\n", pass_count, trials);

    printf("=================================\n");
    printf("OVERALL RESULT: %s\n", all_pass ? "ALL TESTS PASSED" : "SOME TESTS FAILED");
    printf("=================================\n");

    return all_pass ? 0 : 1;
}