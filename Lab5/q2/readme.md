# Quick Sort on N Random Elements (File-Based) — C Program

This program generates **N random integers**, stores them in an input file,
reads them back, sorts them using the **Quick Sort** algorithm, and writes
the sorted result to an output file.




## Compilation

```bash
gcc -o quicksort quicksort.c
```

## Usage

```bash
./quicksort <N> [max_value]
```

- `N` — number of random elements to generate (required)
- `max_value` — upper bound for random numbers, inclusive (optional, default `1000`)

### Examples

```bash
./quicksort 20            # 20 random numbers between 0 and 1000
./quicksort 50 500        # 50 random numbers between 0 and 500
./quicksort 100000 1000000  # stress test with 100,000 elements
```

## Algorithm: Quick Sort

Quick Sort is a divide-and-conquer algorithm:

1. Pick a **pivot** element (here, the last element of the sub-array).
2. **Partition** the array so all elements smaller than the pivot come
   before it, and all larger elements come after it.
3. **Recursively** apply the same process to the sub-arrays on either
   side of the pivot.

**Time Complexity**
- Best / Average case: `O(n log n)`
- Worst case: `O(n²)` (rare, occurs on already-sorted or adversarial input
  with this pivot strategy)

**Space Complexity**: `O(log n)` average (recursion stack); the array is
sorted in-place.

## Sample Output

```
Generated 20 random numbers and saved to 'input.txt'

Unsorted array (read from file):
300 117 264 77 391 343 355 327 403 238 58 35 152 227 369 460 428 448 65 303

Sorted array:
35 58 65 77 117 152 227 238 264 300 303 327 343 355 369 391 403 428 448 460

Time taken to sort: 0.000003 seconds
Sorted array written to 'output.txt'
```

## Notes

- Random numbers are seeded with the current time, so each run produces a
  different set of numbers.
- The program dynamically allocates memory for the array based on the
  actual count of numbers found in `input.txt`, so it will also work if you
  manually edit `input.txt` and re-run with matching logic.
- For very large `N`, sorting still completes quickly since Quick Sort
  averages `O(n log n)`.