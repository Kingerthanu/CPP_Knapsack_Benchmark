import json
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# Preconditions:
#   1.) The Data Is In The Format Of A Dictionary With Keys Representing The Input Size (N) And Values Representing The Runtime (Y)
#   2.) Benchmark Results In JSON File Are In Benchmark_Results_Heuristic.json In Cmake-Build-Debug Folder
#   3.) Will Run "SubOptimal" Greedy Heuristic Benchmark Of 0-1 Knapsack Problem
# Postconditions:
#   1.) A Plot Is Generated With The Input Size On The X-Axis And The Runtime On The Y-Axis
#   2.) The Plot Is Saved As A PNG File
#   3.) The Plot Is Displayed
def plot_benchmark_heuristic():
    # Set Dark Style For Plot
    plt.style.use('dark_background')

    # Read JSON Data From File
    with open('benchmark_results_heuristic.json') as f:
        data = json.load(f)

    # Extract Data Points From JSON
    x_values = []
    y_values = []
    for test in data["Benchmark Results"].values():
        x_values.append(test["n"])
        y_values.append((test["y"] * 1000))

    # Create Modern Looking Plot With Styling
    fig, ax = plt.subplots(figsize=(12, 8))
    ax.plot(x_values, y_values, 'c-', linewidth=2, marker='o', markersize=8)

    # Apply Style Enhancements To Plot
    ax.set_title('Algorithm Heuristic Performance Analysis', fontsize=16, pad=20)
    ax.set_xlabel('Input Size (n)', fontsize=12)
    ax.set_ylabel('Runtime (µs)', fontsize=12)

    # Increase Tick Density And Rotate Labels For Better Visibility
    ax.xaxis.set_major_locator(plt.MaxNLocator(20))
    ax.yaxis.set_major_locator(plt.MaxNLocator(20))

    # Rotate X-Axis Labels For Clarity
    plt.xticks(rotation=45)

    # Add Minor Ticks For Enhanced Precision
    ax.minorticks_on()

    # Style The Grid To Match The Increased Detail
    ax.grid(True, which='major', linestyle='-', alpha=0.3)
    ax.grid(True, which='minor', linestyle=':', alpha=0.2)

    # Apply Modern Visual Touches
    plt.fill_between(x_values, y_values, alpha=0.2, color='cyan')
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)

    # Add Last Value Annotation To Plot
    last_x = x_values[-1]
    last_y = y_values[-1]
    ax.annotate(f'{last_y:.6f} µs',
                xy=(last_x, last_y),
                xytext=(10, 10),
                textcoords='offset points',
                color='cyan',
                fontweight='bold')

    # Save Plot With High DPI And Display
    plt.savefig('runtime_analysis_heuristic.png', dpi=600, bbox_inches='tight', facecolor='black')
    plt.show()


