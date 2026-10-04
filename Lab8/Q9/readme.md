# Q09 - Collatz Conjecture (Open Unsolved Problem)

## Question Title

The Collatz Conjecture (also known as the 3n+1 problem or Ulam conjecture) defines a recurrence relation for any strictly positive integer `n`:

```
T(n) = n/2       if n is even
T(n) = 3n + 1    if n is odd
```

The sequence repeatedly applies this function until `n = 1`. Write a modular C program to analyse the trajectory of a user-provided starting value `n ≥ 1` and across an interval `[a, b]`.

## How the Code Works

The program has two modes — **single-value trajectory** and **interval `[a,b]` analysis** — each supporting manual or random input. `collatzStep()` applies one step with unsigned 64-bit arithmetic and overflow detection. `singleTrajectory()` dynamically grows a buffer (doubling with `realloc`) to store the full path to 1, then prints every step with the operation applied (÷2 or ×3+1). `analyseInterval()` runs the trajectory for each `n` in `[a,b]` in O(1) space, reporting per-value stopping times, maximum values reached, and overall interval statistics.

## Maths / Logic Behind This

**Recurrence:**
```
T(n) = n/2      (n even)
T(n) = 3n + 1   (n odd)
```

**Stopping time** (also called *total stopping time*): the number of steps for `n` to reach 1. Denoted `σ(n)`.

**Conjecture:** For all positive integers `n`, iterating `T` will eventually reach 1. Despite extensive computational verification (all `n` up to ~2⁶⁸ have been checked), the conjecture remains **unproven**.

**Why it's hard:** Each odd step multiplies by ~3 (growth) and each even step halves (decay). The sequence has no obvious monotone behaviour, and the ratio of odd to even steps varies unpredictably. Connections to number theory, dynamical systems, and computability theory make a general proof elusive.

**Overflow handling:** `3n+1` can overflow 64-bit integers for large `n`; the program checks `n > (ULLONG_MAX - 1) / 3` before the multiplication and reports an overflow error if triggered.

## Complexity Analysis

- **Time complexity:** O(σ(n)) per starting value, where σ(n) is the stopping time. No closed form is known; empirically O(log n) on average. For interval [a,b]: O((b−a+1) × max σ(n))
- **Extra space complexity:** O(σ(n)) for single-trajectory storage (dynamic array); O(1) per value for interval analysis