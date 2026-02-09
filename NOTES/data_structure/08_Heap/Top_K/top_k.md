[简化版代码](./top_k.cpp)


# 算法笔记：解决 Top-K 问题 (小顶堆法)

## 1. 问题描述

Top-K 问题是指从海量数据或一个大数组中，找出**最大**的 K 个元素。

## 2. 核心思路：为什么使用小顶堆？

初学者容易产生直觉误区：找最大的数，应该用大顶堆。
但在解决 Top-K 问题时，最优解通常是使用**小顶堆 (Min Heap)**。

**逻辑推导：**

1. 我们维护一个大小为 `k` 的容器，代表当前找到的“最大的 K 个数”。
2. 在这个容器中，**堆顶**（根节点）是这 K 个数中**最小**的那个（即“门槛”）。
3. 遍历剩余元素时，拿新元素与堆顶比较：
* 如果 **新元素 > 堆顶**：说明新元素比容器里最弱的元素要强，它有资格进入前 K 名。因此，我们将堆顶踢出 (`pop`)，将新元素加入 (`push`)。
* 如果 **新元素 <= 堆顶**：说明新元素比前 K 名里最弱的还要弱，直接忽略。


4. 遍历结束后，堆里留下的就是最大的 K 个元素。

## 3. 代码实现

以下代码基于 C++ STL 的 `std::priority_queue` 实现。

> **注意**：
> 1. 标准库默认是**大顶堆** (`less<T>`)。
> 2. 声明**小顶堆**需指定比较器为 `greater<T>`，完整定义为 `priority_queue<int, vector<int>, greater<int>>`。
> 
> 

```cpp
#include <iostream>
#include <queue>
#include <vector>
#include <stdexcept>

using namespace std;

/**
 * @brief 获取数组中最大的 K 个元素
 * @param nums 输入的整数数组
 * @param k 需要获取的元素个数
 * @return 包含 Top K 元素的小顶堆
 */
static priority_queue<int, vector<int>, greater<int>> topKHeap(const vector<int>& nums, int k)
{
    // 边界检查：如果 k 大于数组长度，抛出异常
    if (k > nums.size()) // 修正点：size() 是函数，需要加括号
        throw out_of_range("k 的值超过数组长度");

    if (k <= 0)
        return priority_queue<int, vector<int>, greater<int>>();

    // 定义小顶堆：数值越小优先级越高，排在堆顶
    priority_queue<int, vector<int>, greater<int>> heap;

    // 1. 先将前 k 个元素直接入堆
    for (int i = 0; i < k; i++)
    {
        heap.push(nums[i]);
    }

    // 2. 遍历剩余元素，维护堆的性质
    for (int j = k; j < nums.size(); j++)
    {
        // 如果当前元素比堆顶（当前 TopK 中的最小值）大
        // 说明当前元素更有资格进入 TopK
        if (nums[j] > heap.top())
        {
            heap.pop();       // 移除堆顶（最小值）
            heap.push(nums[j]); // 压入新元素，堆会自动调整结构
        }
    }

    return heap;
}

int main()
{
    vector<int> nums = { 1, 0, 6, 4, 10, 7, 3, 8 };
    int k = 4;

    try 
    {
        // 获取结果堆
        priority_queue<int, vector<int>, greater<int>> heap = topKHeap(nums, k);

        cout << "数组中最大的 " << k << " 个元素为（从小到大输出）: ";
        
        // 注意：priority_queue 没有迭代器，只能通过 pop 遍历
        // 这里的 size 必须提前保存，因为 pop 会改变 size
        int size = heap.size();
        for (int i = 0; i < size; i++)
        {
            cout << heap.top() << " ";
            heap.pop();
        }
        cout << endl;
    }
    catch (const exception& e)
    {
        cerr << "错误: " << e.what() << endl;
    }

    return 0;
}

```

## 4. 复杂度分析

假设数组长度为 ，需要查找的个数为 。

### 时间复杂度：

* **建堆阶段**：前 K 个元素入堆，耗时 （如果使用 `build_heap` 方式可优化至 ，但在 STL `push` 方式下是 ）。
* **遍历调整阶段**：剩余  个元素，每个元素最坏情况下需要进行一次 `pop` 和 `push` 操作。每次堆调整的时间复杂度是 。
* **总计**：。
* **对比**：如果直接对整个数组排序（Quick Sort），时间复杂度是 。当  很大且  较小时（例如从 10 亿数据中取前 100 个），堆方法的效率远高于排序。

### 空间复杂度：

* 我们只维护了一个大小为  的优先队列，不需要将  个数据全部加载到内存中，非常适合处理流式数据或海量数据。