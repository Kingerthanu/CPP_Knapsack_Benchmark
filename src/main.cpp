#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;
#define MAX_THREADS 4


/*
    Desc:
      For 0-1 Knapsack Problem It Usually Utilizes 2 Arrays For weight And profit Of Each Item
      But Without names This Is A Custom Struct That Holds weight, profit, And name For
      Each Item For Easier Debugging And Unit Testing For Plotting Of Increasing n.
*/
struct Item
{
    float weight = 0.0f;
    float profit = 0.0f;
    std::string name;
};

// Postconditions:
//   1.) Holds Input Size n As Unsigned Integer
//   2.) Holds Runtime As Float Value
//   3.) Members Are Publicly Accessible
struct BenchmarkResult
{
    unsigned int n;
    float runtime;
};

// Postconditions:
//   1.) Provides Thread-Safe Queue Operations
//   2.) Maintains FIFO Order Of Tasks
//   3.) Handles Multiple Producer-Consumer Scenarios
class ThreadSafeQueue
{
    private:
        std::queue<unsigned int> tasks;
        std::mutex mutex;
        std::condition_variable condition;

    public:
        // Preconditions:
        //   1.) Valid Unsigned Integer Input
        // Postconditions:
        //   1.) Task Added To Queue In Thread-Safe Manner
        //   2.) Waiting Threads Notified Of New Task
        void push(unsigned int n)
        {
            std::lock_guard<std::mutex> lock(mutex);
            tasks.push(n);
            condition.notify_one();
        }

        // Preconditions:
        //   1.) Works As Valid Reference To Store Popped Value
        // Postconditions:
        //   1.) Returns True If Value Successfully Popped
        //   2.) Returns False If Queue Empty
        //   3.) Popped Value Stored In Reference Parameter
        bool pop(unsigned int& n)
        {
            std::unique_lock<std::mutex> lock(mutex);
            if (tasks.empty())
            {
                return false;
            }
            n = tasks.front();
            tasks.pop();
            return true;
        }
};



// Preconditions:
//   1.) Valid Array Of Items With Size n
//   2.) Capacity Must Be Non-Negative Float
//   3.) n Must Be Non-Negative Integer
//   4.) bin Vector Must Be Empty & Is Bin To Fill
// Postconditions:
//   1.) Returns Maximum Profit Achievable With Given capacity
//   2.) bin Contains Selected Items For Maximum Profit
//   3.) Original Items Array Remains Unchanged
float knapSack(Item items[], const float capacity, const unsigned int n, std::vector<Item*>& bin)
{
    // If We've Looked At All Elements Or At Full capacity
    if (capacity == 0.0f || n <= 0)
    {
        return 0.0f;
    }

    // If The item[n-1] Won't Fit
    if (capacity < items[n - 1].weight)
    {
        // Check When Not-Included (Can't Fit This, But Could Fit Maybe Remainder)
        return knapSack(items, capacity, n - 1, bin);
    }

    // Check The Remainder Things We Could Add If We Included Or Excluded Our Current item[n-1]
    std::vector<Item*> withBin, withoutBin;
    const float maxWithout = knapSack(items, capacity, n-1, withoutBin),
    maxWith = items[n-1].profit + knapSack(items, capacity - items[n-1].weight, n-1, withBin);

    // If We Should Include item[n-1]
    if (maxWith > maxWithout)
    {
        bin = withBin;
        bin.push_back(&items[n-1]);
        return maxWith;
    }

    // Else Exclude
    bin = withoutBin;
    return maxWithout;
}


// Preconditions:
//   1.) Valid Array Of Items With Size n
//   2.) Capacity Must Be Non-Negative Float
//   3.) n Must Be Non-Negative Integer
//   4.) bin Vector Must Be Empty & Is Bin To Fill
// Postconditions:
//   1.) Returns Near-Optimal Profit Using Greedy Approach
//   2.) bin Contains Selected Items Based On Profit/Weight Ratio
//   3.) Items Array Is Sorted By Profit/Weight Ratio
float knapSackHeuristic(Item items[], float capacity, unsigned int n, std::vector<Item*>& bin)
{
    // Sort Items By Profit/Weight Ratio (Descending) We Greedily Get As Much Profit Per Limited Capacity (Max)
    sort(items, items + n, [](const Item& a, const Item& b)
    {
        return (a.profit / a.weight) > (b.profit / b.weight);
    });

    float maxProfit = 0.0f, currentCapacity = 0.0f;
    unsigned int i = 0;

    // Go Until We Run Out Of Items To Look At Or We Have Reached Capacity
    while (i < n && currentCapacity < capacity)
    {
        // If We Can Fit The Next Item In
        if (items[i].weight + currentCapacity <= capacity)
        {
            // Add Item To Bin And Update Profit & Current Capacity
            bin.push_back(&items[i]);
            maxProfit += items[i].profit;
            currentCapacity += items[i].weight;
        }
        // Now Look At Next Item
        i++;
    }

    return maxProfit;
}

