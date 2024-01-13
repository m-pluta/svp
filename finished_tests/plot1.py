import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

DIR = 'finished_tests/'

# Load the CSV files
file_paths = {
    "Non-mem_75_LLLSE": f"{DIR}Non-mem_75_LLLSE.csv",
    "Non-mem_99_LLLSE": f"{DIR}Non-mem_99_LLLSE.csv",
    "LLLSE_75_double":  f"{DIR}LLLSE_75_double.csv",
    "LLLSE_99_double":  f"{DIR}LLLSE_99_double.csv"}
dataframes = {name: pd.read_csv(path) for name, path in file_paths.items()}

# Filter dataframes
for name, df in dataframes.items():
    df = df[df["dimension"] <= 40]
    df["lattice_type"] = df["lattice_type"].replace(
        {"u": "Uniform", "r": "Knapsack"})
    dataframes[name] = df

# Create a 2x2 subplot
fig, axs = plt.subplots(2, 2, figsize=(12, 10))

titles = ["LLL + SE, Non-memoised, Delta=0.75",
          "LLL + SE, Non-memoised, Delta=0.99",
          "LLL + SE, Memoised, Delta=0.75",
          "LLL + SE, Memoised, Delta=0.99"]

# Plotting
for i, (name, df) in enumerate(dataframes.items()):
    ax = axs[i // 2, i % 2]
    sns.scatterplot(data=df, x="dimension", y=np.log10(
        df["mean"]*1000), hue="lattice_type", ax=ax)
    ax.set_title(titles[i])
    ax.set_xlabel("Dimension")
    ax.set_ylabel("Log(Mean Time in ms)")
    ax.set_ylim([-1, 5])
    ax.legend(title='Lattice Type', loc='upper left')

plt.tight_layout()

plt.savefig(f"{DIR}time_dimension_delta_mem.png")
plt.show()

mean_times = [df["mean"].mean() for df in dataframes.values()]
print(mean_times)
