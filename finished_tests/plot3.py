import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import seaborn as sns

DIR = 'finished_tests/'

dfs = [
    ['LLL Delta=0.75', 'upper right', pd.read_csv(f"{DIR}LLL_75_double.csv")],
    ['LLL Delta=0.99', 'lower left', pd.read_csv(f"{DIR}LLL_99_double.csv")],
    ['LLL + SE Delta=0.75', 'lower left', pd.read_csv(f"{DIR}LLLSE_75_double.csv")],
    ['LLL + SE Delta=0.99', 'lower left', pd.read_csv(f"{DIR}LLLSE_99_double.csv")],
    ['SE', 'center left', pd.read_csv(f"{DIR}SE_double.csv")]]

for (title, pos, df) in dfs:
    df['actual_answer'] = pd.to_numeric(df['actual_answer'], errors='coerce')
    df['lattice_type'] = df['lattice_type'].replace({'u': 'Uniform', 'r': 'Knapsack'})
    df['accurate'] = (df['expected_answer'].sub(df['actual_answer']).abs() < 0.0005)
    
    
# Create a 2x3 subplots figure
fig, axes = plt.subplots(2, 3, figsize=(16, 9))

for i, (title, pos, df)  in enumerate(dfs):
    if i != 5:  # Skip axes[5]
        ax = axes[i % 2, i // 2]
        
        sns.lineplot(data=df, x='dimension', y='accurate', hue='lattice_type', ax=ax)
        ax.set_title(f"{title}")
        ax.set_xlabel("Dimension")
        ax.set_ylim(0 - 0.05, 1 + 0.05)
        ax.set_ylabel("Accuracy")
        ax.legend(title='Lattice Type', loc=pos)
    
# Hide the sixth (empty) subplot
axes[1, 2].set_visible(False)

# Add a gap between the two rows in the subplots
plt.subplots_adjust(hspace=0.4)

plt.savefig(f"{DIR}algorithms_comparison.png")

plt.show()
