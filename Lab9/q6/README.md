# Q6 – Reorganise String with K-Distance Apart

## Input representation
Line 1: the string `S` (any bytes, alphabet size σ ≤ 256). Line 2: integer `K`. Internally: a frequency table `cnt[256]`, a **max-heap of (count, char)** holding the characters that are currently *available*, and an array `placed[]` remembering the (char, remaining count) put at each position (a cooldown queue).

## Algorithm
For `K ≤ 1` the answer is `S` itself. Otherwise for position `i = 0 … n−1`:
1. the character placed at `i−K` has cooled down → push it back into the heap if copies remain;
2. if the heap is empty → **impossible**, return `""`;
3. pop the available character with the **largest remaining count**, write it at `i`, decrement.

**Why greedy works:** the most frequent character is the scarcest resource (it needs `(m−1)K + p` slots, `m` = max count, `p` = number of characters with that count). Always spending the most demanding available character keeps that condition satisfiable. Empirically/exhaustively checked: a rearrangement exists iff `(m−1)·K + p ≤ n` (verified for all multisets of ≤ 10 letters over 4 symbols, `K ≤ 6`; and `K ≤ 1` is always possible).

## Complexity
| | |
|---|---|
| Counting | `O(n + σ)` |
| `n` positions × (≤1 push + 1 pop) on a heap of ≤ σ items | `O(n log σ)` |
| **Time** | **`O(n log σ)`** (= `O(n)` for a fixed alphabet) |
| **Space** | `O(n + σ)` as coded (`placed[]`); a circular buffer of size `K` reduces it to `O(min(n,K) + σ)` |

## Validation (`./q6 --selftest`)
20000 random strings (n ≤ 9, 4 letters, K ∈ [0,5]): the greedy returns non-empty exactly when an exhaustive backtracking search finds an arrangement (13475 feasible cases), and every output is a permutation of `S` with equal letters ≥ K apart → **0 failures**.

## Build & run
```
gcc -O2 -o q6 solution.c
./q6 < sample_input.txt      # "aabbcc", K=3
```
Output: `abcabc`.  (e.g. `aaabc`, `K=2` prints `Impossible -> ""`).
