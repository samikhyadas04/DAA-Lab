"""
Lab 03 - Question 3: Max and Min using D&C
Plots D&C comparisons vs linear-scan comparisons vs the 3n/2 bound.
"""
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("benchmark.csv")

plt.figure(figsize=(8, 5.5))
plt.plot(df["n"], df["dc_comparisons"], marker='o', label="D&C Max-Min (actual)", color="#1f77b4")
plt.plot(df["n"], df["linear_comparisons"], marker='s', label="Naive linear scan (2n-2)", color="#d62728")
plt.plot(df["n"], df["bound_3n_2"], linestyle="--", label="3n/2 bound", color="gray")
plt.xscale("log")
plt.yscale("log")
plt.xlabel("n (array size, log scale)")
plt.ylabel("Number of comparisons (log scale)")
plt.title("Max-Min via Divide & Conquer vs Naive Linear Scan")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("plot.png", dpi=150)
print("Saved plot.png")
