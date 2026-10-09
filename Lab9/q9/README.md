# Q9 – Hu–Tucker Greedy Simulation (Optimal Alphabetic Binary Tree)

Goal: binary tree whose leaves appear **in the given order** `1..n`, minimising `Σ w_i · depth(i)`. (Huffman would be cheaper but reorders leaves.)

## Input representation
`n`, then `n` positive weights. A list of current nodes, each with `(weight, is_square, tree_id)`; arrays `Lc[], Rc[]` store the combination tree.

## Algorithm
**Phase 1 – combination.** Original leaves are *squares*, merged nodes are *circles*. A pair `(i, j)`, `i < j`, is **compatible** if no square lies strictly between them. Repeatedly pick the compatible pair with the **smallest weight sum** (ties → leftmost `i`, then leftmost `j`), replace it by one circle of weight `w_i + w_j` placed at the position of the **left** node.
**Phase 2 – levels.** The depth of each leaf in this (not necessarily alphabetic) tree is its level in the optimal alphabetic tree – this is the Hu–Tucker theorem (1971).
**Phase 3 – recombination.** Scan leaves left to right with a stack of `(subtree, level)`; push a leaf and, while the top two entries have equal level, merge them into one entry of level−1. With Hu–Tucker levels the stack ends with a single root of level 0, an alphabetic tree with exactly those depths.

## Complexity
| | |
|---|---|
| Phase 1 (as implemented) | `n−1` merges × scanning all compatible pairs `O(n²)` → **`O(n³)`** worst case; an `O(n²)` version scans each block "square, circles…, square" once per merge; the classical Hu–Tucker with priority queues (and Garsia–Wachs, its simplification) run in **`O(n log n)`** |
| Phase 2 | `O(n)` |
| Phase 3 | `O(n)` stack work (printing the bracket string costs `O(n²)` characters worst case) |
| **Space** | **`O(n)`** (lists, tree arrays, stack) |

## Validation (`./q9 --selftest`)
30000 random weight lists (n ≤ 12, half with weights in 1..4 to force many ties): Phase-1 cost equals the exact optimum of an `O(n³)` interval DP (`cost[i][j] = min_k cost[i][k]+cost[k+1][j] + W(i..j)`), and Phase 3 always rebuilds a valid tree → **0 failures**.

## Build & run
```
gcc -O2 -o q9 solution.c
./q9 < sample_input.txt      # weights 20 10 3 5 7 30 1 24
```
```
Leaf levels (depths): 2 3 5 5 4 2 3 3
Alphabetic tree: ((1 (2 ((3 4) 5))) (6 (7 8)))
Optimal cost sum w_i*depth_i (Hu-Tucker) = 273
Optimal cost (interval-DP check)         = 273
```