# Preconditions:
#   1.) Both JSON Files Exist In Cmake-Build-Debug Directory
#   2.) JSON Files Contain Valid Benchmark Results
#   3.) Data Structure Follows Standard Format With n And y Values
# Postconditions:
#   1.) Generates High Resolution Comparison Plot
#   2.) Saves Plot As PNG File
#   3.) Displays Interactive Plot Window
def plot_comparison():
    # Set Dark Theme For Professional Visualization
    plt.style.use('dark_background')

    # Read Both Algorithm Results From JSON Files
    with open('benchmark_results_backup.json') as f1, \
            open('benchmark_results_heuristic.json') as f2:
        data_exact = json.load(f1)
        data_heuristic = json.load(f2)

    # Extract And Convert Data Points For Both Algorithms
    x_exact, y_exact = [], []
    x_heur, y_heur = [], []

    # Process Exact Algorithm Data (Convert To Hours)
    for test in data_exact["Benchmark Results"].values():
        x_exact.append(test["n"])
        y_exact.append(test["y"] / (1000 * 60 * 60))

    # Process Heuristic Algorithm Data (Convert To Microseconds)
    for test in data_heuristic["Benchmark Results"].values():
        x_heur.append(test["n"])
        y_heur.append(test["y"] * 1000)

    # Initialize Dual-Axis Plot Configuration
    fig, ax1 = plt.subplots(figsize=(15, 8))
    ax2 = ax1.twinx()

    # Create Plot Lines With Distinct Styling
    line1 = ax1.plot(x_exact, y_exact, 'c-', linewidth=2, marker='o', label='Exact (2^n)')
    line2 = ax2.plot(x_heur, y_heur, 'r-', linewidth=2, marker='s', label='Heuristic (n log n)')

    # Apply Professional Styling Elements
    ax1.set_title('Algorithm Performance Comparison (Exact Runtime × 10^6)', fontsize=16, pad=20)
    ax1.set_xlabel('Input Size (n)', fontsize=12)
    ax1.set_ylabel('Exact Runtime (hours)', fontsize=12, color='cyan')
    ax2.set_ylabel('Heuristic Runtime (µs)', fontsize=12, color='red')

    # Combine Legend Elements
    lines = line1 + line2
    labels = [l.get_label() for l in lines]
    ax1.legend(lines, labels, loc='upper left')

    # Increase Tick Density And Rotate Labels For Better Visibility
    ax1.xaxis.set_major_locator(plt.MaxNLocator(20))
    ax1.yaxis.set_major_locator(plt.MaxNLocator(20))

    # Rotate X-Axis Labels For Clarity
    plt.xticks(rotation=45)

    # Add Minor Ticks For Enhanced Precision
    ax1.minorticks_on()

    # Apply Grid And Color-Coded Axis Styling
    ax1.grid(True, linestyle='--', alpha=0.3)
    ax1.tick_params(axis='y', colors='cyan')
    ax2.tick_params(axis='y', colors='red')

    # Generate Final Output
    plt.savefig('algorithm_comparison.png', dpi=600, bbox_inches='tight', facecolor='black')
    plt.show()


# Preconditions:
#   1.) Valid JSON Files With Benchmark Results Exist
#   2.) Python Environment Has Required Libraries (pandas, seaborn, matplotlib)
# Postconditions:
#   1.) Generates Box And Whisker Plot Comparing Both Algorithms
#   2.) Displays Statistical Summary Of Runtime Distributions
#   3.) Saves Plot To Output File
def plotBoxComparison():
    """
    Generates professional dual-axis box and whisker plot comparing exact vs heuristic KnapSack implementations,
    with exact algorithm plotted in microseconds and heuristic in milliseconds
    """
    # Load benchmark results with correct paths
    with open('benchmark_results_backup.json') as f:
        exact_data = json.load(f)
        exact_runtimes = [(test['y']) for test in exact_data['Benchmark Results'].values()]

    with open('benchmark_results_heuristic.json') as f:
        heuristic_data = json.load(f)
        heuristic_runtimes = [(test['y']) for test in heuristic_data['Benchmark Results'].values()]

    # Create figure with two subplots
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 8))

    # Plot exact algorithm (microseconds) - horizontal
    bp1 = ax1.boxplot(exact_runtimes,
                      vert=False,  # Make horizontal
                      patch_artist=True,
                      medianprops={'color': 'black', 'linewidth': 2},
                      flierprops={'marker': 'o', 'markerfacecolor': 'gray'},
                      boxprops={'facecolor': 'lightblue', 'alpha': 0.8})

    # Plot heuristic algorithm (milliseconds) - horizontal
    bp2 = ax2.boxplot(heuristic_runtimes,
                      vert=False,  # Make horizontal
                      patch_artist=True,
                      medianprops={'color': 'black', 'linewidth': 2},
                      flierprops={'marker': 'o', 'markerfacecolor': 'gray'},
                      boxprops={'facecolor': 'lightgreen', 'alpha': 0.8})

    # Configure appearance for exact algorithm plot
    ax1.set_title('Exact Algorithm Runtime (x 10^6)', fontsize=14)
    ax1.set_xlabel('Runtime (milliseconds)', fontsize=12)  # Changed from ylabel to xlabel
    ax1.grid(True, linestyle='--', alpha=0.7)
    ax1.spines['top'].set_visible(False)
    ax1.spines['right'].set_visible(False)

    # Configure appearance for heuristic algorithm plot
    ax2.set_title('Heuristic Algorithm Runtime', fontsize=14)
    ax2.set_xlabel('Runtime (milliseconds)', fontsize=12)  # Changed from ylabel to xlabel
    ax2.grid(True, linestyle='--', alpha=0.7)
    ax2.spines['top'].set_visible(False)
    ax2.spines['right'].set_visible(False)

    # Add overall title
    fig.suptitle('Runtime Distribution Analysis: Exact vs Heuristic Implementation',
                 fontsize=16, y=1.05)

    # Calculate and display statistics
    for algorithm, runtimes, unit in [('Exact', exact_runtimes, 'μs'),
                                      ('Heuristic', heuristic_runtimes, 'ms')]:
        stats = {
            'Mean': np.mean(runtimes),
            'Median': np.median(runtimes),
            'Q1': np.percentile(runtimes, 25),
            'Q3': np.percentile(runtimes, 75),
            'Min': min(runtimes),
            'Max': max(runtimes)
        }

        print(f"\n{algorithm} Algorithm Statistics ({unit}):")
        for metric, value in stats.items():
            print(f"{metric}: {value :.2f}{unit}")

    plt.tight_layout()

    # Increase Tick Density And Rotate Labels For Better Visibility
    ax1.xaxis.set_major_locator(plt.MaxNLocator(22))

    # Increase Tick Density And Rotate Labels For Better Visibility
    ax2.xaxis.set_major_locator(plt.MaxNLocator(18))

    plt.savefig('knapsack_algorithm_comparison_split.png', dpi=300, bbox_inches='tight')
    plt.close()


