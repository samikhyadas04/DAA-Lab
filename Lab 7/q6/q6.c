#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[50];
    int birth;
    int death;
} Scientist;

int compareInts(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main() {
    int n;
    printf("Enter number of scientists: ");
    scanf("%d", &n);

    Scientist *sci = malloc(n * sizeof(Scientist));
    int *birth = malloc(n * sizeof(int));
    int *death = malloc(n * sizeof(int));

    printf("Enter name, birth year, death year for each scientist:\n");
    for (int i = 0; i < n; i++) {
        printf("Scientist %d: ", i + 1);
        scanf("%49s %d %d", sci[i].name, &sci[i].birth, &sci[i].death);
        birth[i] = sci[i].birth;
        death[i] = sci[i].death;
    }

    // Sort birth and death years independently
    qsort(birth, n, sizeof(int), compareInts);
    qsort(death, n, sizeof(int), compareInts);

    // Sweep / merge step
    int i = 0, j = 0, current = 0;
    int maxAlive = 0, maxYear = birth[0];

    while (i < n && j < n) {
        if (birth[i] < death[j]) {
            current++;
            if (current > maxAlive) {
                maxAlive = current;
                maxYear = birth[i];
            }
            i++;
        } else {
            // birth[i] >= death[j]: death processed first (tie rule)
            current--;
            j++;
        }
    }

    printf("\nMaximum number of scientists alive simultaneously: %d\n", maxAlive);
    printf("This maximum was first reached in the year: %d\n", maxYear);

    free(sci);
    free(birth);
    free(death);
    return 0;
}