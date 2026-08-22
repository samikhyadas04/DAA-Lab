
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    long time;
    int  delta;   /* +1 = entry, -1 = exit */
} Event;

/* Sort events purely by time (all times are distinct, so no
 * tie-breaking rule is required). */
static int cmp_event(const void *pa, const void *pb) {
    long ta = ((const Event *)pa)->time;
    long tb = ((const Event *)pb)->time;
    if (ta < tb) return -1;
    if (ta > tb) return 1;
    return 0;
}

/*
 * max_occupancy
 * --------------
 * a, b   : entry/exit time arrays, size n each (b[i] > a[i])
 * n      : number of guests
 * out_time : set to the time at which peak occupancy is (first)
 *            reached
 *
 * Returns the maximum number of guests simultaneously present.
 * O(n log n) time, O(n) space.
 */
int max_occupancy(const long *a, const long *b, int n, long *out_time) {
    Event *events = (Event *)malloc(sizeof(Event) * 2 * n);
    for (int i = 0; i < n; i++) {
        events[2 * i]     = (Event){ a[i], +1 };
        events[2 * i + 1] = (Event){ b[i], -1 };
    }

    qsort(events, 2 * n, sizeof(Event), cmp_event);   /* O(n log n) */

    int current = 0, best = 0;
    long best_time = (n > 0) ? events[0].time : 0;

    for (int i = 0; i < 2 * n; i++) {                  /* O(n) */
        current += events[i].delta;
        if (events[i].delta == +1 && current > best) {
            best = current;
            best_time = events[i].time;
        }
    }

    free(events);
    *out_time = best_time;
    return best;
}

/* ---------------------------------------------------------------- */
/*                    Brute-force O(n^2) reference                  */
/* ---------------------------------------------------------------- */
/* Try every entry time a_i as a candidate peak moment (the true
 * maximum must occur at some entry time, by the same argument used
 * in the fast algorithm) and count occupancy at that instant by
 * scanning all n guests. */
static int brute_force(const long *a, const long *b, int n, long *out_time) {
    int best = 0;
    long best_time = (n > 0) ? a[0] : 0;
    for (int i = 0; i < n; i++) {
        long t = a[i];
        int count = 0;
        for (int j = 0; j < n; j++) {
            if (a[j] <= t && t < b[j]) count++;
        }
        if (count > best) {
            best = count;
            best_time = t;
        }
    }
    *out_time = best_time;
    return best;
}

/* ---------------------------------------------------------------- */
/*                       Helper / demo utilities                    */
/* ---------------------------------------------------------------- */
static void print_guests(const long *a, const long *b, int n) {
    printf("guests (entry, exit): ");
    for (int i = 0; i < n; i++) {
        printf("(%ld,%ld)%s", a[i], b[i], (i == n - 1) ? "" : ", ");
    }
    printf("\n");
}

/* Count occupancy at a given instant t via brute force - used to
 * verify that the *time* the fast algorithm reports really does
 * carry the maximum occupancy (there can be multiple instants that
 * tie for the max). */
static int occupancy_at(const long *a, const long *b, int n, long t) {
    int count = 0;
    for (int j = 0; j < n; j++) {
        if (a[j] <= t && t < b[j]) count++;
    }
    return count;
}

static int run_case(const char *name, const long *a, const long *b, int n,
                     int verbose) {
    long fast_time, brute_time;
    int fast_best  = max_occupancy(a, b, n, &fast_time);
    int brute_best = brute_force(a, b, n, &brute_time);

    /* The two algorithms need not report the *same* peak time if
     * ties exist, so the real correctness check is:
     *   1) the maximum COUNTS match, and
     *   2) the time the fast algorithm reports genuinely achieves
     *      that maximum occupancy when checked independently.
     */
    int fast_time_ok = (occupancy_at(a, b, n, fast_time) == fast_best);
    int ok = (fast_best == brute_best) && fast_time_ok;

    if (verbose) {
        printf("---- %s ----\n", name);
        print_guests(a, b, n);
        printf("FAST : peak = %d guests at time %ld\n", fast_best, fast_time);
        printf("BRUTE: peak = %d guests at time %ld\n", brute_best, brute_time);
        printf("Result: %s\n\n", ok ? "MATCH (PASS)" : "MISMATCH (FAIL)");
    }
    return ok;
}

