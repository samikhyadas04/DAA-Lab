#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

typedef unsigned long long ull;

#define INIT_TRAJ_CAP  1024
#define MAX_STEPS      1000000   /* safety cap against infinite loops */

ull collatzStep(ull n) {
    if (n % 2 == 0) return n / 2;

    /* Overflow check: 3n+1 > ULLONG_MAX iff n > (ULLONG_MAX-1)/3 */
    if (n > (ULLONG_MAX - 1) / 3) {
        fprintf(stderr, "Overflow detected at n = %llu\n", n);
        return ULLONG_MAX;
    }
    return 3 * n + 1;
}

ull *singleTrajectory(ull start, int *steps, ull *maxVal) {
    int   cap   = INIT_TRAJ_CAP;
    ull  *traj  = (ull *)malloc(cap * sizeof(ull));
    if (!traj) return NULL;

    int   count = 0;
    ull   cur   = start;
    *maxVal     = start;

    while (cur != 1 && count < MAX_STEPS) {
        /* Grow buffer if needed */
        if (count >= cap - 1) {
            cap *= 2;
            ull *tmp = (ull *)realloc(traj, cap * sizeof(ull));
            if (!tmp) { free(traj); return NULL; }
            traj = tmp;
        }

        traj[count++] = cur;
        if (cur > *maxVal) *maxVal = cur;

        ull next = collatzStep(cur);
        if (next == ULLONG_MAX) { free(traj); return NULL; }
        cur = next;
    }

    traj[count++] = 1;   /* append the terminal 1 */
    *steps = count - 1;  /* number of steps to REACH 1 (excluding start) */
    return traj;
}

void analyseInterval(ull a, ull b) {
    ull longest_n    = a;
    int maxSteps     = 0;
    ull globalMax    = 0;
    ull globalMax_n  = a;

    printf("\n%-12s %-12s %-20s\n", "n", "Steps", "Max Value Reached");

    for (ull n = a; n <= b; n++) {
        int steps    = 0;
        ull maxVal   = n;
        ull cur      = n;

        while (cur != 1 && steps < MAX_STEPS) {
            ull next = collatzStep(cur);
            if (next == ULLONG_MAX) break;
            cur = next;
            if (cur > maxVal) maxVal = cur;
            steps++;
        }

        printf("%-12llu %-12d %-20llu\n", n, steps, maxVal);

        if (steps > maxSteps) {
            maxSteps  = steps;
            longest_n = n;
        }
        if (maxVal > globalMax) {
            globalMax   = maxVal;
            globalMax_n = n;
        }
    }

    printf("\nInterval Statistics [%llu, %llu]:\n", a, b);
    printf("  Longest trajectory : starts at n = %llu (%d steps)\n",
           longest_n, maxSteps);
    printf("  Highest value seen : %llu  (starting from n = %llu)\n",
           globalMax, globalMax_n);
}

void getManualSingle(ull *start) {
    printf("\n Manual Input (Single Value) \n");
    printf("Enter starting value n (>= 1): ");
    scanf("%llu", start);
    if (*start < 1) { printf("Invalid. Using n = 1.\n"); *start = 1; }
}

void getManualInterval(ull *a, ull *b) {
    printf("\n Manual Input (Interval) \n");
    printf("Enter interval start a (>= 1): ");
    scanf("%llu", a);
    printf("Enter interval end   b (>= a): ");
    scanf("%llu", b);
    if (*a < 1) *a = 1;
    if (*b < *a) *b = *a;
}

void getRandomSingle(ull *start) {
    srand((unsigned)time(NULL));
    *start = (ull)(rand() % 999) + 2;   /* 2 to 1000 */
    printf("\n Randomised Input (Single Value) \n");
    printf("Generated starting value n = %llu\n", *start);
}

void getRandomInterval(ull *a, ull *b) {
    srand((unsigned)time(NULL));
    *a = (ull)(rand() % 20) + 2;    /* 2 to 21 */
    *b = *a + (ull)(rand() % 15);   /* [a, a+14] */
    printf("\n Randomised Input (Interval) \n");
    printf("Generated interval [%llu, %llu]\n", *a, *b);
}

int main(void) {
    int outerChoice, inputChoice;

    printf("Select mode:\n");
    printf("  1. Single value trajectory\n");
    printf("  2. Interval [a, b] analysis\n");
    printf("Choice: ");
    scanf("%d", &outerChoice);

    printf("Select input method:\n");
    printf("  1. Manual input\n");
    printf("  2. Randomised data\n");
    printf("Choice: ");
    scanf("%d", &inputChoice);

    if (outerChoice == 1) {
        /* ── Single-value mode ── */
        ull start;
        if (inputChoice == 1)
            getManualSingle(&start);
        else
            getRandomSingle(&start);

        int steps;
        ull maxVal;
        ull *traj = singleTrajectory(start, &steps, &maxVal);

        if (!traj) {
            fprintf(stderr, "Trajectory computation failed (overflow or OOM).\n");
            return 1;
        }

        printf("\n Trajectory for n = %llu \n", start);
        printf("Steps to reach 1 : %d\n", steps);
        printf("Maximum value    : %llu\n", maxVal);
        printf("\nFull trajectory (each value on its own line):\n");

        for (int i = 0; i <= steps; i++) {
            printf("  Step %-5d : %llu", i, traj[i]);
            if (traj[i] % 2 == 0)
                printf("  → %llu  (÷2)\n", traj[i] / 2);
            else if (traj[i] != 1)
                printf("  → %llu  (×3+1)\n", 3 * traj[i] + 1);
            else
                printf("  ← reached 1\n");
        }

        free(traj);

    } else {
        /* ── Interval mode ── */
        ull a, b;
        if (inputChoice == 1)
            getManualInterval(&a, &b);
        else
            getRandomInterval(&a, &b);

        analyseInterval(a, b);
    }

    return 0;
}