import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import seaborn as sns

DIR = 'finished_tests/'

df_double = pd.read_csv(f"{DIR}LLLSE_99_double.csv")
df_long_double = pd.read_csv(f"{DIR}LLLSE_99_long_double.csv")


# Calculate max absolute difference per dimension for each dataframe
mad_double = df_double.groupby('dimension').apply(
    lambda x: (x['expected_answer'] - x['actual_answer']).abs().max())
mad_long_double = df_long_double.groupby('dimension').apply(
    lambda x: (x['expected_answer'] - x['actual_answer']).abs().max())

# Create subplots
fig, axes = plt.subplots(nrows=1, ncols=2, figsize=(12, 6))

# Plot for double precision
axes[0].plot(mad_double.index, mad_double.values, marker='o', color='cornflowerblue')
axes[0].set_xlabel('Dimension')
axes[0].set_ylabel('Max Absolute Difference')
axes[0].set_title('Double')
axes[0].set_ylim(0, 3.5e-5)
axes[0].grid(True)

# Plot for long double precision
axes[1].plot(mad_long_double.index, mad_long_double.values,
             marker='o', color='orange')
axes[1].set_xlabel('Dimension')
axes[1].set_ylabel('Max Absolute Difference')
axes[1].set_title('Long double')
axes[1].set_ylim(0, 3.5e-5)
axes[1].grid(True)

plt.savefig(f"report/max_absolute_difference_plot.png")
plt.tight_layout()
plt.show()
