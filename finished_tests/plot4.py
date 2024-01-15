import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
import pandas as pd

DIR = 'finished_tests/'

# Load the CSV files
file_paths = {
    "Non-mem_99_LLLSE": f"{DIR}Non-mem_99_LLLSE.csv",
    "LLLSE_99_double":  f"{DIR}LLLSE_99_double.csv"}
dataframes = {name: pd.read_csv(path) for name, path in file_paths.items()}

# Filter dataframes
for name, df in dataframes.items():
    df = df[df["dimension"] <= 40]
    df["lattice_type"] = df["lattice_type"].replace(
        {"u": "Uniform", "r": "Knapsack"})
    dataframes[name] = df

# Create a 2x2 subplot
fig, axs = plt.subplots(1, 2, figsize=(12, 6))

titles = ["LLL + SE, Uniform lattices",
          "LLL + SE, Delta=0.99, Knapsack lattices"]

# Plotting
for i, (name, df) in enumerate(dataframes.items()):
    ax = axs[0]
    sns.scatterplot(data=df[df['lattice_type'] == 'Uniform'], x="dimension", y=np.log10(
        df["median"]*1000), ax=ax, alpha=0.4)
    ax.set_title(titles[0])
    ax.set_xlabel("Dimension")
    ax.set_ylabel("log(Median Time in ms)")
    ax.legend(labels = ['Non-memoised', 'Memoised'])
    ax.set_ylim([-1, 3])

# Plotting
for i, (name, df) in enumerate(dataframes.items()):
    ax = axs[1]
    sns.scatterplot(data=df[df['lattice_type'] == 'Knapsack'], x="dimension", y=np.log10(
        df["median"]*1000), ax=ax, alpha=0.4)
    ax.set_title(titles[1])
    ax.set_xlabel("Dimension")
    ax.set_ylabel("log(Median Time in ms)")
    ax.legend(labels = ['Non-memoised', 'Memoised'])
    ax.set_ylim([-1, 3])


