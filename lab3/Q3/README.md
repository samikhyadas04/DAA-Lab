# Q3 - Maximum and Minimum Using Divide and Conquer

## Problem Statement

Find both the maximum and minimum elements of an array of size `n` using
the divide-and-conquer approach. The number of comparisons should be
bounded by `3n/2`.

## Approach

1.  If the subarray contains one element, it is both the minimum and
    maximum.
2.  If it contains two elements, compare them once and determine the
    minimum and maximum.
3.  Otherwise, divide the array into two halves.
4.  Recursively find the minimum and maximum of both halves.
5.  Compare the two minimums and the two maximums.

## Complexity

-   Time: `O(n)`
-   Comparisons: at most about `3n/2`
-   Space: `O(log n)` due to recursion

## Conclusion

The divide-and-conquer method efficiently finds both minimum and maximum
values while reducing unnecessary comparisons compared with separately
scanning for minimum and maximum.
