# Q1 - Binary vs Ternary Search

## Problem Statement

Given a sorted list of `n` elements, search for an element `x` using
both binary search and ternary search. Compare their performance and
justify which approach is better.

## Approach

### Binary Search

-   Divide the search range into two halves.
-   Compare `x` with the middle element.
-   Continue searching in the appropriate half.

### Ternary Search

-   Divide the search range into three nearly equal parts.
-   Compare `x` with the two division points.
-   Continue searching in the relevant part.

## Complexity

-   Binary Search: `O(log₂ n)` time
-   Ternary Search: `O(log₃ n)` iterations
-   Space: `O(1)` for iterative implementations

## Conclusion

Although ternary search uses fewer levels because it divides the array
into three parts, each level requires more comparisons. For searching in
a sorted array, binary search is generally more efficient in practice
because it performs fewer comparisons per iteration.
