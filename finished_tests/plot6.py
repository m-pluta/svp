import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
DIR = 'finished_tests/'

# Load the CSV files
df = pd.read_csv(f"{DIR}LLLSE_99_double.csv")
df = df[df["dimension"] <= 40]
df["lattice_type"] = df["lattice_type"].replace({"u": "Uniform", "r": "Knapsack"})

# Create a 2x2 subplot
fig, ax = plt.subplots(1, 1, figsize=(6, 5))

# Line plot of byte_at_tgmax against dimension
sns.lineplot(data=df, x="dimension", y="bytes_at_tgmax", ax=ax)

# Set plot title and labels
ax.set_title("Peak Memory Usage vs Dimension")
ax.set_xlabel("Dimension")
ax.set_ylabel("Bytes")



plt.tight_layout()

plt.savefig(f"report/total_bytes_dimension.png")
plt.show()
