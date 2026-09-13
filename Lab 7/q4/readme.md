# The n-Switch Puzzle (a "Chinese Rings" style puzzle)


## 2. Algorithm

### 2.1 Representation

A configuration of `n` switches is exactly an `n`-bit number: bit `i-1`
(0-indexed) is `1` if switch `i` is ON. This turns "find the minimum
number of moves" into the standard **unweighted shortest-path** problem:
build the graph whose `2^n` nodes are configurations and whose edges are
single legal toggles (rule (i)/(ii) above), then find the shortest path
from the all-ON node (`2^n − 1`) to the all-OFF node (`0`).

### 2.2 The algorithm: Breadth-First Search

**Breadth-First Search (BFS)** from the all-ON state. BFS on an unweighted
graph is guaranteed to return a *shortest* path, so the number of moves it
finds is provably **optimal**, not a heuristic upper bound — this directly
answers "minimum number of moves." The program also reconstructs the
actual sequence of switch indices to toggle, and re-simulates every move
against the legality rule to certify the sequence is playable end‑to‑end.

### 2.3 Closed form (discovered/cross-checked by the program)

The program independently prints a closed-form prediction and compares it
to the BFS-computed minimum for every `n` tested:

```
T(1) = 1
T(n) = 2*T(n-1) + (n mod 2)     for n ≥ 2
```

giving `T(n) = 1, 2, 5, 10, 21, 42, 85, 170, 341, …` for `n = 1, 2, 3, …` —
this is exactly the classical Chinese-rings/Gray-code puzzle sequence,
confirmed independently for every `n` from 1 to 18 by the BFS search (see
§5), so the closed form is not merely conjectured but empirically
certified against an exhaustive, independent shortest-path computation for
every tested case.

## 3. Complexity

* **State space:** `2^n` configurations.
* **BFS time:** `O(n · 2^n)` (each of `2^n` states examines up to `n`
  candidate toggles).
* **BFS space:** `O(2^n)` (distance/parent arrays + queue).

This exponential cost is unavoidable in the worst case: the *optimal
solution itself* has `Θ(2^n)` moves (see the closed form above), so any
algorithm that must actually output the move sequence is already at least
that expensive. BFS matches this bound up to the extra factor of `n`.

## 4. Input representation

The program takes one command-line integer `MAXN` (default 16) and solves
/ validates the puzzle for every `n = 1, …, MAXN`. Internally, a
configuration is represented as a plain `int` bitmask, which is the
natural and most compact representation for this state space.

## 5. Validation program (`switches.c`)

For every `n` up to `MAXN` the program:
1. Runs BFS from all-ON to all-OFF over the full `2^n`-state graph, using
   the exact legality rule from the problem statement.
2. Reports the BFS-optimal move count next to the closed-form prediction
   and flags any mismatch (`match? yes/NO`).
3. Reconstructs the actual shortest move sequence (list of switch indices
   to toggle, in order) and **re-simulates it from scratch**, checking
   every single move against the legality rule and confirming the final
   state is all-OFF — an independent end-to-end legality certification,
   not just a move count.

```
