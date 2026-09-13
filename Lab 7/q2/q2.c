

#include <stdio.h>
#include <stdlib.h>

/* ---------- Formulation A: classic dp[e][f], O(E*F*log F) ---------- */

static int minTrialsClassicDP(int E, int F) {
    if (F == 0) return 0;
    if (E == 0) return -1; /* undefined: no eggs, can't test */

    /* dp[e][f] for e = 0..E, f = 0..F */
    int **dp = (int **)malloc((E + 1) * sizeof(int *));
    for (int e = 0; e <= E; e++)
        dp[e] = (int *)calloc(F + 1, sizeof(int));

    /* Base cases */
    for (int f = 0; f <= F; f++) dp[1][f] = f;          /* 1 egg: linear */
    for (int e = 2; e <= E; e++) dp[e][0] = 0;
    for (int e = 2; e <= E; e++) dp[e][1] = 1;

    for (int e = 2; e <= E; e++) {
        for (int f = 2; f <= F; f++) {
            int lo = 1, hi = f, best = f; /* fallback = worst possible */
            while (lo <= hi) {
                int x = lo + (hi - lo) / 2;
                int breakCase = dp[e - 1][x - 1];   /* egg breaks at x   */
                int surviveCase = dp[e][f - x];     /* egg survives at x */
                int worst = (breakCase > surviveCase) ? breakCase : surviveCase;
                if (worst < best) best = worst;

                /* Move toward the crossover: if breakCase < surviveCase,
                   increasing x helps (breakCase rises, surviveCase falls) */
                if (breakCase < surviveCase) lo = x + 1;
                else hi = x - 1;
            }
            dp[e][f] = 1 + best;
        }
    }

    int result = dp[E][F];
    for (int e = 0; e <= E; e++) free(dp[e]);
    free(dp);
    return result;
}

/* ---------- Formulation B: f(e,k) max-floors-covered, O(E*K) ---------- */

static int minTrialsFastDP(int E, int F) {
    if (F == 0) return 0;

    /* dp[e] = max floors distinguishable with e eggs at current k */
    int *dp = (int *)calloc(E + 1, sizeof(int));
    int k = 0;

    while (dp[E] < F) {
        k++;
        /* update from e = E downward so dp[e-1] on the RHS is still
           the value from the previous k (in-place rolling array) */
        for (int e = E; e >= 1; e--) {
            dp[e] = dp[e] + dp[e - 1] + 1;
        }
        if (k > F) break; /* safety guard, should never trigger */
    }

    free(dp);
    return k;
}

/* ---------- Driver / validation ---------- */

int main(int argc, char *argv[]) {
    if (argc == 3) {
        int E = atoi(argv[1]);
        int F = atoi(argv[2]);
        int a = minTrialsClassicDP(E, F);
        int b = minTrialsFastDP(E, F);
        printf("Eggs=%d Floors=%d -> ClassicDP=%d  FastDP=%d  %s\n",
               E, F, a, b, (a == b) ? "(match)" : "(MISMATCH!)");
        return 0;
    }

    /* No args: run a validation suite */
    struct { int E, F; } tests[] = {
        {1, 100}, {2, 100}, {2, 10}, {2, 36}, {2, 200},
        {3, 100}, {3, 1000}, {4, 1000}, {5, 1000}, {10, 1000},
        {2, 1}, {2, 0}, {1, 1}, {7, 5000}
    };
    int n = sizeof(tests) / sizeof(tests[0]);

    printf("%-6s %-8s %-14s %-10s %s\n", "Eggs", "Floors", "ClassicDP", "FastDP", "Match?");
    for (int i = 0; i < n; i++) {
        int E = tests[i].E, F = tests[i].F;
        int a = minTrialsClassicDP(E, F);
        int b = minTrialsFastDP(E, F);
        printf("%-6d %-8d %-14d %-10d %s\n", E, F, a, b, (a == b) ? "yes" : "NO");
    }

    printf("\nHeadline result: with 2 eggs and 100 floors, minimum guaranteed "
           "drops = %d\n", minTrialsFastDP(2, 100));
    return 0;
}