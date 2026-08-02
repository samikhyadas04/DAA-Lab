#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long long moveCount = 0;
void towerOfHanoi(int n, char from, char aux, char to, int verbose)
{
    if (n == 0)
        return;

    towerOfHanoi(n - 1, from, to, aux, verbose);

    moveCount++;
    if (verbose)
        printf("Move disc %d from %c to %c\n", n, from, to);

    towerOfHanoi(n - 1, aux, from, to, verbose);
}

int main()
{
    int maxDiscs = 20;  
    FILE *fp = fopen("toh_moves.csv", "w");
    if (fp == NULL)
    {
        printf("Unable to create CSV file.\n");
        return 1;
    }
    fprintf(fp, "Discs,Moves,Formula_2n_minus_1\n");
    printf("=============================================\n");
    printf("      TOWER OF HANOI - MOVE COUNT ANALYSIS\n");
    printf("=============================================\n\n");
    printf("Discs\tMoves\t\t2^n - 1\n");
    printf("---------------------------------\n");

    for (int n = 1; n <= maxDiscs; n++)
    {
        moveCount = 0;
        towerOfHanoi(n, 'A', 'B', 'C', 0); 
        long long formula = (long long)(pow(2, n) - 1);
        printf("%3d\t%8lld\t%8lld\n", n, moveCount, formula);
        fprintf(fp, "%d,%lld,%lld\n", n, moveCount, formula);
    }

    fclose(fp);
    printf("\nData saved to 'toh_moves.csv'\n");
    printf("\n=============================================\n");
    printf("Sample move sequence for n = 3 discs:\n");
    printf("=============================================\n");
    moveCount = 0;
    towerOfHanoi(3, 'A', 'B', 'C', 1);
    printf("Total moves for 3 discs: %lld\n", moveCount);
    FILE *gp = popen("gnuplot", "w");
    if (gp == NULL)
    {
        printf("\ngnuplot not found - make sure it's installed and on PATH.\n");
        printf("Skipping chart generation; CSV data is still available.\n");
        return 0;
    }

    fprintf(gp, "set datafile separator ','\n");
    fprintf(gp, "set terminal png size 900,550\n");
    fprintf(gp, "set output 'toh_chart.png'\n");
    fprintf(gp, "set title 'Tower of Hanoi: Moves vs Number of Discs'\n");
    fprintf(gp, "set xlabel 'Number of Discs (n)'\n");
    fprintf(gp, "set ylabel 'Total Moves'\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "set key left top\n");
    fprintf(gp, "plot 'toh_moves.csv' using 1:2 with linespoints title 'Actual Moves' lc rgb 'steelblue' pt 7, \\\n");
    fprintf(gp, "     'toh_moves.csv' using 1:3 with lines title '2^n - 1 (theoretical)' lc rgb 'red' dashtype 2\n");

    int status = pclose(gp);
    if (status != 0)
        printf("Warning: gnuplot may have failed (exit code %d).\n", status);
    else
        printf("Chart image saved as 'toh_chart.png' in your project folder.\n");
    
        printf("\n============== CONCLUSION ==============\n");
    printf("1. Moves follow the formula 2^n - 1 exactly (optimal solution).\n");
    printf("2. Growth is exponential -> O(2^n), not linear or polynomial.\n");
    printf("3. Each extra disc roughly DOUBLES the total moves.\n");
    printf("4. Becomes impractical fast: n=30 already exceeds 1 billion moves.\n");
    printf("5. Shows why exponential algorithms are infeasible at large n,\n");
    printf("   regardless of hardware speed or code optimization.\n");

    return 0;
}