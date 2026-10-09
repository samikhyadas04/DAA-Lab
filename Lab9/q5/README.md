# Q5 – Candy Distribution (Bi-directional Slope Greedy)

## Input representation
`n`, then `n` ratings (int array `r[]`). Output array `c[]` of candies. Rule: `c[i] ≥ 1`; if `r[i] > r[i±1]` then `c[i] > c[i±1]` (equal ratings impose nothing).

## Algorithm A – two passes (`O(n)` time, `O(n)` space)
* Left→right: `c[i] = c[i−1] + 1` if `r[i] > r[i−1]`, else 1 (satisfies the left-neighbour constraints minimally).
* Right→left: if `r[i] > r[i+1]` and `c[i] ≤ c[i+1]` set `c[i] = c[i+1] + 1` (satisfies the right constraints without breaking the left ones, because values only go up).
Each `c[i]` ends as the *smallest* value satisfying both constraint families, so the sum is minimal.

## Algorithm B – single pass over slopes (`O(n)` time, **`O(1)` space**)
The rating line is a sequence of up-slopes, flat parts and down-slopes. Track `up` (length of the current rise), `down` (length of the current fall) and `peak` (height given to the last peak). A rise of length `up` adds `1 + up`; a fall of length `down` adds `1 + down`, minus 1 if the fall is no longer than the preceding rise (`peak ≥ down`) since the peak child already has enough candies. Equal neighbours reset everything.

## Complexity
| | Time | Space |
|---|---|---|
| Two-pass | `O(n)` (2 linear scans) | `O(n)` |
| Slope | `O(n)` (1 scan) | `O(1)` extra |

## Validation (`./q5 --selftest`)
20000 random arrays (many ties): two-pass = slope = an independent fixed-point relaxation answer, and the distribution is verified to satisfy every rule → **0 failures**.

## Build & run
```
gcc -O2 -o q5 solution.c
./q5 < sample_input.txt      # 1 3 4 5 2 2 1
```
Output: `Candies per child: 1 2 3 4 1 2 1`, total `14`.
