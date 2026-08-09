#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ================= MERGE TWO SORTED ARRAYS ================= */

int *combineArrays(int *first, int sizeFirst, int *second, int sizeSecond)
{
    int total = sizeFirst + sizeSecond;

    int *merged = malloc(total * sizeof(int));

    int i = 0;
    int j = 0;
    int position = 0;

    // Compare elements from both arrays
    while (i < sizeFirst && j < sizeSecond)
    {
        if (first[i] <= second[j])
            merged[position++] = first[i++];
        else
            merged[position++] = second[j++];
    }

    // Copy remaining elements from first array
    while (i < sizeFirst)
        merged[position++] = first[i++];

    // Copy remaining elements from second array
    while (j < sizeSecond)
        merged[position++] = second[j++];

    return merged;
}


/* ============================================================
   METHOD 1: SEQUENTIAL MERGING
   Complexity: O(n * k^2)
   ============================================================ */

int *sequentialMerge(int **input, int numberOfArrays, int elements)
{
    if (numberOfArrays == 0)
        return NULL;

    // Start with the first sorted array
    int *combined = malloc(elements * sizeof(int));

    for (int i = 0; i < elements; i++)
        combined[i] = input[0][i];

    int currentLength = elements;

    // Merge one array at a time
    for (int i = 1; i < numberOfArrays; i++)
    {
        int *newResult =
            combineArrays(combined, currentLength,
                          input[i], elements);

        free(combined);

        combined = newResult;
        currentLength += elements;
    }

    return combined;
}


/* ============================================================
   METHOD 2: DIVIDE AND CONQUER
   Complexity: O(n * k log k)
   ============================================================ */

int *divideMergeRecursive(int **input,
                          int start,
                          int end,
                          int elements,
                          int *resultLength)
{
    // Base case: only one array remains
    if (start == end)
    {
        int *copy = malloc(elements * sizeof(int));

        for (int i = 0; i < elements; i++)
            copy[i] = input[start][i];

        *resultLength = elements;

        return copy;
    }

    int middle = start + (end - start) / 2;

    int leftLength;
    int rightLength;

    // Merge the left group of arrays
    int *leftResult =
        divideMergeRecursive(input, start, middle,
                              elements, &leftLength);

    // Merge the right group of arrays
    int *rightResult =
        divideMergeRecursive(input, middle + 1, end,
                              elements, &rightLength);

    // Combine both sorted results
    int *finalResult =
        combineArrays(leftResult, leftLength,
                      rightResult, rightLength);

    free(leftResult);
    free(rightResult);

    *resultLength = leftLength + rightLength;

    return finalResult;
}

int *divideMerge(int **input, int numberOfArrays, int elements)
{
    if (numberOfArrays == 0)
        return NULL;

    int totalLength;

    return divideMergeRecursive(input,
                                0,
                                numberOfArrays - 1,
                                elements,
                                &totalLength);
}


/* ============================================================
   BENCHMARK FUNCTION
   ============================================================ */

void runBenchmark()
{
    int elements = 500;

    // Different values of k are tested
    int arrayCounts[] = {100, 500, 1000, 2000, 4000};

    int testCount =
        sizeof(arrayCounts) / sizeof(arrayCounts[0]);

    printf("\n--- EFFICIENCY BENCHMARK ---\n");
    printf("Elements per array = %d\n", elements);
    printf("Comparing sequential and divide-and-conquer merging\n\n");

    printf("%-10s | %-20s | %-20s\n",
           "k", "Sequential Time", "Divide & Conquer Time");

    printf("--------------------------------------------------------------\n");

    for (int test = 0; test < testCount; test++)
    {
        int numberOfArrays = arrayCounts[test];

        // Allocate all sorted input arrays
        int **input =
            malloc(numberOfArrays * sizeof(int *));

        for (int i = 0; i < numberOfArrays; i++)
        {
            input[i] = malloc(elements * sizeof(int));

            // Create interleaved values for worst-case merging
            for (int j = 0; j < elements; j++)
            {
                input[i][j] =
                    i + (j * numberOfArrays);
            }
        }

        clock_t start;
        clock_t finish;

        /* ---------- Method 1 ---------- */

        start = clock();

        int *resultSequential =
            sequentialMerge(input,
                            numberOfArrays,
                            elements);

        finish = clock();

        double sequentialTime =
            (double)(finish - start) / CLOCKS_PER_SEC;


        /* ---------- Method 2 ---------- */

        start = clock();

        int *resultDivide =
            divideMerge(input,
                        numberOfArrays,
                        elements);

        finish = clock();

        double divideTime =
            (double)(finish - start) / CLOCKS_PER_SEC;


        printf("%-10d | %-20f | %-20f\n",
               numberOfArrays,
               sequentialTime,
               divideTime);


        // Free the result arrays
        free(resultSequential);
        free(resultDivide);

        // Free all input arrays
        for (int i = 0; i < numberOfArrays; i++)
            free(input[i]);

        free(input);
    }

    printf("\n");
}


/* ============================================================
   PRINT ARRAY
   ============================================================ */

void showArray(int *values, int length)
{
    printf("[");

    for (int i = 0; i < length; i++)
    {
        printf("%d%s",
               values[i],
               i == length - 1 ? "" : ", ");
    }

    printf("]\n");
}


/* ============================================================
   MAIN FUNCTION
   ============================================================ */

int main()
{
    /* ---------- Correctness Test ---------- */

    int numberOfArrays = 4;
    int elements = 3;

    int **input =
        malloc(numberOfArrays * sizeof(int *));

    for (int i = 0; i < numberOfArrays; i++)
        input[i] = malloc(elements * sizeof(int));

    int sampleData[4][3] =
    {
        {1, 5, 9},
        {2, 6, 10},
        {3, 7, 11},
        {4, 8, 12}
    };

    printf("--- CORRECTNESS TEST ---\n");

    // Copy sample data into dynamically allocated arrays
    for (int i = 0; i < numberOfArrays; i++)
    {
        for (int j = 0; j < elements; j++)
            input[i][j] = sampleData[i][j];

        printf("Array %d: ", i + 1);
        showArray(input[i], elements);
    }


    /* ---------- Method 1 Result ---------- */

    int *answerOne =
        sequentialMerge(input,
                        numberOfArrays,
                        elements);

    printf("\nMethod 1 - Sequential Result:\n");
    showArray(answerOne,
              numberOfArrays * elements);


    /* ---------- Method 2 Result ---------- */

    int *answerTwo =
        divideMerge(input,
                    numberOfArrays,
                    elements);

    printf("Method 2 - Divide & Conquer Result:\n");
    showArray(answerTwo,
              numberOfArrays * elements);


    // Release result memory
    free(answerOne);
    free(answerTwo);

    // Release input memory
    for (int i = 0; i < numberOfArrays; i++)
        free(input[i]);

    free(input);


    /* ---------- Performance Benchmark ---------- */

    runBenchmark();

    return 0;
}