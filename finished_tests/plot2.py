import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import seaborn as sns

DIR = 'finished_tests/'

df_double = pd.read_csv(f"{DIR}LLLSE_99_double.csv")
df_long_double = pd.read_csv(f"{DIR}LLLSE_99_long_double.csv")


# Calculate mean absolute difference per dimension for each dataframe
mad_double = df_double.groupby('dimension').apply(
    lambda x: (x['expected_answer'] - x['actual_answer']).abs().mean())
mad_long_double = df_long_double.groupby('dimension').apply(
    lambda x: (x['expected_answer'] - x['actual_answer']).abs().mean())

print(mad_double)

# Create subplots
fig, axes = plt.subplots(nrows=1, ncols=2, figsize=(15, 6))
fig.suptitle('Mean Absolute Difference per dimension')

# Plot for double precision
axes[0].plot(mad_double.index, mad_double.values, marker='o', color='blue')
axes[0].set_xlabel('Dimension')
axes[0].set_ylabel('Mean Absolute Difference')
axes[0].set_title('LLL + SE with Double Precision')
axes[0].set_ylim(0, 4e-6)
axes[0].grid(True)

# Plot for long double precision
axes[1].plot(mad_long_double.index, mad_long_double.values,
             marker='o', color='orange')
axes[1].set_xlabel('Dimension')
axes[1].set_ylabel('Mean Absolute Difference')
axes[1].set_title('LLL + SE with Long Double Precision')
axes[1].set_ylim(0, 4e-6)
axes[1].grid(True)


plt.tight_layout()

plt.savefig(f"{DIR}mean_absolute_difference_plot.png")
plt.show()
