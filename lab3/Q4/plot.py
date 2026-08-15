"""
Lab 03 - Question 4: Strassen's Matrix Multiplication
Plots naive O(n^3) vs Strassen O(n^2.807) timings.
"""
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("benchmark.csv")

plt.figure(figsize=(8, 5.5))
plt.plot(df["n"], df["naive_time_sec"], marker='o', label="Naive O(n^3)", color="#d62728")
plt.plot(df["n"], df["strassen_time_sec"], marker='s', label="Strassen O(n^2.807)", color="#1f77b4")
plt.xscale("log", base=2)
plt.yscale("log")
plt.xlabel("n (matrix dimension, log2 scale)")
plt.ylabel("Time (seconds, log scale)")
plt.title("Matrix Multiplication: Naive vs Strassen's D&C Method")
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("plot.png", dpi=150)
print("Saved plot.png")
