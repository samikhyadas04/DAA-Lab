

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum { RED = 0, BLUE = 1, YELLOW = 2 } Color;

typedef struct {
    long  number;
    Color color;
} Item;

static const char *color_name(Color c) {
    switch (c) {
        case RED:    return "R";
        case BLUE:   return "B";
        case YELLOW: return "Y";
    }
    return "?";
}

/*
 * sort_by_color
 * --------------
 * in  : array of n items, sorted by `number` (assumed by the
 *       problem statement; the algorithm does not re-check this)
 * n   : number of items
 * out : caller-supplied array with capacity >= n; filled with the
 *       items grouped RED, then BLUE, then YELLOW, numbers stable
 *       within each group
 *
 * O(n) time, O(1) extra space beyond the output array.
 */
void sort_by_color(const Item *in, int n, Item *out) {
    int count[3] = {0, 0, 0};
    for (int i = 0; i < n; i++) count[in[i].color]++;      /* pass 1: O(n) */

    int next[3];
    next[RED]    = 0;
    next[BLUE]   = count[RED];
    next[YELLOW] = count[RED] + count[BLUE];

    for (int i = 0; i < n; i++) {                            /* pass 2: O(n) */
        Color c = in[i].color;
        out[next[c]] = in[i];
        next[c]++;
    }
}

/* ---------------------------------------------------------------- */
/*   Independently-coded O(n) reference: three sequential filters   */
/* ---------------------------------------------------------------- */
static void reference_sort_by_color(const Item *in, int n, Item *out) {
    int k = 0;
    for (int i = 0; i < n; i++) if (in[i].color == RED)    out[k++] = in[i];
    for (int i = 0; i < n; i++) if (in[i].color == BLUE)   out[k++] = in[i];
    for (int i = 0; i < n; i++) if (in[i].color == YELLOW) out[k++] = in[i];
}

/* ---------------------------------------------------------------- */
/*                       Structural validation                      */
/* ---------------------------------------------------------------- */

/* All items of one colour must come before all items of the next
 * colour, in the fixed order RED, BLUE, YELLOW. */
static int is_color_grouped(const Item *arr, int n) {
    for (int i = 0; i + 1 < n; i++) {
        if (arr[i].color > arr[i + 1].color) return 0;
    }
    return 1;
}

/* Within each colour group, numbers must be non-decreasing
 * (stability w.r.t. the original number-sorted order). */
static int is_stable_within_colors(const Item *arr, int n) {
    for (int i = 0; i + 1 < n; i++) {
        if (arr[i].color == arr[i + 1].color && arr[i].number > arr[i + 1].number)
            return 0;
    }
    return 1;
}

/* Output must be a permutation of the input (same items, same
 * multiplicities) -- checked via an O(n log n) sort-and-compare
 * (used only for test validation, not part of the O(n) algorithm
 * itself). */
static int cmp_item_full(const void *pa, const void *pb) {
    const Item *a = (const Item *)pa, *b = (const Item *)pb;
    if (a->number != b->number) return (a->number < b->number) ? -1 : 1;
    return (int)a->color - (int)b->color;
}

static int same_multiset(const Item *a, const Item *b, int n) {
    Item *ca = (Item *)malloc(sizeof(Item) * n);
    Item *cb = (Item *)malloc(sizeof(Item) * n);
    for (int i = 0; i < n; i++) { ca[i] = a[i]; cb[i] = b[i]; }
    qsort(ca, n, sizeof(Item), cmp_item_full);
    qsort(cb, n, sizeof(Item), cmp_item_full);
    int ok = 1;
    for (int i = 0; i < n; i++) {
        if (ca[i].number != cb[i].number || ca[i].color != cb[i].color) { ok = 0; break; }
    }
    free(ca);
    free(cb);
    return ok;
}

static int items_equal_array(const Item *a, const Item *b, int n) {
    for (int i = 0; i < n; i++) {
        if (a[i].number != b[i].number || a[i].color != b[i].color) return 0;
    }
    return 1;
}

/* ---------------------------------------------------------------- */
/*                       Helper / demo utilities                    */
/* ---------------------------------------------------------------- */
static void print_items(const char *label, const Item *arr, int n) {
    printf("%s = [", label);
    for (int i = 0; i < n; i++) {
        printf("(%ld,%s)%s", arr[i].number, color_name(arr[i].color), (i == n - 1) ? "" : ",");
    }
    printf("]\n");
}

