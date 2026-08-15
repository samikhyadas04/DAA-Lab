"""
Lab 03 - Question 1: Binary vs Ternary Search
Plots average and worst-case comparisons vs n for both algorithms.
"""
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("benchmark.csv")

fig, axes = plt.subplots(1, 2, figsize=(13, 5.5))

# --- Average comparisons ---
ax = axes[0]
ax.plot(df["n"], df["avg_binary"], marker='o', label="Binary Search (avg)", color="#1f77b4")
ax.plot(df["n"], df["avg_ternary"], marker='s', label="Ternary Search (avg)", color="#d62728")
ax.set_xscale("log")
ax.set_xlabel("n (array size, log scale)")
ax.set_ylabel("Average number of key comparisons")
ax.set_title("Average Comparisons: Binary vs Ternary Search")
ax.legend()
ax.grid(True, alpha=0.3)

# --- Worst-case comparisons ---
ax = axes[1]
ax.plot(df["n"], df["worst_binary"], marker='o', label="Binary Search (worst)", color="#1f77b4")
ax.plot(df["n"], df["worst_ternary"], marker='s', label="Ternary Search (worst)", color="#d62728")
ax.set_xscale("log")
ax.set_xlabel("n (array size, log scale)")
ax.set_ylabel("Worst-case number of key comparisons")
ax.set_title("Worst-Case Comparisons: Binary vs Ternary Search")
ax.legend()
ax.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig("plot.png", dpi=150)
print("Saved plot.png")
