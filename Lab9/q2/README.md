# Q2 – Huffman Coding with Canonical Codebook

## Input representation
`n`, then `n` lines `symbol frequency` (symbol = any token without spaces). Internally: arrays `W[], L[], R[]` for the tree (leaves `0..n-1`, internal nodes `n..2n-2`) and a binary **min-heap of node indices** ordered by `(weight, index)` so ties are deterministic.

## Algorithm
1. **Huffman:** push all leaves; `n−1` times pop the two lightest nodes, create a parent with the sum, push it back. Depth of each leaf = code length (computed top-down in `O(n)` by walking internal nodes in reverse creation order). `n = 1` gets length 1.
2. **Canonical codebook:** only the *lengths* are kept. Sort by `(length, symbol)`. The first code is all zeros; each next code is `(previous + 1)` followed by zeros until the new length is reached.
   This gives prefix-free codes of the same lengths (hence the same optimal expected length), and the codebook is determined uniquely by the lengths.

**Why it is optimal:** greedy-choice property (the two least frequent symbols can be assumed to be deepest siblings) + optimal substructure (merging them into one symbol of weight `f_a+f_b` reduces the problem by one symbol, adding exactly `f_a+f_b` to the cost).

## Complexity
| | |
|---|---|
| Huffman tree | `n−1` merges × 3 heap operations of `O(log n)` = **`O(n log n)`** |
| Depths | `O(n)` |
| Canonical step | sort `O(n log n)` + code construction `O(n·L)`, `L` = longest code ≤ `n−1` (with machine-word codes, `L ≤ 64`, it is `O(n)`) |
| **Time total** | `O(n log n)` typical, `O(n²)` worst-case bit operations with unbounded code strings |
| **Space** | `O(n)` for tree/heap; the printed codes need `O(n·L)` characters |

If the frequencies are already sorted, the **two-queue method** builds the tree in `O(n)` (used in the self-test as an independent check).

## Validation (`./q2 --selftest`)
2000 random instances: codes are prefix-free, Kraft sum = 1, `Σ f·len` equals the cost, and the cost equals the independent two-queue Huffman cost → **0 failures**.

## Build & run
```
gcc -O2 -o q2 solution.c
./q2 < sample_input.txt
```
Sample (`a45 b13 c12 d16 e9 f5`):
```
a  45  1  0
b  13  3  100
c  12  3  101
d  16  3  110
e  9   4  1110
f  5   4  1111
Total encoded bits = 224, expected length = 2.2400 bits/symbol
```
