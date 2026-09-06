# DAA Lab-6, Q1: Fibonacci (DP) — Time & Space Complexity

## Time Complexity
The algorithm fills the dp array once from index 2 to n, doing O(1) work per index. Hence the time complexity is **O(n)**.

## Space Complexity
A dp array of size (n+1) is used to store all Fibonacci values up to n, giving a space complexity of **O(n)**. (This can be optimized to O(1) by keeping only the last two values, but the tabulation version above uses O(n) space.)