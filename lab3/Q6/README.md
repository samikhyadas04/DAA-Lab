# Q6 - Loop Invariants in Sorting

## Problem Statement

Consider an array `A[1...n]`. The algorithm repeatedly finds the
smallest element in the unsorted portion of the array and exchanges it
with the first element of that portion.

The task is to: - Write the pseudocode. - State and prove the loop
invariant. - Explain why the algorithm runs for `n - 1` iterations. -
Analyze its running time. - Determine whether the best-case running time
is better. - Implement the algorithm in C.

## Algorithm

``` text
for i = 1 to n - 1
    min = i
    for j = i + 1 to n
        if A[j] < A[min]
            min = j
    swap A[i] and A[min]
```

## Loop Invariant

At the start of each iteration `i`: - `A[1...i-1]` contains the smallest
`i-1` elements. - These elements are in sorted order. - Therefore, every
element in `A[1...i-1]` is less than or equal to every element in
`A[i...n]`.

### Initialization

Before the first iteration, `i = 1`, so the sorted portion is empty. The
invariant is true.

### Maintenance

During iteration `i`, the smallest element in `A[i...n]` is found and
swapped with `A[i]`. Thus `A[1...i]` becomes correctly sorted,
preserving the invariant.

### Termination

After iteration `n-1`, the first `n-1` elements are sorted. The final
element must automatically be the largest, so the entire array is
sorted.

## Why Only `n - 1` Iterations?

After placing the smallest `n-1` elements in their correct positions,
only one element remains. Its position is necessarily correct, so
another iteration is unnecessary.

## Complexity

The inner loop performs approximately:

``` text
(n-1) + (n-2) + ... + 1 = n(n-1)/2
```

comparisons.

Therefore: - Best case: `O(n²)` - Average case: `O(n²)` - Worst case:
`O(n²)` - Extra space: `O(1)`

## Best-Case Analysis

The best case is still `O(n²)` because the algorithm must scan the
remaining unsorted portion even when the array is already sorted. It
does not stop early.

## Conclusion

The algorithm is essentially selection sort. Its loop invariant proves
correctness, and its running time remains `O(n²)` in all cases.
