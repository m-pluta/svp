import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

DIR = 'finished_tests/'

# Load the CSV files
file_paths = {
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
fig, axs = plt.subplots(1, 2, figsize=(12, 6))

titles = ["Delta=0.75",
          "Delta=0.99"]

# Plotting
for i, (name, df) in enumerate(dataframes.items()):
    ax = axs[i]
    sns.scatterplot(data=df, x="dimension", y=np.log10(
        df["median"]*1000), hue="lattice_type", ax=ax)
    ax.set_title(titles[i])
    ax.set_xlabel("Dimension")
    ax.set_ylabel("Log(Median Time in ms)")
    ax.set_ylim([-1, 5])
    ax.legend(title='Lattice Type', loc='upper left')

plt.tight_layout()

mean_times = [df["mean"].mean() for df in dataframes.values()]
print(mean_times)

plt.savefig(f"report/time_dimension_delta.png")
plt.show()


means = [[], [], [], []]
for i, name in enumerate(["LLLSE_75_double", "LLLSE_99_double"]):
    for j, type in enumerate(['Uniform', 'Knapsack']):
        df = dataframes[name]
        df = df[df['lattice_type'] == type]
        mean = df.groupby("dimension")["median"].mean()
        means[2 * i + j].append(mean)

# Output means to a file
for i, mean in enumerate(means):
    df = pd.DataFrame(mean)
    df = df.transpose()
    
means = [list(mean) for mean in means]

print(means)


# Output means to an Excel file
excel_file_path = "report/means.xlsx"
with pd.ExcelWriter(excel_file_path) as writer:
    for i, mean in enumerate(means):
        df = pd.DataFrame(mean)
        df = df.transpose()
        df.to_excel(writer, sheet_name=f"Mean {i+1}")