# Preconditions:
#   1.) The Data Is In The Format Of A Dictionary With Keys Representing The Input Size (N) And Values Representing The Runtime (Y)
#   2.) Benchmark Results In JSON File Are In Benchmark_Results.json In Cmake-Build-Debug Folder
#   3.) Will Run Optimal Exhaustive Search Benchmark Of 0-1 Knapsack Problem
# Postconditions:
#   1.) A Plot Is Generated With The Input Size On The X-Axis And The Runtime On The Y-Axis
#   2.) The Plot Is Saved As A PNG File
#   3.) The Plot Is Displayed
def plot_benchmark():
    # Set Dark Style For Plot
    plt.style.use('dark_background')

    # Read JSON Data From File
    with open('benchmark_results_backup.json') as f:
        data = json.load(f)

    # Extract Data Points From JSON
    x_values = []
    y_values = []
    for test in data["Benchmark Results"].values():
        x_values.append(test["n"])
        y_values.append((test["y"]/ (1000 * 60 * 60)))

    # Create Modern Looking Plot With Styling
    fig, ax = plt.subplots(figsize=(12, 8))
    ax.plot(x_values, y_values, 'c-', linewidth=2, marker='o', markersize=8)

    # Apply Style Enhancements To Plot
    ax.set_title('Algorithm Performance Analysis', fontsize=16, pad=20)
    ax.set_xlabel('Input Size (n)', fontsize=12)
    ax.set_ylabel('Runtime (hrs)', fontsize=12)

    # Increase Tick Density And Rotate Labels For Better Visibility
    ax.xaxis.set_major_locator(plt.MaxNLocator(20))
    ax.yaxis.set_major_locator(plt.MaxNLocator(20))

    # Rotate X-Axis Labels For Clarity
    plt.xticks(rotation=45)

    # Add Minor Ticks For Enhanced Precision
    ax.minorticks_on()

    # Style The Grid To Match The Increased Detail
    ax.grid(True, which='major', linestyle='-', alpha=0.3)
    ax.grid(True, which='minor', linestyle=':', alpha=0.2)

    # Apply Modern Visual Touches
    plt.fill_between(x_values, y_values, alpha=0.2, color='cyan')
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)

    # Add Last Value Annotation To Plot
    last_x = x_values[-1]
    last_y = y_values[-1]
    ax.annotate(f'{last_y:.6f} hrs',
                xy=(last_x, last_y),
                xytext=(10, 10),
                textcoords='offset points',
                color='cyan',
                fontweight='bold')

    # Save Plot With High DPI And Display
    plt.savefig('runtime_analysis.png', dpi=600, bbox_inches='tight', facecolor='black')
    plt.show()


