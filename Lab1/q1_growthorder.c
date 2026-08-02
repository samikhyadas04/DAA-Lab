#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    const char* name;
    double value;
} listofFunctions;
int compare(const void* a, const void* b) {
    double val_a = ((listofFunctions*)a)->value;
    double val_b = ((listofFunctions*)b)->value;
    
    if (val_a < val_b) return -1;
    if (val_a > val_b) return 1;
    return 0;
}
int main() {
    double n;
    printf("Enter a value for n: \n");
    scanf("%lf", &n); 

    listofFunctions functions[] = {
        { "n * log2(n)", n * log2(n) },
        { "12 * sqrt(n)", 12.0 * sqrt(n) },
        { "1", 1.0 },
        { "2 * n^3", 2.0 * pow(n, 3.0) },
        { "100 * n^2 + 6 * n", 100.0 * pow(n, 2.0) + 6.0 * n },
        { "n^0.51", pow(n, 0.51) },
        { "n^2 - 324", pow(n, 2.0) - 324.0 },
        { "50 * n^0.5", 50.0 * pow(n, 0.5) },
        { "3^n", pow(3.0, n) },
        { "2^32 * n", pow(2.0, 32.0) * n },
        { "log2(n)", log2(n) },
        { "n", n }
    };

    int length = sizeof(functions) / sizeof(functions[0]);

    qsort(functions, length, sizeof(listofFunctions), compare);

    printf("Functions in ascending order of growth for n = %.1f:\n\n", n);
    for (int i = 0; i < length; i++) {
        printf("%d. %-20s (Evaluated value: %e)\n", i + 1, functions[i].name, functions[i].value);
    }
    return 0;
}