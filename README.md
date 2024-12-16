# CPP_Knapsack_Benchmark
Runtime Complexity Benchmark Analysis Of The Exhaustive &amp; "Standard Greedy"  0-1 Knapsack Heuristic Implementations. 

----------------------------------------------

<img src="https://github.com/user-attachments/assets/b42434d4-bd8f-46c2-b5e7-5de18d4db46b" alt="Cornstarch <3" width="75" height="99"> <img src="https://github.com/user-attachments/assets/b42434d4-bd8f-46c2-b5e7-5de18d4db46b" alt="Cornstarch <3" width="75" height="99"> <img src="https://github.com/user-attachments/assets/b42434d4-bd8f-46c2-b5e7-5de18d4db46b" alt="Cornstarch <3" width="75" height="99"> <img src="https://github.com/user-attachments/assets/b42434d4-bd8f-46c2-b5e7-5de18d4db46b" alt="Cornstarch <3" width="75" height="99"> 


<h3>**The Breakdown:**</h3>

This Program Starts With A C++ Process Which Will Benchmark 2 Differing Implementations To Solve The 0-1 Knapsack Problem:
  <br>&nbsp; 1) Recursive Optimal Exhaustive Include-Exclude Algorithm: A O(2^n) Runtime Complexity Algorithm Which Checks All Subset Combinations
  <br>&nbsp; 2) Iterative Suboptimal "Standard Greedy" (Profit-Per-Weight) Heuristic: A O(n*log(n)) Runtime Complexity Heuristic Binning By Profit-Per-Weight

The Program Is Meant To See How Varying Algorthmic Implementations Increases In Runtime Complexity And Considerations For Each Of Their Utility Depending On Domain-Knowledge. 

For 0-1 Knapsack Problem It Usually Utilizes 2 Arrays For weight And profit Of Each Item But Without names This Is A Custom Struct That Holds weight, profit, And name For Each Item For Easier Debugging And Unit Testing For Plotting Of Increasing n:

    struct Item
    {
      float weight = 0.0f;
      float profit = 0.0f;
      std::string name;
    };


After The Program Is Ran, It Will Run 45 Individual Benchmarks On The Two Differing Algorithms--with both utilizing the same items to consider to ensure equal argument complexity--each of these tests will be done in increments of 10; this means we are running from 10 -> 450. This Allows Us To See How Runtime Scales Between Varying Values Of n, Where n Is The Number Of Input Items We Are Checking To Insert.

Each Test Will Utilize This Code Snippet To Generate Our Items For Each Increment Of n:

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


<img src="https://github.com/user-attachments/assets/00f8d76b-9e49-432c-9506-3d460840a991" alt="Cornstarch <3" width="75" height="99"> <img src="https://github.com/user-attachments/assets/00f8d76b-9e49-432c-9506-3d460840a991" alt="Cornstarch <3" width="75" height="99"> <img src="https://github.com/user-attachments/assets/00f8d76b-9e49-432c-9506-3d460840a991" alt="Cornstarch <3" width="75" height="99"> <img src="https://github.com/user-attachments/assets/00f8d76b-9e49-432c-9506-3d460840a991" alt="Cornstarch <3" width="75" height="99"> 

----------------------------------------------

<img src="https://github.com/user-attachments/assets/645e1b72-7232-4f0c-9214-ec72adf171cf" alt="Cornstarch <3" width="55" height="49"> <img src="https://github.com/user-attachments/assets/645e1b72-7232-4f0c-9214-ec72adf171cf" alt="Cornstarch <3" width="55" height="49"> <img src="https://github.com/user-attachments/assets/645e1b72-7232-4f0c-9214-ec72adf171cf" alt="Cornstarch <3" width="55" height="49"> <img src="https://github.com/user-attachments/assets/645e1b72-7232-4f0c-9214-ec72adf171cf" alt="Cornstarch <3" width="55" height="49">  




<h3>**Features:**</h3>

Generated Benchmark Charts From Python Script Using Collected Data:
![algorithm_comparison](https://github.com/user-attachments/assets/d6297d24-06dd-43b3-ba97-4cccfdc59476)

![knapsack_algorithm_comparison_split](https://github.com/user-attachments/assets/28fefa31-6856-4a5e-b491-dc36a61b6550)

![low_n_comparison](https://github.com/user-attachments/assets/379f939f-4d5b-4bee-b062-bd0090fdff19)

![runtime_analysis_heuristic](https://github.com/user-attachments/assets/840c5d62-99a5-4075-a423-1fea90fbbf82)

![runtime_analysis](https://github.com/user-attachments/assets/2973feea-065f-4349-8feb-7fcd90b3d456)

