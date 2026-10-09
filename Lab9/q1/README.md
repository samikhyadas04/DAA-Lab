# Q1 – Fractional Knapsack with Deterioration Rate

## Problem model (assumptions made explicit)
* Items `(v_i, w_i, λ_i)`, density `d_i = v_i / w_i`, capacity `W`.
* Weight is consumed at **1 unit per unit time**, so after `t` units have been consumed the clock is `t`.
* A unit of item `i` consumed at time `t` is worth `d_i − λ_i·t`. We choose amounts `x_i ∈ [0, w_i]`, `Σx_i ≤ W`, and an order. Consuming never has to continue once density is negative, so we simply may stop (idle time at the end).

## Input representation
`n W` followed by `n` lines `v_i w_i λ_i` (array of structs `{v, w, λ, d, id}`).

## Algorithm
1. **Order (pure exchange argument).** Take two adjacent blocks `i, j` with amounts `x_i, x_j`.
   If `i` goes first, `j` starts `x_i` later and loses `λ_j·x_j·x_i`; if `j` goes first, the loss is `λ_i·x_i·x_j`.
   So `i` should go first iff `λ_i ≥ λ_j` – **independent of the amounts**. Optimal schedule = sort by **λ descending**, each chosen item consumed in one contiguous block. (Steeper-decaying items must be consumed first.)
2. **Amounts.** With that order, total value is
   `f(x) = Σ d_i x_i − ½ Σ_ij min(λ_i, λ_j) x_i x_j`.
   The matrix `min(λ_i,λ_j)` is positive semidefinite, so `f` is **concave**; the feasible set is a polytope, so a local optimum is global. We solve it with pairwise mass transfers (SMO style): move mass `s` from item `j` to item `i` with the exact 1-D optimal `s = (g_i − g_j)/|λ_i − λ_j|` clipped to the bounds, until no transfer gains more than 1e-13. A dummy "idle" item (`d=0, λ=0, w=W`) soaks up unused capacity so the capacity constraint is an equality.
3. Output order, start times, fractions and value.

> **Important finding:** the "obvious" greedy (always consume the item with the best *current* density `d_i − λ_i t`) is **not optimal**. The self-test shows it strictly loses to the algorithm above in 93 of 300 random instances, because it ignores that a steeply decaying item must be used early even if it is not the best right now.

## Complexity
| | |
|---|---|
| Sorting by λ | `O(n log n)` |
| Amount solver | one sweep = `O(n²)` pair checks, each *accepted* transfer costs `O(n)` to update gradients. The method converges linearly; in practice a few dozen sweeps (≈ `O(log 1/ε)`) are needed, so ≈ `O(S·n²)`–`O(S·n³)` worst case with `S` sweeps |
| **Space** | `O(n)` (arrays `x, g, d, λ, w`) |

(The problem can be made separable in prefix sums `S_k`, which admits an isotonic-regression style `O(n²)`/`O(n log n)` exact method; the SMO version was chosen because it is short and easy to validate.)

## Validation
`./q1 --selftest` compares against a brute force (all `n!` orders × a 41-level grid of amounts, n ≤ 3) on 300 random instances: **0 failures** (the algorithm is never worse than brute force), and the naive greedy is never better.

## Build & run
```
gcc -O2 -o q1 solution.c -lm
./q1 < sample_input.txt
./q1 --selftest
```
Sample (`4 10 / 60 6 2.0 / 40 5 0.5 / 30 3 3.0 / 24 4 0.1`):
```
item   start t    amount     fraction   value
1      0.0000     1.3333     0.2222     11.5556
2      1.3333     4.6667     0.9333     28.7778
4      6.0000     4.0000     1.0000     20.8000
Total value (optimal)        = 61.133333
Total value (naive greedy)   = 59.680000
```
