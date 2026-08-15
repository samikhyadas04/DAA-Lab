# Q2 - Search the Defective Coin

## Problem Statement

Among `n` coins, exactly one coin may be lighter than the others. Using
a balance weighing scale, find the defective coin if it exists, or
report that all coins have the same weight.

The algorithm should use divide and conquer and perform at most
`O(log₂ n)` weighings for the specified positive constant factor.

## Approach

-   Divide the coins into nearly equal groups.
-   Weigh two groups against each other.
-   If their weights are equal, the defective coin is in the remaining
    group.
-   If one side is lighter, the defective coin is in that lighter group.
-   Recursively continue with the identified group.
-   When one coin remains, verify it against a known normal coin if
    necessary.

## Complexity

-   Time / Number of weighings: `O(log n)`
-   Auxiliary space: `O(log n)` for recursive calls

## Conclusion

The divide-and-conquer strategy reduces the number of possible defective
coins by a constant factor after every weighing, giving logarithmic
complexity.
