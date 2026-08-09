import pandas as pd
import matplotlib.pyplot as plt

try:
    df = pd.read_csv('results.csv')
except FileNotFoundError:
    print("Error: 'results.csv' not found. Please run the C program first.")
    exit(1)

operations = [
    'Time_Insert', 'Time_Search', 'Time_Max', 
    'Time_Min', 'Time_Pred', 'Time_Succ', 'Time_Delete'
]
op_titles = [
    'Insertion', 'Search', 'Maximum', 
    'Minimum', 'Predecessor', 'Successor', 'Deletion'
]

# Mapping the theoretical worst-case time complexities for each operation
complexities = {
    'Time_Insert': {
        'Unsorted_Array': 'O(1)', 'Sorted_Array': 'O(N)',
        'SLL_Unsorted': 'O(1)', 'SLL_Sorted': 'O(N)',
        'DLL_Unsorted': 'O(1)', 'DLL_Sorted': 'O(N)'
    },
    'Time_Search': {
        'Unsorted_Array': 'O(N)', 'Sorted_Array': 'O(log N)',
        'SLL_Unsorted': 'O(N)', 'SLL_Sorted': 'O(N)',
        'DLL_Unsorted': 'O(N)', 'DLL_Sorted': 'O(N)'
    },
    'Time_Max': {
        'Unsorted_Array': 'O(N)', 'Sorted_Array': 'O(1)',
        'SLL_Unsorted': 'O(N)', 'SLL_Sorted': 'O(N)',
        'DLL_Unsorted': 'O(N)', 'DLL_Sorted': 'O(1)'
    },
    'Time_Min': {
        'Unsorted_Array': 'O(N)', 'Sorted_Array': 'O(1)',
        'SLL_Unsorted': 'O(N)', 'SLL_Sorted': 'O(1)',
        'DLL_Unsorted': 'O(N)', 'DLL_Sorted': 'O(1)'
    },
    'Time_Pred': {
        'Unsorted_Array': 'O(N)', 'Sorted_Array': 'O(1)',
        'SLL_Unsorted': 'O(N)', 'SLL_Sorted': 'O(N)',
        'DLL_Unsorted': 'O(N)', 'DLL_Sorted': 'O(1)'
    },
    'Time_Succ': {
        'Unsorted_Array': 'O(N)', 'Sorted_Array': 'O(1)',
        'SLL_Unsorted': 'O(N)', 'SLL_Sorted': 'O(1)',
        'DLL_Unsorted': 'O(N)', 'DLL_Sorted': 'O(1)'
    },
    'Time_Delete': {
        'Unsorted_Array': 'O(1)', 'Sorted_Array': 'O(N)',
        'SLL_Unsorted': 'O(N)', 'SLL_Sorted': 'O(N)',
        'DLL_Unsorted': 'O(1)', 'DLL_Sorted': 'O(1)'
    }
}

data_structures = df['Data_Structure'].unique()
markers = ['o', 's', '^', 'D', 'v', 'p']
colors = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd', '#8c564b']

fig, axes = plt.subplots(nrows=2, ncols=4, figsize=(20, 10))
axes = axes.flatten()

for i, op in enumerate(operations):
    ax = axes[i]
    
    for j, ds in enumerate(data_structures):
        subset = df[df['Data_Structure'] == ds]
        
        # Append the complexity to the label
        complexity_label = complexities[op].get(ds, 'O(?)')
        display_name = ds.replace('_', ' ')
        legend_label = f"{display_name} - {complexity_label}"
        
        ax.plot(subset['N'], subset[op], 
                label=legend_label, 
                marker=markers[j], 
                color=colors[j],
                linewidth=2, 
                markersize=6)
    
    ax.set_title(f'Order of Growth: {op_titles[i]} Time', fontsize=12, fontweight='bold')
    ax.set_xlabel('Number of Elements (N)', fontsize=10)
    ax.set_ylabel('Execution Time (seconds)', fontsize=10)
    ax.grid(True, linestyle='--', alpha=0.6)
    ax.legend(fontsize=8, loc='upper left')

axes[7].axis('off')

plt.tight_layout()
plt.savefig('q1_plot_results.png', dpi=300)
print("Plot successfully generated and saved as 'q1_plot_results.png'.")
plt.show()