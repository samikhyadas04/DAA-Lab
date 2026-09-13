
## Algorithms implemented

### 1. Classic DP — `minTrialsClassicDP(E, F)`

State: `dp[e][f]` = minimum drops needed with `e` eggs and `f` floors.

```
dp[1][f] = f
dp[e][0] = 0,  dp[e][1] = 1
dp[e][f] = 1 + min over x in [1..f] of max( dp[e-1][x-1],  // egg breaks at x
                                             dp[e][f-x] )   // egg survives x
```

The inner minimum is found with **binary search** instead of a linear scan:
`dp[e-1][x-1]` increases with `x` and `dp[e][f-x]` decreases with `x`, so
their max is unimodal and the crossover point can be binary-searched.

- Time: `O(E · F · log F)`
- Space: `O(E · F)`

### 2. Fast DP — `minTrialsFastDP(E, F)` (preferred)

State: `f(e,k)` = maximum number of floors distinguishable using `e` eggs
and `k` drops.

```
f(e,k) = f(e-1,k-1) + f(e,k-1) + 1
```

Increase `k` until `f(E,k) >= F`; that `k` is the answer. Equivalently,
`f(e,k) = Σ_{i=1}^{e} C(k,i)`, which generalizes the triangular-number
argument used for the classic 2-egg case.

- Time: `O(E · K)`, where `K` is the answer itself (`K = O(F^{1/E})`,
  collapsing to `O(log F)` once `E ≥ log₂ F`)
- Space: `O(E)` (rolling array over `k`, updated in place)

Both functions are run against each other in `main()` for a battery of
`(E, F)` pairs as a correctness check.

## Build & run

```bash
gcc -O2 -Wall -o egg_drop egg_drop.c
```

Run the built-in validation suite (no arguments):

```bash
./egg_drop
```

Run a single case:

```bash
./egg_drop <eggs> <floors>
# e.g.
./egg_drop 2 100
# Eggs=2 Floors=100 -> ClassicDP=14  FastDP=14  (match)
```

## Complexity summary

| Approach            | Time                  | Space     |
|---------------------|-----------------------|-----------|
| Naive recursion      | exponential, ~`O(F^E)`| `O(E)`    |
| Classic DP (binary search inner loop) | `O(E·F·log F)` | `O(E·F)` |
| Fast DP (coverage form) | `O(E·K)`, `K = O(F^{1/E})` | `O(E)` |

For large `F` and moderate `E`, the fast DP is dramatically cheaper and is
the one worth using in practice; the classic DP is kept as a correctness
oracle.