int main(void) {
    int all_pass = 1;

    /* ---------------- Fixed, hand-checked examples ---------------- */
    {
        /* Classic example:
           p1: 1-4   p2: 2-5   p3: 3-9   p4: 6-8  -> peak 3 people during [3,4) */
        long a[] = {1, 2, 3, 6};
        long b[] = {4, 5, 9, 8};
        all_pass &= run_case("Example 1 (classic overlap)", a, b, 4, 1);
    }
    {
        /* No overlap at all: everyone comes and goes separately. */
        long a[] = {1, 10, 20, 30};
        long b[] = {5, 15, 25, 35};
        all_pass &= run_case("Example 2 (no overlaps, peak=1)", a, b, 4, 1);
    }
    {
        /* Everyone overlaps at one common point. */
        long a[] = {1, 2, 3, 4, 5};
        long b[] = {100, 90, 80, 70, 60};
        all_pass &= run_case("Example 3 (all n overlap)", a, b, 5, 1);
    }
    {
        /* Single guest. */
        long a[] = {7};
        long b[] = {42};
        all_pass &= run_case("Example 4 (single guest)", a, b, 1, 1);
    }
    {
        /* Negative / mixed timestamps (e.g. minutes relative to some epoch). */
        long a[] = {-10, -5, 0, 3};
        long b[] = {-2, 6, 8, 10};
        all_pass &= run_case("Example 5 (negative timestamps)", a, b, 4, 1);
    }

    /* ---------------- Randomized stress testing ---------------- */
    printf("---- Randomized stress test vs brute force ----\n");
    srand((unsigned)time(NULL));
    int trials = 2000;
    int pass_count = 0;
    for (int t = 0; t < trials; t++) {
        int n = 1 + rand() % 50;
        long *a = (long *)malloc(sizeof(long) * n);
        long *b = (long *)malloc(sizeof(long) * n);

        /* Generate n distinct entry times and, for each, a distinct
         * exit time strictly greater than the entry time; then
         * shuffle-ensure global distinctness by drawing from a wide
         * range and rejecting collisions (range is large enough that
         * collisions are rare, and we just resample on the rare
         * collision). */
        long used_count = 0;
        long *used = (long *)malloc(sizeof(long) * 2 * n);
        for (int i = 0; i < n; i++) {
            long at, bt;
            /* ensure uniqueness against everything used so far */
            int collide;
            do {
                collide = 0;
                at = rand() % 1000;
                for (long k = 0; k < used_count; k++)
                    if (used[k] == at) { collide = 1; break; }
            } while (collide);
            used[used_count++] = at;

            do {
                collide = 0;
                bt = at + 1 + rand() % 1000;
                for (long k = 0; k < used_count; k++)
                    if (used[k] == bt) { collide = 1; break; }
            } while (collide);
            used[used_count++] = bt;

            a[i] = at;
            b[i] = bt;
        }
        free(used);

        int ok = run_case("random", a, b, n, 0);
        pass_count += ok;
        if (!ok) {
            printf("Mismatch found on trial %d!\n", t);
            all_pass = 0;
        }
        free(a);
        free(b);
    }
    printf("Random trials passed: %d / %d\n\n", pass_count, trials);

    printf("=================================\n");
    printf("OVERALL RESULT: %s\n", all_pass ? "ALL TESTS PASSED" : "SOME TESTS FAILED");
    printf("=================================\n");

    return all_pass ? 0 : 1;
}