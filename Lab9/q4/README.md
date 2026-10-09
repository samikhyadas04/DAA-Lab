# Q4 – Minimum Cost to Connect Sticks

## Input representation
`n`, then `n` lengths. A **min-heap of `long long`** (the total cost can exceed 32 bits: with `n` sticks of length `L` it is about `L·n·log n`).

## Algorithm
Repeat `n−1` times: pop the two shortest sticks `x, y`, pay `x + y`, push `x + y`.

**Why it is optimal:** this is exactly Huffman's algorithm. Every stick `L_i` is paid once for each merge it participates in, i.e. `depth(i)` times in the merge tree, so total cost = `Σ L_i · depth(i)` – the weighted external path length, minimised by Huffman merging (the two smallest can be assumed to be the deepest siblings; merging them is an optimal substructure step).

## Complexity
| | |
|---|---|
| Heap build | `O(n)` with bottom-up heapify (the code uses `n` pushes: `O(n log n)`) |
| `n−1` iterations × (2 pops + 1 push) | `O(n log n)` |
| **Time** | **`O(n log n)`** |
| **Space** | **`O(n)`** |

Edge case: `n = 1` costs 0.

## Validation (`./q4 --selftest`)
3000 random instances (n ≤ 7) vs an exhaustive search that tries every pair at every step: **0 failures**.

## Build & run
```
gcc -O2 -o q4 solution.c
./q4 < sample_input.txt      # 2 4 3 7 1 5
```
Output: `Minimum total cost = 53` (merges: 1+2, 3+3, 4+5, 6+7, 9+13).
