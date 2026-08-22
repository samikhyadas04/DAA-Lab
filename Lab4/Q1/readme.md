# Stable Sort by Colour (3-Bucket Counting Sort) — O(n) Algorithm

## Problem

Given `n` pairs `(number, colour)`, already **sorted by number**,
where colour is one of `{RED, BLUE, YELLOW}`, rearrange the items so
that all REDs come before all BLUEs before all YELLOWs, while the
numbers **within** each colour group remain sorted — i.e. the sort
must be **stable** with respect to the original order.

## Algorithm

**Input/output representation:** each item is an
`Item { long number; Color color; }` struct (`Color` is a 3-valued
enum `RED=0, BLUE=1, YELLOW=2`), and the whole input/output is a
plain array of `Item`. This is exactly what the algorithm needs: O(1)
random-access writes into a freshly allocated output array, with no
pointers or in-place swapping that could risk breaking stability.

This is a **3-bucket counting sort**, which is naturally stable when
implemented with prefix-sum bucket offsets:

**Steps:**

1. Scan the input once and count how many items are RED, BLUE, and
   YELLOW: `countR, countB, countY`. — `O(n)`
2. Compute the starting index of each colour's block in the output
   array via a running prefix sum:
   ```
   startR = 0
   startB = countR
   startY = countR + countB
   ```
   — `O(1)`
3. Scan the input a **second time, in original order**. For each
   item, place it at `out[next[colour]]` and increment
   `next[colour]` (initialised to `{startR, startB, startY}`). — `O(n)`

**Total time complexity:** `O(n)` — two linear passes, no
comparisons, no comparison-based sorting involved.
**Space complexity:** `O(n)` for the output array; `O(1)` extra for
the counters/offsets (a fixed 3 colours → a constant number of
counters).

### Why this is stable

Step 3 processes the input in its original (number-sorted) order and
simply appends each item to the next free slot in its colour's block.
Since we never compare or reorder items relative to each other — only
bucket them by colour while preserving arrival order within a bucket
— items of the same colour land in the output in exactly the relative
order they had in the input, i.e. still sorted by number.

*(Note: the classic in-place "Dutch National Flag" 3-way partition
also sorts 3 colours in `O(n)` time and `O(1)` extra space, but it is
**not** stable — it can reorder equal-numbered items within a colour.
Since the problem requires numbers to stay sorted within each colour,
the stable counting-sort/bucket approach above is used instead of
Dutch flag partitioning.)*

## Time Complexity

| Step                                             | Cost   |
|-----------------------------------------------------|--------|
| Count items per colour                               | `O(n)` |
| Compute bucket start offsets                          | `O(1)` |
| Place each item into its bucket (original-order scan) | `O(n)` |
| **Total**                                             | **`O(n)`** |

**Space:** `O(n)` for the output array, `O(1)` extra for the 3
counters/offsets.

## Notes

- **Input representation matters.** An array with O(1) indexed writes
  is what lets step 3 place each item directly at its computed bucket
  offset in one pass, with no shifting or searching — a linked-list
  representation would still be O(n) but would need extra bookkeeping
  (three separate list tails) rather than simple index arithmetic.
- **Only 3 colours is what makes this O(n) rather than O(n log n).**
  Counting sort is linear precisely because the number of distinct
  keys (colours) is a small constant; the algorithm would not stay
  `O(n)` if the number of "colours" could grow with `n`.
- **Correctness was validated, not just argued.** `sortbycolor.c`
  cross-checks the fast algorithm against an independently-coded
  `O(n)` reference (three sequential filtering passes — emit all
  REDs in input order, then all BLUEs, then all YELLOWs) and also
  directly verifies three structural properties on the fast
  algorithm's own output:
  1. **colour-grouped** — no BLUE appears before a RED, no YELLOW
     before a BLUE, etc.
  2. **stable within each colour** — numbers are non-decreasing
     inside each colour's block.
  3. **same multiset as the input** — no items lost, duplicated, or
     altered.

  This runs on 8 fixed cases (mixed colours, a single colour,
  already colour-sorted input, reverse colour order, duplicate
  numbers with different colours, a single item, empty input, and
  negative numbers) plus 2000 randomized trials (0–59 items, sorted
  non-decreasing numbers with repeats, random colours) — all passed.

## Example (worked)

```
input = [(1,Y), (2,R), (3,B), (4,R), (5,Y), (6,B), (7,R), (8,Y)]
```

1. Count: `countR = 3, countB = 2, countY = 3`
2. Offsets: `startR = 0, startB = 3, startY = 5`
3. Scan input in order, placing each item and advancing its bucket
   pointer:

   | item   | goes to index | next[colour] after |
   |--------|---------------|---------------------|
   | (1,Y)  | 5             | next[Y] = 6         |
   | (2,R)  | 0             | next[R] = 1         |
   | (3,B)  | 3             | next[B] = 4         |
   | (4,R)  | 1             | next[R] = 2         |
   | (5,Y)  | 6             | next[Y] = 7         |
   | (6,B)  | 4             | next[B] = 5         |
   | (7,R)  | 2             | next[R] = 3         |
   | (8,Y)  | 7             | next[Y] = 8         |

4. Result: `[(2,R), (4,R), (7,R), (3,B), (6,B), (1,Y), (5,Y), (8,Y)]`

Note the REDs are `2, 4, 7` (still increasing), the BLUEs are `3, 6`
(still increasing), and the YELLOWs are `1, 5, 8` (still increasing)
— each colour group preserved its original relative order, exactly
as required.

## Files

- `sortbycolor.c` — implementation of the algorithm
  (`sort_by_color`), an independently-coded three-pass reference
  implementation (`reference_sort_by_color`), structural validators
  (`is_color_grouped`, `is_stable_within_colors`, `same_multiset`),
  and a test harness covering the cases listed above.


Expected output ends with:

```
=================================
OVERALL RESULT: ALL TESTS PASSED
=================================
```

## Using `sort_by_color` in your own code

```c
Item in[]  = { {1,YELLOW}, {2,RED}, {3,BLUE}, {4,RED} };
Item out[4];   /* capacity must be >= n */

sort_by_color(in, 4, out);
for (int i = 0; i < 4; i++) {
    printf("(%ld,%s) ", out[i].number, color_name(out[i].color));
}
/* (2,R) (4,R) (3,B) (1,Y) */
```