// Postconditions:
//   1.) Executes All Unit Tests For Exact Algorithm
//   2.) Displays Results For Each Test Case
//   3.) Memory Is Properly Managed And Released
void testUnit() {

    std::vector<Item*> selectedItems;
    unsigned int n;
    float totalWeight;
    float totalProfit;
    float capacity;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~UNIT TESTS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        // Unit-Test Case 1:
        {
            std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~START UNIT TEST CASE 1~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
            capacity = 10.0f;

            /*
               Create Items List For Each Unit Test
               Each Item Struct Contains: {Weight, Profit, Name}
            */
            Item items[] =
            {
                {2.5f, 100.0f, "Gaming_Console"},
                {1.0f, 50.0f, "Premium_Headphones"},
                {3.0f, 150.0f, "Drone"},
                {0.5f, 95.0f, "Smartwatch"},
                {2.0f, 75.0f, "Bluetooth_Speaker"},
                {1.5f, 80.0f, "Portable_Charger"},
                {0.8f, 60.0f, "Wireless_Mouse"},
                {4.0f, 200.0f, "4K_Camera"},
                {1.2f, 70.0f, "Keyboard"},
                {0.3f, 40.0f, "USB_Drive"}
            };

            n = sizeof(items) / sizeof(items[0]);

            totalProfit = knapSack(items, capacity, n, selectedItems);

            std::cout << "  Selected items:\n";
            totalWeight = 0.0f;
            for (const auto& item : selectedItems)
            {
                std::cout << "  - " << item->name << " (Weight: " << item->weight
                         << " kg, Profit: $" << item->profit << ")\n";
                totalWeight += item->weight;
            }
            std::cout << "\n  Total weight: " << totalWeight << " kg\n";
            std::cout << "  Total profit: $" << totalProfit << "\n";
            selectedItems.clear();
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END UNIT TEST CASE 1~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

        }

        // Unit-Test Case 2:
        {
            std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~START UNIT TEST CASE 2~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
            capacity = 1.0f;

            /*
               Create Items List For Each Unit Test
               Each Item Struct Contains: {Weight, Profit, Name}
            */
            Item items[] =
            {
                {0.1f, 1000.0f, "Diamond_Ring"},
                {0.5f, 800.0f, "Gold_Watch"},
                {0.3f, 1200.0f, "Platinum_Chain"},
                {0.2f, 900.0f, "Ruby_Earrings"},
                {0.4f, 1100.0f, "Sapphire_Bracelet"}
            };

            n = sizeof(items) / sizeof(items[0]);

            totalProfit = knapSack(items, capacity, n, selectedItems);

            std::cout << "  Selected items:\n";
            totalWeight = 0.0f;

            for (const auto& item : selectedItems)
            {
                std::cout << "  - " << item->name << " (Weight: " << item->weight
                         << " kg, Profit: $" << item->profit << ")\n";
                totalWeight += item->weight;
            }
            std::cout << "\n  Total weight: " << totalWeight << " kg\n";
            std::cout << "  Total profit: $" << totalProfit << "\n";
            selectedItems.clear();
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END UNIT TEST CASE 2~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

        }

        // Unit-Test Case 3:
        {
            std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~START UNIT TEST CASE 3~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
            capacity = 15.0f;

            /*
               Create Items List For Each Unit Test
               Each Item Struct Contains: {Weight, Profit, Name}
            */
            Item items[] =
            {
                {5.0f, 300.0f, "Tennis_Racket"},
                {8.0f, 500.0f, "Golf_Set"},
                {3.0f, 200.0f, "Basketball"},
                {2.0f, 150.0f, "Soccer_Ball"},
                {4.0f, 250.0f, "Baseball_Kit"},
                {6.0f, 400.0f, "Snowboard"}
            };

            n = sizeof(items) / sizeof(items[0]);

            totalProfit = knapSack(items, capacity, n, selectedItems);

            std::cout << "  Selected items:\n";
            totalWeight = 0.0f;
            for (const auto& item : selectedItems)
            {
                std::cout << "  - " << item->name << " (Weight: " << item->weight
                         << " kg, Profit: $" << item->profit << ")\n";
                totalWeight+= item->weight;
            }
            std::cout << "\n  Total weight: " << totalWeight << " kg\n";
            std::cout << "  Total profit: $" << totalProfit << "\n";
            selectedItems.clear();
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END UNIT TEST CASE 3~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

        }

        // Unit-Test Case 4:
        {
            std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~START UNIT TEST CASE 4~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
            capacity = 12.0f;

            /*
               Create Items List For Each Unit Test
               Each Item Struct Contains: {Weight, Profit, Name}
            */
            Item items[] =
            {
                {1.0f, 1000.0f, "High_Ratio"},
                {10.0f, 1100.0f, "Low_Ratio"},
                {2.0f, 900.0f, "Medium_High"},
                {5.0f, 800.0f, "Medium_Low"},
                {0.5f, 400.0f, "Small_High"},
                {8.0f, 850.0f, "Large_Low"}
            };

            n = sizeof(items) / sizeof(items[0]);

            totalProfit = knapSack(items, capacity, n, selectedItems);

            std::cout << "  Selected items:\n";
            totalWeight = 0.0f;
            for (const auto& item : selectedItems)
            {
                std::cout << "  - " << item->name << " (Weight: " << item->weight
                         << " kg, Profit: $" << item->profit << ")\n";
                totalWeight += item->weight;
            }
            std::cout << "\n  Total weight: " << totalWeight << " kg\n";
            std::cout << "  Total profit: $" << totalProfit << "\n";
            selectedItems.clear();
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END UNIT TEST CASE 4~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END UNIT TESTS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


}


// Postconditions:
//   1.) Executes All Unit Tests For Heuristic Algorithm
//   2.) Displays Results For Each Test Case
//   3.) Memory Is Properly Managed And Released
void testUnitHeuristic() {

    std::vector<Item*> selectedItems;
    unsigned int n;
    float totalWeight;
    float totalProfit;
    float capacity;

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~UNIT TESTS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

        // Unit-Test Case 1:
        {
            std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~START UNIT TEST CASE 1~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
            capacity = 10.0f;

            /*
               Create Items List For Each Unit Test
               Each Item Struct Contains: {Weight, Profit, Name}
            */
            Item items[] =
            {
                {2.5f, 100.0f, "Gaming_Console"},
                {1.0f, 50.0f, "Premium_Headphones"},
                {3.0f, 150.0f, "Drone"},
                {0.5f, 95.0f, "Smartwatch"},
                {2.0f, 75.0f, "Bluetooth_Speaker"},
                {1.5f, 80.0f, "Portable_Charger"},
                {0.8f, 60.0f, "Wireless_Mouse"},
                {4.0f, 200.0f, "4K_Camera"},
                {1.2f, 70.0f, "Keyboard"},
                {0.3f, 40.0f, "USB_Drive"}
            };

            n = sizeof(items) / sizeof(items[0]);

            totalProfit = knapSackHeuristic(items, capacity, n, selectedItems);

            std::cout << "  Selected items:\n";
            totalWeight = 0.0f;
            for (const auto& item : selectedItems) {
                std::cout << "  - " << item->name << " (Weight: " << item->weight
                         << " kg, Profit: $" << item->profit << ")\n";
                totalWeight += item->weight;
            }
            std::cout << "\n  Total weight: " << totalWeight << " kg\n";
            std::cout << "  Total profit: $" << totalProfit << "\n";
            selectedItems.clear();
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END UNIT TEST CASE 1~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

        }

        // Unit-Test Case 2:
        {
            std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~START UNIT TEST CASE 2~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
            capacity = 1.0f;

            /*
               Create Items List For Each Unit Test
               Each Item Struct Contains: {Weight, Profit, Name}
            */
            Item items[] = {
                {0.1f, 1000.0f, "Diamond_Ring"},
                {0.5f, 800.0f, "Gold_Watch"},
                {0.3f, 1200.0f, "Platinum_Chain"},
                {0.2f, 900.0f, "Ruby_Earrings"},
                {0.4f, 1100.0f, "Sapphire_Bracelet"}
            };

            n = sizeof(items) / sizeof(items[0]);

            totalProfit = knapSackHeuristic(items, capacity, n, selectedItems);

            std::cout << "  Selected items:\n";
            totalWeight = 0.0f;

            for (const auto& item : selectedItems) {
                std::cout << "  - " << item->name << " (Weight: " << item->weight
                         << " kg, Profit: $" << item->profit << ")\n";
                totalWeight += item->weight;
            }
            std::cout << "\n  Total weight: " << totalWeight << " kg\n";
            std::cout << "  Total profit: $" << totalProfit << "\n";
            selectedItems.clear();
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END UNIT TEST CASE 2~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

        }

        // Unit-Test Case 3:
        {
            std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~START UNIT TEST CASE 3~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
            capacity = 15.0f;

            /*
               Create Items List For Each Unit Test
               Each Item Struct Contains: {Weight, Profit, Name}
            */
            Item items[] = {
                {5.0f, 300.0f, "Tennis_Racket"},
                {8.0f, 500.0f, "Golf_Set"},
                {3.0f, 200.0f, "Basketball"},
                {2.0f, 150.0f, "Soccer_Ball"},
                {4.0f, 250.0f, "Baseball_Kit"},
                {6.0f, 400.0f, "Snowboard"}
            };

            n = sizeof(items) / sizeof(items[0]);

            totalProfit = knapSackHeuristic(items, capacity, n, selectedItems);

            std::cout << "  Selected items:\n";
            totalWeight = 0.0f;
            for (const auto& item : selectedItems) {
                std::cout << "  - " << item->name << " (Weight: " << item->weight
                         << " kg, Profit: $" << item->profit << ")\n";
                totalWeight+= item->weight;
            }
            std::cout << "\n  Total weight: " << totalWeight << " kg\n";
            std::cout << "  Total profit: $" << totalProfit << "\n";
            selectedItems.clear();
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END UNIT TEST CASE 3~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

        }

        // Unit-Test Case 4:
        {
            std::cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~START UNIT TEST CASE 4~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
            capacity = 12.0f;

            /*
               Create Items List For Each Unit Test
               Each Item Struct Contains: {Weight, Profit, Name}
            */
            Item items[] = {
                {1.0f, 1000.0f, "High_Ratio"},
                {10.0f, 1100.0f, "Low_Ratio"},
                {2.0f, 900.0f, "Medium_High"},
                {5.0f, 800.0f, "Medium_Low"},
                {0.5f, 400.0f, "Small_High"},
                {8.0f, 850.0f, "Large_Low"}
            };

            n = sizeof(items) / sizeof(items[0]);

            totalProfit = knapSackHeuristic(items, capacity, n, selectedItems);

            std::cout << "  Selected items:\n";
            totalWeight = 0.0f;
            for (const auto& item : selectedItems) {
                std::cout << "  - " << item->name << " (Weight: " << item->weight
                         << " kg, Profit: $" << item->profit << ")\n";
                totalWeight += item->weight;
            }
            std::cout << "\n  Total weight: " << totalWeight << " kg\n";
            std::cout << "  Total profit: $" << totalProfit << "\n";
            selectedItems.clear();
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END UNIT TEST CASE 4~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        }

    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~END UNIT TESTS~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


}

// Preconditions:
//   1.) Valid Queue, Results Vector, And Mutex References
// Postconditions:
//   1.) Processes Benchmark Tasks For Exact Algorithm
//   2.) Updates Results Vector With Runtime Data
//   3.) Thread-Safe Operation Maintained
void benchmarkWorker(ThreadSafeQueue& taskQueue, std::vector<BenchmarkResult>& results, std::mutex& resultsMutex)
{
    unsigned int n;
    while(taskQueue.pop(n))
    {
        std::cout << "- BRUTE FORCE:   Current n Size Of n = " << n << std::endl;
        Item* items = new Item[n];
        for(unsigned int i = 0; i < n; i++)
        {
            items[i] =
            {
                (float)(i + 1),
                (float)((i + 1) * 10),
                "Item" + std::to_string(i)
            };
        }

        std::vector<Item*> bin;
        float capacity = (float)(n) * 0.5f;

        auto start = std::chrono::high_resolution_clock::now();
        knapSack(items, capacity, n, bin);
        auto end = std::chrono::high_resolution_clock::now();

        float runtime = std::chrono::duration<float, std::milli>(end - start).count();
        {
            std::lock_guard<std::mutex> lock(resultsMutex);
            results.push_back({n, runtime});
        }

        delete[] items;
        std::cout << "- BRUTE FORCE:   END OF Current n Size Of n = " << n << std::endl;
    }
}

// Preconditions:
//   1.) Valid Queue, Results Vector, And Mutex References
// Postconditions:
//   1.) Processes Benchmark Tasks For Heuristic Algorithm
//   2.) Updates Results Vector With Runtime Data
//   3.) Thread-Safe Operation Maintained
void benchmarkWorkerHeuristic(ThreadSafeQueue& taskQueue, std::vector<BenchmarkResult>& results, std::mutex& resultsMutex)
{
    unsigned int n;
    while(taskQueue.pop(n))
    {
        std::cout << "- HEURISTIC:   Current n Size Of n = " << n << std::endl;
        Item* items = new Item[n];
        for(unsigned int i = 0; i < n; i++)
        {
            items[i] =
            {
                static_cast<float>(i + 1),
                static_cast<float>((i + 1) * 10),
                "Item" + std::to_string(i)
            };
        }

        std::vector<Item*> bin;
        float capacity = static_cast<float>(n) * 0.5f;

        auto start = std::chrono::high_resolution_clock::now();
        knapSackHeuristic(items, capacity, n, bin);
        auto end = std::chrono::high_resolution_clock::now();

        float runtime = std::chrono::duration<float, std::milli>(end - start).count();
        {
            std::lock_guard<std::mutex> lock(resultsMutex);
            results.push_back({n, runtime});
        }

        delete[] items;
        std::cout << "- HEURISTIC:   END OF Current n Size Of n = " << n << std::endl;
    }
}

// Postconditions:
//   1.) Generates JSON File With Benchmark Results
//   2.) Multi-Threaded Execution Complete
//   3.) Results Sorted By Input Size
void testBenchmark() {
    ThreadSafeQueue taskQueue;
    std::vector<BenchmarkResult> results;
    std::mutex resultsMutex;

    // Fill queue with tasks
    for(unsigned int n = 10; n <= 450; n += 10)
    {
        taskQueue.push(n);
    }

    // Create thread pool
    std::vector<std::thread> threads;
    for(int i = 0; i < MAX_THREADS; i++) {
        threads.emplace_back(benchmarkWorker, std::ref(taskQueue),  std::ref(results), std::ref(resultsMutex));
    }

    // Wait for all threads to complete
    for(auto& thread : threads) {
        thread.join();
    }

    // Sort results by n
    std::sort(results.begin(), results.end(),
              [](const BenchmarkResult& a, const BenchmarkResult& b) {
                  return a.n < b.n;
              });

    // Write results to JSON
    std::ofstream jsonFile("benchmark_results.json");
    jsonFile << "{\n    \"Benchmark Results\": {\n";

    for(size_t i = 0; i < results.size(); i++) {
        jsonFile << "        \"Test " << results[i].n << "\": {\n";
        jsonFile << "            \"n\": " << results[i].n << ",\n";
        jsonFile << "            \"y\": " << results[i].runtime << "\n";
        jsonFile << "        }";
        if(i < results.size() - 1) jsonFile << ",";
        jsonFile << "\n";
    }

    jsonFile << "    }\n}";
    jsonFile.close();

}

// Postconditions:
//   1.) Generates JSON File With Heuristic Benchmark Results
//   2.) Multi-Threaded Execution Complete
//   3.) Results Sorted By Input Size
void testBenchmarkHeuristic() {
    ThreadSafeQueue taskQueue;
    std::vector<BenchmarkResult> results;
    std::mutex resultsMutex;

    // Fill queue with tasks
    for(unsigned int n = 10; n <= 450; n += 10) {
        taskQueue.push(n);
    }

    // Create thread pool
    std::vector<std::thread> threads;
    for(int i = 0; i < MAX_THREADS; i++) {
        threads.emplace_back(benchmarkWorkerHeuristic, std::ref(taskQueue),  std::ref(results), std::ref(resultsMutex));
    }

    // Wait for all threads to complete
    for(auto& thread : threads) {
        thread.join();
    }

    // Sort results by n
    std::sort(results.begin(), results.end(),
              [](const BenchmarkResult& a, const BenchmarkResult& b) {
                  return a.n < b.n;
              });

    // Write results to JSON
    std::ofstream jsonFile("benchmark_results_heuristic.json");
    jsonFile << "{\n    \"Benchmark Results\": {\n";

    for(size_t i = 0; i < results.size(); i++) {
        jsonFile << "        \"Test " << results[i].n << "\": {\n";
        jsonFile << "            \"n\": " << results[i].n << ",\n";
        jsonFile << "            \"y\": " << results[i].runtime << "\n";
        jsonFile << "        }";
        if(i < results.size() - 1) jsonFile << ",";
        jsonFile << "\n";
    }

    jsonFile << "    }\n}";
    jsonFile.close();

}


int main()
{

    testUnit();
    testUnitHeuristic();

    return 0;
}
