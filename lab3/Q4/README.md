# Q4 - Matrix Multiplication Using Strassen's Method

## Problem Statement

Multiply two square matrices of size `n × n` using Strassen's
divide-and-conquer matrix multiplication algorithm.

## Approach

For two matrices divided into four blocks:

``` text
A = [A11 A12]    B = [B11 B12]
    [A21 A22]        [B21 B22]
```

Strassen's method computes seven recursive matrix products instead of
the eight products used by ordinary divide-and-conquer multiplication.

The seven products are:

``` text
P1 = A11(B12 - B22)
P2 = (A11 + A12)B22
P3 = (A21 + A22)B11
P4 = A22(B21 - B11)
P5 = (A11 + A22)(B11 + B22)
P6 = (A12 - A22)(B21 + B22)
P7 = (A11 - A21)(B11 + B12)
```

The result blocks are then obtained from these seven products.

## Complexity

-   Strassen's recurrence: `T(n) = 7T(n/2) + O(n²)`
-   Time: `O(n^log₂7) ≈ O(n^2.807)`
-   Space: depends on temporary matrices and implementation

## Conclusion

Strassen's algorithm improves the asymptotic complexity of matrix
multiplication by reducing the number of recursive multiplications from
eight to seven.
