#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 2000
#define STEP 100

long long comparisons;
void generateRandomArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 10000;
}
void copyArray(int src[], int dest[], int n)
{
    for (int i = 0; i < n; i++)
        dest[i] = src[i];
}

// ---------- Version 1: Optimized (early termination) ----------
long long bubbleSortOptimized(int arr[], int n)
{
    comparisons = 0;
    for (int i = 0; i < n - 1; i++)
    {
        int swapped = 0;
        for (int j = 0; j < n - i - 1; j++)
        {
            comparisons++;
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
            }
        }
        if (swapped == 0)
            break;
    }
    return comparisons;
}

// ---------- Version 2: Always completes all (n-1) passes ----------
long long bubbleSortFull(int arr[], int n)
{
    comparisons = 0;
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            comparisons++;
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
        // no early exit check here - always runs all n-1 passes
    }
    return comparisons;
}

int main()
{
    srand((unsigned)time(NULL));

    FILE *fp = fopen("bubble_sort_analysis.csv", "w");
    if (fp == NULL)
    {
        printf("Unable to create CSV file.\n");
        return 1;
    }
    fprintf(fp, "n,OptimizedComparisons,FullComparisons\n");

    printf("=============================================\n");
    printf("   BUBBLE SORT: OPTIMIZED vs FULL PASS ANALYSIS\n");
    printf("=============================================\n\n");
    printf("n\tOptimized\tFull\n");
    printf("----------------------------------\n");

    int original[MAX_SIZE], work[MAX_SIZE];

    for (int n = STEP; n <= MAX_SIZE; n += STEP)
    {
        generateRandomArray(original, n);

        copyArray(original, work, n);
        long long optComp = bubbleSortOptimized(work, n);

        copyArray(original, work, n);
        long long fullComp = bubbleSortFull(work, n);

        printf("%d\t%lld\t\t%lld\n", n, optComp, fullComp);
        fprintf(fp, "%d,%lld,%lld\n", n, optComp, fullComp);
    }

    fclose(fp);
    printf("\nData saved to 'bubble_sort_analysis.csv'\n");

    
    FILE *gp = popen("gnuplot", "w");
    if (gp == NULL)
    {
        printf("\ngnuplot not found - make sure it's installed and on PATH.\n");
        return 0;
    }

    fprintf(gp, "set datafile separator ','\n");
    fprintf(gp, "set terminal png size 900,550\n");
    fprintf(gp, "set output 'bubble_sort_chart.png'\n");
    fprintf(gp, "set title 'Bubble Sort: Comparisons vs Array Size'\n");
    fprintf(gp, "set xlabel 'Array Size (n)'\n");
    fprintf(gp, "set ylabel 'Number of Comparisons'\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "set key left top\n");
    fprintf(gp, "plot 'bubble_sort_analysis.csv' using 1:2 with linespoints title 'Optimized (early exit)' lc rgb 'steelblue' pt 7, \\\n");
    fprintf(gp, "     'bubble_sort_analysis.csv' using 1:3 with linespoints title 'Full (n-1) passes' lc rgb 'red' pt 5\n");

    int status = pclose(gp);
    if (status != 0)
        printf("Warning: gnuplot may have failed (exit code %d).\n", status);
    else
        printf("Chart image saved as 'bubble_sort_chart.png' in your project folder.\n");

    printf("\n============== CONCLUSION ==============\n");
    printf("1. On random data, both versions perform nearly identically -\n");
    printf("   early termination rarely triggers before the last pass.\n");
    printf("2. Both still show O(n^2) growth in comparisons.\n");
    printf("3. The optimized version's real benefit appears on nearly-sorted\n");
    printf("   or already-sorted data, not shown by random-input testing here.\n");
    printf("4. Takeaway: early-exit optimization is a best-case improvement,\n");
    printf("   not a worst/average-case one.\n");

    return 0;
}