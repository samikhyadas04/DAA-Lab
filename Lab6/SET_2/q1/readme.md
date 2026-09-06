# DAA Lab-06, Question 1 - Time & Space Complexity Analysis

Input representation: array of n integers, contiguous storage
(O(1) random access).

## (i) Maximum element
- **TC:** Theta(n) worst case - one comparison per element, single pass.
  Optimal: Omega(n) lower bound, since every element must be inspected
  at least once to rule it out as the maximum.
- **SC:** O(1) extra space (one variable `max`).

## (ii) First and second largest
- **TC:** O(n) - single pass, at most two comparisons per element.
- **SC:** O(1) extra space (`first`, `second`).

## (iii) Mean
- **TC:** Theta(n) - every element must contribute to the running sum.
- **SC:** O(1) extra space (accumulator `sum`).

## (iv) Median
- **TC:** O(n log n) using a comparison sort (implementation used).
  An O(n) worst-case bound is achievable via the median-of-medians
  (BFPRT) selection algorithm: `T(n) = T(n/5) + T(7n/10) + O(n) = O(n)`.
- **SC:** O(n) for the sorted copy `tmp` (the input array itself is
  not modified). The BFPRT alternative needs only O(1) extra space
  if done in place (O(log n) recursion stack).

## (v) Standard deviation
- **TC:** Theta(n) - one linear pass given the mean.
- **SC:** O(1) extra space (accumulator `sumSq`).

## (vi) Mode
- **TC:** O(n log n) worst case, dominated by the sort; the scan for
  the longest run afterward is Theta(n).
  A hash-table approach gives O(n) expected time (no worst-case
  guarantee under adversarial hash collisions).
- **SC:** O(n) for the sorted copy. The hash-table alternative uses
  O(n) space for the counts as well.

## (vii) Removing all duplicates
- **TC:** O(n log n), dominated by the sort; the compaction pass is
  Theta(n). A hash-set pass gives O(n) expected time instead.
- **SC:** O(1) extra space beyond the array itself if sorted/compacted
  in place (as implemented); the hash-set alternative needs O(n)
  extra space.

## (viii) Reversing the array
- **TC:** Theta(n) - exactly floor(n/2) swaps; every element must
  move, so this is optimal.
- **SC:** O(1) extra space - purely in place.

## (ix) Partitioning (>= pivot placed before < pivot)
- **TC:** Theta(n) - one linear pass, at most one swap per element.
- **SC:** O(1) extra space - purely in place (mirrors the classical
  Lomuto/Hoare partition used in quicksort).

## Summary table

| Part | Operation                 | Time complexity | Space complexity |
|------|-----------------------------|:----------------:|:-----------------:|
| i    | Maximum                      | O(n)             | O(1) |
| ii   | 1st & 2nd largest            | O(n)             | O(1) |
| iii  | Mean                         | O(n)             | O(1) |
| iv   | Median                       | O(n log n) [O(n) via BFPRT] | O(n) [O(1) via BFPRT in place] |
| v    | Standard deviation           | O(n)             | O(1) |
| vi   | Mode                         | O(n log n) [O(n) expected via hashing] | O(n) |
| vii  | Remove duplicates            | O(n log n) [O(n) expected via hashing] | O(1) in place [O(n) via hashing] |
| viii | Reverse                      | O(n)             | O(1) |
| ix   | Partition (>=pivot first)    | O(n)             | O(1) |