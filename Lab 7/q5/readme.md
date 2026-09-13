# Shooter vs. a Moving Target on a Line


## 2. Answer: yes, such an algorithm exists

### 2.1 The algorithm

Fire at the following sequence of spots, in order:

```
2, 3, 4, …, n-1, n,  n, n-1, …, 3, 2
└──────  up-sweep  ──────┘ └── down-sweep ──┘
```

That is: sweep once from spot `2` up to spot `n`, then sweep back down from
`n` to `2`. This is a fixed, non-adaptive sequence of `2n − 2` shots.

### 2.2 Why it works

Let `x_t` be the target's (unknown) position immediately before shot number
`t` (`t = 1, 2, 3, …`). Because the target must move to an adjacent spot
between shots, `x_{t+1} = x_t ± 1`, so the parity of `x_t + t` never
changes:

```
(x_{t+1} + (t+1)) − (x_t + t) = (x_{t+1} − x_t) + 1 ∈ {0, 2}   (always even)
```

Hence the parity of `x_1 + 1` (fixed by the target's unknown starting spot)
determines the parity of `x_t + t` for *every* future `t`. There are only
two possibilities ("types"):

* **Type A** (`x_1` odd): `x_t` has the *same* parity as `t`, for all `t`.
* **Type B** (`x_1` even): `x_t` has the *opposite* parity of `t`, for all `t`.

**Up-sweep catches Type B.** During the up-sweep, shot `t` (for
`t = 1, …, n−1`) is fired at spot `t + 1`, which has the opposite parity of
`t` — exactly matching Type B. Define `g_t = x_t − (t+1)`. Since
`x_{t+1} − x_t ∈ {+1,−1}`, we get `g_{t+1} − g_t ∈ {0, −2}`: `g_t` is
non-increasing and changes by an even amount. For a Type‑B target,
`x_1 ≥ 2` so `g_1 ≥ 0`, while at the last up-sweep shot `x_{n-1} ≤ n` gives
`g_{n-1} ≤ 0`. A quantity that starts `≥ 0`, ends `≤ 0`, and only ever moves
by steps of `0` or `−2` **cannot skip over 0** — so `g_t = 0` for some `t`,
i.e. the target is hit.

**Down-sweep catches Type A.** Symmetrically, the down-sweep shot at global
time `t` (`t = n, …, 2n−2`) is fired at spot `2n − t`, which has the *same*
parity as `t` — matching Type A. Defining `h_t = x_t − (2n − t)`, the same
monovariant argument (this time non-decreasing, from `≤ 0` up to `≥ 0`)
shows `h_t = 0` for some `t` in this phase.

Since every possible target is either Type A or Type B, and each type is
provably caught by one of the two sweeps, **the target is always hit** —
this is a complete existence proof, not just a plausibility argument.

*(The sequence is not claimed to be the shortest possible; the problem only
asks for a guaranteed-hit algorithm, which this is, with a clean
closed-form proof. A shorter `2n−3`-shot sequence is also known, at the
cost of a slightly fiddlier proof.)*

## 3. Complexity

* **Algorithm itself:** the firing sequence has `2n − 2` shots, each an
  `O(1)` decision (just read the next entry of a precomputed array of size
  `O(n)`). Total: **`O(n)` time, `O(n)` space.**
* **Validator** (below): exhaustive, not part of the algorithm — see §5.

## 4. Input representation

The program takes a single command-line integer `MAXN` (default 14): it
validates the algorithm for every `n = 2, …, MAXN`. No other input is
needed since the "input" to the algorithm itself is just `n`; the target's
behaviour is the adversary the validator searches over.

## 5. Validation program (`shooter.c`)

For each `n` the program:
1. Builds the firing sequence described above.
2. Runs an **exhaustive adversarial search** (`adv()`): the adversary picks
   the target's starting spot and then, at every step, whichever legal
   adjacent move keeps it alive longest. If *any* full trajectory survives
   every shot, that is a counter-example and is printed with `FAIL`.
3. If no trajectory survives, prints `PASS` together with the sequence
   used.

This is a full game-tree search over the adversary's strategy space
(bounded by pruning any branch the instant it is hit), so a `PASS` is a
genuine proof of correctness for that `n`, not a spot-check.

### Complexity of the validator

The target has up to 2 legal moves per step and the sequence has
`M = 2n − 2` steps, so the naive search space is `O(2^M)`; branches are
pruned the moment they are hit, which keeps it fast for the `n` tested here
(runs for `n` up to 15 finish in well under a second). This exponential
cost is a property of the *validator* (exhaustively checking every possible
adversary strategy), not of the algorithm itself, which is linear.

```