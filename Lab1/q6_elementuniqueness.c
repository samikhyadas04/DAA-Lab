#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RANGE 100000     
#define TRIALS 200      
#define MAX_N 1000
#define STEP 20

int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}
int hasDuplicate(int arr[], int n)
{
    qsort(arr, n, sizeof(int), compare);
    for (int i = 0; i < n - 1; i++)
    {
        if (arr[i] == arr[i + 1])
            return 1;
    }
    return 0;
}

int main()
{
    srand((unsigned)time(NULL));

    FILE *fp = fopen("uniqueness_analysis.csv", "w");
    if (fp == NULL)
    {
        printf("Unable to create CSV file.\n");
        return 1;
    }
    fprintf(fp, "n,ProbabilityDuplicate,AvgTimeSeconds\n");

    printf("=============================================\n");
    printf("     ELEMENT UNIQUENESS - DUPLICATE ANALYSIS\n");
    printf("=============================================\n\n");
    printf("n\tP(duplicate)\tAvgTime(s)\n");
    printf("--------------------------------------\n");

    int *arr = malloc(MAX_N * sizeof(int));

    for (int n = STEP; n <= MAX_N; n += STEP)
    {
        int duplicateCount = 0;
        double totalTime = 0.0;

        for (int t = 0; t < TRIALS; t++)
        {
            for (int i = 0; i < n; i++)
                arr[i] = rand() % RANGE;

            clock_t start = clock();
            int found = hasDuplicate(arr, n);
            clock_t end = clock();

            totalTime += (double)(end - start) / CLOCKS_PER_SEC;
            if (found)
                duplicateCount++;
        }

        double probability = (double)duplicateCount / TRIALS;
        double avgTime = totalTime / TRIALS;

        printf("%4d\t%.4f\t\t%.8f\n", n, probability, avgTime);
        fprintf(fp, "%d,%.4f,%.8f\n", n, probability, avgTime);
    }

    free(arr);
    fclose(fp);
    printf("\nData saved to 'uniqueness_analysis.csv'\n");
    FILE *gp = popen("gnuplot", "w");
    if (gp == NULL)
    {
        printf("\ngnuplot not found - make sure it's installed and on PATH.\n");
        return 0;
    }
    fprintf(gp, "set datafile separator ','\n");
    fprintf(gp, "set terminal png size 950,600\n");
    fprintf(gp, "set output 'uniqueness_chart.png'\n");
    fprintf(gp, "set title 'Element Uniqueness: Probability of Duplicate & Time vs n'\n");
    fprintf(gp, "set xlabel 'n (number of random values)'\n");
    fprintf(gp, "set ylabel 'Probability of Duplicate' textcolor rgb 'steelblue'\n");
    fprintf(gp, "set y2label 'Average Time (seconds)' textcolor rgb 'red'\n");
    fprintf(gp, "set ytics nomirror textcolor rgb 'steelblue'\n");
    fprintf(gp, "set y2tics nomirror textcolor rgb 'red'\n");
    fprintf(gp, "set yrange [0:1]\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "set key left top\n");
    fprintf(gp, "plot 'uniqueness_analysis.csv' using 1:2 with linespoints title 'P(duplicate)' lc rgb 'steelblue' pt 7 axes x1y1, \\\n");
    fprintf(gp, "     'uniqueness_analysis.csv' using 1:3 with linespoints title 'Avg Time (s)' lc rgb 'red' pt 5 axes x1y2\n");
    int status = pclose(gp);
    if (status != 0)
        printf("Warning: gnuplot may have failed (exit code %d).\n", status);
    else
        printf("Chart image saved as 'uniqueness_chart.png' in your project folder.\n");
    printf("\n============== CONCLUSION ==============\n");
    printf("1. P(duplicate) rises sharply with n - birthday paradox effect.\n");
    printf("2. With RANGE=%d, duplicates become near-certain once n\n", RANGE);
    printf("   approaches ~1.2*sqrt(RANGE) (~%d here).\n", (int)(1.2 * 316));
    printf("3. Time grows as O(n log n) due to sorting - visibly slower\n");
    printf("   growth than linear, but far better than O(n^2) pairwise check.\n");
    printf("4. For large n relative to RANGE, duplicates are guaranteed by\n");
    printf("   the pigeonhole principle (n > RANGE means a duplicate MUST exist).\n");

    return 0;
}