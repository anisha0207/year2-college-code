import pandas as pd  # Import Pandas for data manipulation and analysis
import matplotlib.pyplot as plt  # Import Matplotlib for plotting
import seaborn as sns  # Import Seaborn for advanced visualizations

# Load the data from the CSV file into a Pandas DataFrame
# The CSV file contains benchmark results with columns: DataStructure, LoadFactor, Operation, Elements, TimeMS
data = pd.read_csv("performance_results.csv")

# Convert the 'Elements' column to numeric values
# This ensures that the data can be used for plotting on a numerical x-axis
data['Elements'] = pd.to_numeric(data['Elements'])

# Create a line chart to visualize operation performance vs. element count
plt.figure(figsize=(12, 6))  # Set the figure size to 12 inches wide and 6 inches tall

# Use Seaborn's lineplot function to plot the data
# - x='Elements': Number of elements (log scale) on the x-axis
# - y='TimeMS': Average execution time (in milliseconds) on the y-axis
# - hue='Operation': Different operations (Insert, Contains, Remove) are distinguished by color
# - style='DataStructure': Different data structures (HashSet, STL) are distinguished by line style
sns.lineplot(data=data, x='Elements', y='TimeMS', hue='Operation', style='DataStructure')

# Set the x-axis to use a logarithmic scale for better visualization of exponential growth trends
plt.xscale('log')

# Add labels and title to the chart for clarity
plt.xlabel('Number of Elements (Log Scale)')  # Label for the x-axis
plt.ylabel('Average Execution Time (ms)')  # Label for the y-axis
plt.title('Operation Performance vs. Element Count')  # Title of the chart

# Add grid lines to make it easier to read values from the chart
plt.grid(True)

# Adjust layout to prevent overlapping elements in the chart
plt.tight_layout()

# Save the chart as an image file named 'operation_performance.png'
plt.savefig("operation_performance.png")

# Display the chart in a window or notebook environment
plt.show()

# Create a bar chart to visualize load factor impact on performance
plt.figure(figsize=(14, 7))  # Set the figure size to 14 inches wide and 7 inches tall

# Use Seaborn's barplot function to plot the data
# - x='LoadFactor': Load factor thresholds (20%, 70%, 120%) on the x-axis
# - y='TimeMS': Execution time (in milliseconds) on the y-axis
# - hue='DataStructure': Different data structures (HashSet, STL) are distinguished by color
sns.barplot(data=data, x='LoadFactor', y='TimeMS', hue='DataStructure', dodge=True)

# Add labels and title to the chart for clarity
plt.xlabel('Load Factor Thresholds')  # Label for the x-axis
plt.ylabel('Execution Time (ms)')  # Label for the y-axis
plt.title('Load Factor Impact on Performance')  # Title of the chart

# Rotate x-axis labels slightly if needed for better readability (currently set to no rotation)
plt.xticks(rotation=0)

# Add grid lines along the y-axis for easier comparison of bar heights
plt.grid(axis='y')

# Adjust layout to prevent overlapping elements in the chart
plt.tight_layout()

# Save the chart as an image file named 'load_factor_impact.png'
plt.savefig("load_factor_impact.png")

# Display the chart in a window or notebook environment
plt.show()
