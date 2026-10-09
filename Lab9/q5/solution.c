/*
 * Q5  Candy distribution (bi-directional slope greedy)
 * Method A: two passes (left->right, right->left, take max)        O(n) time, O(n) space
 * Method B: single pass tracking length of current up-slope / down-slope  O(n) time, O(1) space
 * Check   : fixed-point relaxation (start with all 1s, repeat raising until stable) + constraint test.
 * Input : n, then n ratings.   Build: gcc -O2 -o q5 solution.c   Run: ./q5 < sample_input.txt | ./q5 --selftest
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long long two_pass(const int *r, int n, int *c) {
    for (int i = 0; i < n; i++) c[i] = 1;
    for (int i = 1; i < n; i++) if (r[i] > r[i-1]) c[i] = c[i-1] + 1;           /* left neighbour rule  */
    for (int i = n - 2; i >= 0; i--) if (r[i] > r[i+1] && c[i] <= c[i+1]) c[i] = c[i+1] + 1; /* right rule */
    long long s = 0; for (int i = 0; i < n; i++) s += c[i]; return s;
}

static long long slope(const int *r, int n) {
    if (n == 0) return 0;
    long long total = 1; int up = 0, down = 0, peak = 0;
    for (int i = 1; i < n; i++) {
        if (r[i] > r[i-1])       { up++; down = 0; peak = up; total += 1 + up; }
        else if (r[i] == r[i-1]) { up = down = peak = 0; total += 1; }
        else {                    up = 0; down++; total += 1 + down; if (peak >= down) total--; }  /* peak already tall enough */
    }
    return total;
}

static long long relax(const int *r, int n) {
    int *c = malloc(n * sizeof(int)); for (int i = 0; i < n; i++) c[i] = 1;
    for (int ch = 1; ch;) { ch = 0;
        for (int i = 0; i < n; i++) {
            if (i > 0   && r[i] > r[i-1] && c[i] <= c[i-1]) { c[i] = c[i-1] + 1; ch = 1; }
            if (i < n-1 && r[i] > r[i+1] && c[i] <= c[i+1]) { c[i] = c[i+1] + 1; ch = 1; } } }
    long long s = 0; for (int i = 0; i < n; i++) s += c[i]; free(c); return s;
}

static int valid(const int *r, const int *c, int n) {
    for (int i = 0; i < n; i++) { if (c[i] < 1) return 0;
        if (i > 0 && r[i] > r[i-1] && c[i] <= c[i-1]) return 0;
        if (i < n-1 && r[i] > r[i+1] && c[i] <= c[i+1]) return 0; }
    return 1;
}

int main(int argc, char **argv) {
    if (argc > 1 && !strcmp(argv[1], "--selftest")) {
        srand(9); int bad = 0;
        for (int tc = 0; tc < 20000; tc++) {
            int n = 1 + rand() % 12, r[12], c[12]; int m = 1 + rand() % 6;
            for (int i = 0; i < n; i++) r[i] = rand() % m;
            long long a = two_pass(r, n, c), b = slope(r, n), z = relax(r, n);
            if (a != b || a != z || !valid(r, c, n)) { bad++; printf("FAIL tc=%d\n", tc); }
        }
        printf("selftest: 20000 cases, %d failures\n", bad); return bad != 0;
    }
    int n; if (scanf("%d", &n) != 1) return 1;
    int *r = malloc(n * sizeof(int)), *c = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) if (scanf("%d", &r[i]) != 1) return 1;
    long long a = two_pass(r, n, c);
    printf("Candies per child:"); for (int i = 0; i < n; i++) printf(" %d", c[i]);
    printf("\nMinimum total candies (two-pass) = %lld\nMinimum total candies (O(1)-space slope) = %lld\n", a, slope(r, n));
    return 0;
}
