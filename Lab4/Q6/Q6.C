

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    long l, r; /* inclusive endpoints, l <= r */
} Interval;

typedef struct {
    long time;
    int  delta;   /* +1 = interval starts, -1 = interval ends */
} Event;

/*
 * Sort events by coordinate; on a tie, START events (+1) come
 * before END events (-1). This is what makes inclusive endpoints
 * that coincide (one interval's r equals another's l) count
 * correctly as both covering that shared point.
 */
static int cmp_event(const void *pa, const void *pb) {
    const Event *ea = (const Event *)pa;
    const Event *eb = (const Event *)pb;
    if (ea->time != eb->time) return (ea->time < eb->time) ? -1 : 1;
    /* same coordinate: +1 (start) before -1 (end) => descending delta */
    return eb->delta - ea->delta;
}

/*
 * max_point_coverage
 * --------------------
 * in     : array of n intervals, in[i] = { l_i, r_i }, l_i <= r_i
 * n      : number of intervals
 * out_p  : set to a point achieving the maximum coverage
 *
 * Returns the maximum number of intervals that simultaneously
 * contain some point. O(n log n) time, O(n) space.
 */
int max_point_coverage(const Interval *in, int n, long *out_p) {
    Event *events = (Event *)malloc(sizeof(Event) * 2 * n);
    for (int i = 0; i < n; i++) {
        events[2 * i]     = (Event){ in[i].l, +1 };
        events[2 * i + 1] = (Event){ in[i].r, -1 };
    }

    qsort(events, 2 * n, sizeof(Event), cmp_event);   /* O(n log n) */

    int current = 0, best = 0;
    long best_p = (n > 0) ? events[0].time : 0;

    for (int i = 0; i < 2 * n; i++) {                  /* O(n) */
        current += events[i].delta;
        if (events[i].delta == +1 && current > best) {
            best = current;
            best_p = events[i].time;
        }
    }

    free(events);
    *out_p = best_p;
    return best;
}

/* ---------------------------------------------------------------- */
/*                    Brute-force O(n^2) reference                  */
/* ---------------------------------------------------------------- */
/* The true maximum is always attained at some l_i (see comment
 * block above), so it suffices to try every l_i as a candidate
 * point and count containment directly by scanning all n
 * intervals. */
static int count_at(const Interval *in, int n, long t) {
    int c = 0;
    for (int j = 0; j < n; j++) {
        if (in[j].l <= t && t <= in[j].r) c++;
    }
    return c;
}

static int brute_force(const Interval *in, int n, long *out_p) {
    int best = 0;
    long best_p = (n > 0) ? in[0].l : 0;
    for (int i = 0; i < n; i++) {
        int c = count_at(in, n, in[i].l);
        if (c > best) {
            best = c;
            best_p = in[i].l;
        }
    }
    *out_p = best_p;
    return best;
}

/* ---------------------------------------------------------------- */
/*                       Helper / demo utilities                    */
/* ---------------------------------------------------------------- */
static void print_intervals(const Interval *in, int n) {
    printf("S = {");
    for (int i = 0; i < n; i++) {
        printf("(%ld,%ld)%s", in[i].l, in[i].r, (i == n - 1) ? "" : ",");
    }
    printf("}\n");
}

static int run_case(const char *name, const Interval *in, int n, int verbose) {
    long fast_p, brute_p;
    int fast_best  = max_point_coverage(in, n, &fast_p);
    int brute_best = brute_force(in, n, &brute_p);

    /* Ties can occur (multiple points achieving the same max), so
     * the real check is: the maximum COUNTS agree, and the point
     * the fast algorithm reports genuinely achieves that count when
     * verified independently. */
    int fast_p_ok = (count_at(in, n, fast_p) == fast_best);
    int ok = (fast_best == brute_best) && fast_p_ok;

    if (verbose) {
        printf("---- %s ----\n", name);
        print_intervals(in, n);
        printf("FAST : max = %d intervals, achieved at p = %ld\n", fast_best, fast_p);
        printf("BRUTE: max = %d intervals, achieved at p = %ld\n", brute_best, brute_p);
        printf("Result: %s\n\n", ok ? "MATCH (PASS)" : "MISMATCH (FAIL)");
    }
    return ok;
}

int main(void) {
    int all_pass = 1;

    /* ---------------- The example from the problem statement ---------------- */
    {
        Interval in[] = { {10,40}, {20,60}, {50,90}, {15,70} };
        all_pass &= run_case("Problem example: expect max = 3", in, 4, 1);
    }

    /* ---------------- Additional fixed, hand-checked examples ---------------- */
    {
        /* Two intervals touching exactly at one point: [1,5] and
         * [5,10] both cover the inclusive point 5. */
        Interval in[] = { {1,5}, {5,10}, {20,30} };
        all_pass &= run_case("Example 2 (touching endpoints, inclusive)", in, 3, 1);
    }
    {
        /* Fully disjoint intervals: best is 1 anywhere inside any one. */
        Interval in[] = { {1,2}, {5,6}, {10,12} };
        all_pass &= run_case("Example 3 (disjoint intervals, max=1)", in, 3, 1);
    }
    {
        /* All n intervals share a common point. */
        Interval in[] = { {1,100}, {2,90}, {3,80}, {4,70}, {5,60} };
        all_pass &= run_case("Example 4 (all n overlap)", in, 5, 1);
    }
    {
        /* Single interval. */
        Interval in[] = { {7,42} };
        all_pass &= run_case("Example 5 (single interval)", in, 1, 1);
    }
    {
        /* Degenerate interval: l == r (a single point). */
        Interval in[] = { {5,5}, {5,10}, {1,5} };
        all_pass &= run_case("Example 6 (degenerate point interval)", in, 3, 1);
    }
    {
        /* Negative coordinates. */
        Interval in[] = { {-10,-2}, {-5,6}, {0,8}, {3,10} };
        all_pass &= run_case("Example 7 (negative coordinates)", in, 4, 1);
    }
    {
        /* Many intervals touching at the same single point (chain of ties). */
        Interval in[] = { {1,5}, {5,9}, {5,5}, {2,5}, {5,20} };
        all_pass &= run_case("Example 8 (many intervals meeting at x=5)", in, 5, 1);
    }

    /* ---------------- Randomized stress testing ---------------- */
    printf("---- Randomized stress test vs brute force ----\n");
    srand((unsigned)time(NULL));
    int trials = 2000;
    int pass_count = 0;
    for (int t = 0; t < trials; t++) {
        int n = 1 + rand() % 50;
        Interval *in = (Interval *)malloc(sizeof(Interval) * n);
        for (int i = 0; i < n; i++) {
            long l = (rand() % 41) - 20;        /* left in [-20,20]   */
            long len = rand() % 21;             /* length in [0,20]   */
            in[i].l = l;
            in[i].r = l + len;                  /* r >= l, ties likely */
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