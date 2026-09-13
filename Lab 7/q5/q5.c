#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Generate the shooting sequence: 2,3,...,n-1, n-1,...,3,2
int *generateSequence(int n, int *len) {
    if (n == 2) {
        *len = 2;
        int *seq = malloc(2 * sizeof(int));
        seq[0] = 1; seq[1] = 1;
        return seq;
    }
    *len = 2 * (n - 2);
    int *seq = malloc((*len) * sizeof(int));
    int idx = 0;
    for (int i = 2; i <= n - 1; i++) seq[idx++] = i;   // forward sweep
    for (int i = n - 1; i >= 2; i--) seq[idx++] = i;   // backward sweep
    return seq;
}

// Validate via the exact "possible-location set" computation.
// Returns true (guaranteed hit) if the set becomes empty.
bool validate(int n, int *seq, int len, bool verbose) {
    bool *possible = malloc((n + 1) * sizeof(bool));
    bool *nextP    = malloc((n + 1) * sizeof(bool));
    for (int i = 1; i <= n; i++) possible[i] = true;

    for (int t = 0; t < len; t++) {
        int shot = seq[t];
        possible[shot] = false;   

        if (verbose) {
            printf("Shot %2d at spot %2d -> possible = { ", t + 1, shot);
            for (int i = 1; i <= n; i++) if (possible[i]) printf("%d ", i);
            printf("}\n");
        }

        for (int i = 1; i <= n; i++) nextP[i] = false;
        for (int i = 1; i <= n; i++) {
            if (possible[i]) {
                if (i - 1 >= 1) nextP[i - 1] = true;
                if (i + 1 <= n) nextP[i + 1] = true;
            }
        }
        for (int i = 1; i <= n; i++) possible[i] = nextP[i];
    }

    bool anyLeft = false;
    for (int i = 1; i <= n; i++) if (possible[i]) anyLeft = true;

    free(possible);
    free(nextP);
    return !anyLeft;
}

int main() {
    int n;
    printf("Enter number of hiding spots (n > 1): ");
    scanf("%d", &n);
    if (n <= 1) { printf("n must be greater than 1.\n"); return 1; }

    int len;
    int *seq = generateSequence(n, &len);

    printf("\nShooting sequence (%d shots): ", len);
    for (int i = 0; i < len; i++) printf("%d ", seq[i]);
    printf("\n\n--- Detailed trace ---\n");

    bool ok = validate(n, seq, len, true);
    printf("\nResult for n=%d: %s\n", n,
           ok ? "GUARANTEED HIT (possible-location set is empty)."
              : "FAILED (some trajectory survives!)");
    free(seq);

    // Batch sanity check across many n values
    printf("\n--- Batch validation, n = 2..20 ---\n");
    for (int m = 2; m <= 20; m++) {
        int L;
        int *s = generateSequence(m, &L);
        bool res = validate(m, s, L, false);
        printf("n=%2d | shots=%2d | %s\n", m, L, res ? "OK" : "FAIL");
        free(s);
    }
    return 0;
}