static int run_case(const char *name, const Item *in, int n, int verbose) {
    Item *fast_out = (Item *)malloc(sizeof(Item) * (n > 0 ? n : 1));
    Item *ref_out  = (Item *)malloc(sizeof(Item) * (n > 0 ? n : 1));

    sort_by_color(in, n, fast_out);
    reference_sort_by_color(in, n, ref_out);

    int matches_reference = items_equal_array(fast_out, ref_out, n);
    int grouped   = is_color_grouped(fast_out, n);
    int stable    = is_stable_within_colors(fast_out, n);
    int permuted  = same_multiset(in, fast_out, n);

    int ok = matches_reference && grouped && stable && permuted;

    if (verbose) {
        printf("---- %s ----\n", name);
        print_items("input ", in, n);
        print_items("output", fast_out, n);
        printf("matches reference impl: %s | colour-grouped: %s | stable: %s | same multiset: %s\n",
               matches_reference ? "yes" : "NO",
               grouped ? "yes" : "NO",
               stable ? "yes" : "NO",
               permuted ? "yes" : "NO");
        printf("Result: %s\n\n", ok ? "PASS" : "FAIL");
    }

    free(fast_out);
    free(ref_out);
    return ok;
}

int main(void) {
    int all_pass = 1;

    /* ---------------- Fixed, hand-checked examples ---------------- */
    {
        /* Numbers already sorted; colours mixed. */
        Item in[] = {
            {1, YELLOW}, {2, RED}, {3, BLUE}, {4, RED},
            {5, YELLOW}, {6, BLUE}, {7, RED}, {8, YELLOW}
        };
        all_pass &= run_case("Example 1 (mixed colours)", in, 8, 1);
    }
    {
        /* All the same colour. */
        Item in[] = { {1,BLUE}, {2,BLUE}, {3,BLUE}, {4,BLUE} };
        all_pass &= run_case("Example 2 (single colour)", in, 4, 1);
    }
    {
        /* Already in the desired colour order. */
        Item in[] = { {1,RED}, {5,RED}, {9,BLUE}, {12,BLUE}, {20,YELLOW} };
        all_pass &= run_case("Example 3 (already colour-sorted)", in, 5, 1);
    }
    {
        /* Reverse of desired colour order. */
        Item in[] = { {1,YELLOW}, {2,YELLOW}, {3,BLUE}, {4,BLUE}, {5,RED}, {6,RED} };
        all_pass &= run_case("Example 4 (reverse colour order)", in, 6, 1);
    }
    {
        /* Duplicate numbers with different colours -- tests that
         * stability is about relative INPUT order, not uniqueness
         * of numbers. */
        Item in[] = { {5,RED}, {5,BLUE}, {5,YELLOW}, {5,RED}, {5,BLUE} };
        all_pass &= run_case("Example 5 (duplicate numbers)", in, 5, 1);
    }
    {
        /* Single item. */
        Item in[] = { {42, YELLOW} };
        all_pass &= run_case("Example 6 (single item)", in, 1, 1);
    }
    {
        /* Empty input. */
        Item in[1] = { {0, RED} }; /* unused, n=0 below */
        all_pass &= run_case("Example 7 (empty input)", in, 0, 1);
    }
    {
        /* Negative numbers. */
        Item in[] = { {-30,BLUE}, {-20,RED}, {-10,YELLOW}, {0,RED}, {15,BLUE} };
        all_pass &= run_case("Example 8 (negative numbers)", in, 5, 1);
    }

    /* ---------------- Randomized stress testing ---------------- */
    printf("---- Randomized stress test vs independent reference ----\n");
    srand((unsigned)time(NULL));
    int trials = 2000;
    int pass_count = 0;
    for (int t = 0; t < trials; t++) {
        int n = rand() % 60; /* includes n=0 sometimes */
        Item *in = (Item *)malloc(sizeof(Item) * (n > 0 ? n : 1));

        /* Build numbers that are sorted (non-decreasing), with
         * random repeats, and assign a random colour to each. */
        long num = (rand() % 21) - 10;
        for (int i = 0; i < n; i++) {
            num += rand() % 3; /* stays non-decreasing */
            in[i].number = num;
            in[i].color = (Color)(rand() % 3);
        }

        int ok = run_case("random", in, n, 0);
        pass_count += ok;
        if (!ok) {
            printf("Mismatch found on trial %d (n=%d)!\n", t, n);
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