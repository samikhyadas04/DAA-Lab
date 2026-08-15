# Q5 - Multiplication of Special-Pattern Square Matrices

## Problem Statement

Two `n × n` matrices are given, where `n = 2^k`. Each matrix has the
recursive structure:

``` text
M = [M1 M2]
    [M2 M1]
```

The diagonal blocks are identical and the off-diagonal blocks are also
identical. This structure continues recursively down to individual
integer elements.

Develop a divide-and-conquer algorithm to multiply two such matrices in
`O(n²)` time and validate the complexity.

## Approach

Represent each structured matrix using its two unique blocks:

``` text
M = [M1 M2]
    [M2 M1]
```

For two matrices:

``` text
A = [A1 A2]    B = [B1 B2]
    [A2 A1]        [B2 B1]
```

their product has the same structured form. The multiplication can
therefore be expressed using a constant number of additions and
recursive multiplications on the unique blocks rather than performing
all ordinary matrix multiplications.

At each recursion level: - Split the matrices into four blocks. - Reuse
identical blocks instead of storing/computing them separately. -
Recursively multiply only the required unique block combinations. -
Combine the results using matrix addition/subtraction. - Stop when the
block size becomes `1 × 1`.

## Complexity

The intended recurrence is of the form:

``` text
T(n) = 4T(n/2) + O(n²)
```

Therefore:

``` text
T(n) = O(n²)
```

## Conclusion

The special recursive structure allows repeated blocks to be reused.
This significantly reduces the number of independent recursive
multiplications and gives an `O(n²)` divide-and-conquer algorithm.