# Preconditions:
#   1.) Both JSON Files Exist In Cmake-Build-Debug Directory
#   2.) Data Structure Contains Valid Runtime Measurements
#   3.) Excel Writing Permissions Are Available In Current Directory
# Postconditions:
#   1.) Creates Excel File With Algorithm Comparison Data
#   2.) File Contains Input Sizes And Corresponding Runtimes
#   3.) Data Is Organized In Clearly Labeled Columns
def generate_excel_comparison():

    # Read benchmark files
    with open('benchmark_results_backup.json') as f1, \
            open('benchmark_results_heuristic.json') as f2:
        exact_data = json.load(f1)
        heuristic_data = json.load(f2)

    # Create data lists
    data = {
        'Input Size (n)': [],
        'Exact Algorithm (hours)': [],
        'Heuristic Algorithm (μs)': []
    }

    # Populate data
    for test_exact, test_heur in zip(exact_data["Benchmark Results"].values(),
                                     heuristic_data["Benchmark Results"].values()):
        data['Input Size (n)'].append(test_exact["n"])
        data['Exact Algorithm (hours)'].append(test_exact["y"])
        data['Heuristic Algorithm (μs)'].append(test_heur["y"])

    # Create DataFrame and export to Excel
    df = pd.DataFrame(data)
    df.to_excel('knapsack_comparison.xlsx', index=False, sheet_name='Algorithm Comparison')


# Preconditions:
#   1.) Matplotlib Library Is Available
#   2.) Predefined Test Data Points Are Valid
#   3.) Write Permissions Exist In Current Directory
# Postconditions:
#   1.) Generates Plot Comparing Algorithm Performance For Small Input Sizes
#   2.) Saves Plot As PNG File With High Resolution
#   3.) Displays Interactive Plot Window
def plot_low_n_comparison():
    plt.style.use('dark_background')

    # Data points
    n_values = [10, 20, 30, 40]
    exact_times = [0.0028, 0.0147, 0.0353, 0.0614]
    heuristic_times = [0.0042, 0.0043, 0.0041, 0.0211]

    # Create plot
    fig, ax = plt.subplots(figsize=(12, 8))

    # Plot both lines
    ax.plot(n_values, exact_times, 'c-', linewidth=2, marker='o', label='Exact Algorithm')
    ax.plot(n_values, heuristic_times, 'r-', linewidth=2, marker='s', label='Heuristic Algorithm')

    # Styling
    ax.set_title('Algorithm Performance Comparison (Low n Values)', fontsize=16, pad=20)
    ax.set_xlabel('Input Size (n)', fontsize=12)
    ax.set_ylabel('Runtime (milliseconds)', fontsize=12)
    ax.legend()
    ax.grid(True, linestyle='--', alpha=0.3)

    # Add minor ticks and grid
    ax.minorticks_on()
    ax.grid(True, which='minor', linestyle=':', alpha=0.2)

    # Save and show
    plt.savefig('low_n_comparison.png', dpi=600, bbox_inches='tight', facecolor='black')
    plt.show()



if __name__ == "__main__":
    plot_benchmark_heuristic()
    plot_benchmark()
    plot_comparison()
    plotBoxComparison()
    plot_low_n_comparison()
    generate_excel_comparison()


