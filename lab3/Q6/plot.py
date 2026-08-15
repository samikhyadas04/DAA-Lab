"""
Lab 03 - Question 6: Loop Invariants in Selection Sort
Plots comparisons vs n for random/sorted/reverse input, all overlapping
with the theoretical n(n-1)/2 curve.
"""
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("benchmark.csv")

plt.figure(figsize=(8, 5.5))
plt.plot(df["n"], df["comparisons_random"], marker='o', label="Random input", color="#1f77b4")
plt.plot(df["n"], df["comparisons_sorted"], marker='^', label="Already sorted", linestyle="--", color="#2ca02c")
plt.plot(df["n"], df["comparisons_reverse"], marker='v', label="Reverse sorted", linestyle=":", color="#d62728")
plt.plot(df["n"], df["n_n_minus_1_over_2"], linestyle="-.", label="n(n-1)/2 (theory)", color="gray")

plt.xlabel("n (array size)")
plt.ylabel("Number of comparisons")
plt.title("Selection Sort: Comparisons vs n\n(Best case = Worst case = Theta(n^2))")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("plot.png", dpi=150)
print("Saved plot.png")
