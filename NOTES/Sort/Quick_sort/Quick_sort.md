[MindMap](./Quick_sort.png)


# 算法笔记：快速排序 (Quick Sort)

## 1. 核心思想与工作原理

快速排序（Quick Sort）是一种基于**分治法（Divide and Conquer）**策略的高效排序算法。其核心逻辑可以概括为：“选定基准，左右划分，递归求解”。

### 1.1 哨兵划分 (Sentinel Partition)

这是快排的灵魂步骤。

* **目标**：在数组中选取一个元素作为**基准数（Pivot）**，通过移动元素，使得基准数左边的所有元素都小于等于它，右边的所有元素都大于等于它。
* **过程**：通常使用双指针（哨兵 `i`, `j`），分别从数组两端向中间扫描。
* `j` 从右向左找比基准小的数。
* `i` 从左向右找比基准大的数。
* 交换 `i` 和 `j` 指向的元素，直到 `i` 与 `j` 相遇。
* 最后将基准数与相遇点的元素交换，完成一轮划分。



### 1.2 递归排序 (Recursive Sort)

完成一轮划分后，数组被基准数切分为两个子数组（左子数组和右子数组）。

* 对这两个子数组分别重复上述“哨兵划分”的过程。
* **终止条件**：当子数组长度为 0 或 1 时，即认为该部分已有序。

---

## 2. 算法特点深度解析

作为大一学生，不仅要会写，更要理解其背后的性能指标，这通常是数据结构考试或面试的重点。

### 2.1 复杂度分析

* **时间复杂度**：
* **平均情况**：$O(n \log n)$ 。
* **最差情况**：$O(n^2)$ 。通常发生在数组已经有序（正序或逆序），且每次都选择第一个元素作为基准时，此时递归树退化为链表。


* **空间复杂度**：
* **平均情况**：$O(\log n)$ 。主要来源于递归调用栈的深度。
* **最差情况**：$O(n)$ 。递归树退化时。



### 2.2 稳定性与原地性

* **原地排序 (In-place)**：是。不需要像归并排序那样开辟额外的 $O(n)$ 辅助空间，仅需常数级的栈空间和交换变量。
* **非稳定排序 (Unstable)**：是。
* **原因**：在哨兵划分进行长距离交换时，可能会把相同元素原本的相对顺序打乱。例如序列 `[3, 3*, 1]`，以第一个3为基准，1和第一个3交换后变成 `[1, 3*, 3]`，两个3的相对位置改变了。



### 2.3 为什么快排通常优于归并和堆排序？

尽管堆排序和归并排序的最差时间复杂度都是稳定的 $O(n \log n)$ ，但在实际工业界应用（如 C++ STL 的 `std::sort`）中，快排往往是首选，原因如下：

1. **缓存使用效率高 (Cache Locality)**：快排的遍历是顺序扫描内存的，拥有极好的空间局部性，能大幅提高 CPU Cache 的命中率。相比之下，堆排序需要在大跨度的索引间跳跃（父节点到子节点），对缓存不友好。
2. **常数系数小**：快排内部循环极其简单（仅仅是比较和指针自增自减），指令开销极小。
3. **最差情况概率低**：通过随机化基准或“三数取中”优化，出现 $O(n^2)$ 的概率在实际应用中几乎可以忽略。

---

## 3. 核心优化策略

针对快排可能退化的问题，通常采取以下两种优化手段（也是本次代码实现的重点）：

### 3.1 基准数优化：三数取中法 (Median-of-Three)

* **痛点**：如果每次盲目选择数组第一个元素作为基准，当数组有序时会退化。
* **策略**：在数组的**首部**、**中间**、**尾部**选取三个元素，取这三个数的**中位数**作为基准。
* **效果**：保证划分出的两个子数组长度尽量均衡，避免递归树倾斜。

### 3.2 递归深度优化 (Introspection / Tail Call Optimization)

* **痛点**：当递归层级过深时，可能导致栈溢出（Stack Overflow）。
* **策略**：
1. 在一轮划分结束后，比较切分出来的左、右两个子数组的长度。
2. **仅对较短的子数组进行递归**。
3. 对较长的子数组，不使用递归，而是通过更新循环变量（尾递归消除的思想）在当前函数栈帧内继续处理。


* **效果**：严格限制递归栈的深度在 $O(\log n)$ 级别。

---

## 4. C++ 代码实现 (Production Ready)


```cpp
/**
 * @file quick_sort_optimized.cpp
 * @brief 快速排序优化版实现 (C++ Note)
 * @details 
 * 包含两项核心优化：
 * 1. 基准数选取优化：三数取中法 (Median-of-Three)
 * 2. 空间复杂度优化：尾递归优化/短子区间优先 (保证栈深度 O(logN))
 */

#include <iostream>
#include <vector>
#include <algorithm> // for std::swap

// 辅助函数：三数取中，选择 pivot 并将其交换到 left 位置
// 返回值：选取好的基准数的值
int medianThree(std::vector<int>& nums, int left, int right) {
    int mid = left + (right - left) / 2;
    
    // 目标：将 left, mid, right 三个位置的数排好序
    // 使得 nums[left] <= nums[mid] <= nums[right]
    // 这样 mid 位置就是中位数
    if (nums[left] > nums[mid]) std::swap(nums[left], nums[mid]);
    if (nums[left] > nums[right]) std::swap(nums[left], nums[right]);
    if (nums[mid] > nums[right]) std::swap(nums[mid], nums[right]);
    
    // 此时 nums[mid] 是中位数，为了方便后续的 standard partition 操作，
    // 我们通常将中位数交换到数组的最左端或者最右端（这里交换到最左端 left）
    std::swap(nums[left], nums[mid]);
    
    return nums[left];
}

// 核心函数：哨兵划分 (Partition)
int partition(std::vector<int>& nums, int left, int right) {
    // 步骤1：优化基准数选取
    int pivot = medianThree(nums, left, right);
    
    int i = left;
    int j = right;
    
    while (i < j) {
        // 步骤2：j 从右向左找小于 pivot 的数
        while (i < j && nums[j] >= pivot) {
            j--;
        }
        // 步骤3：i 从左向右找大于 pivot 的数
        while (i < j && nums[i] <= pivot) {
            i++;
        }
        // 步骤4：交换
        if (i < j) {
            std::swap(nums[i], nums[j]);
        }
    }
    
    // 步骤5：将基准数归位（此时 i 和 j 相遇）
    std::swap(nums[left], nums[i]);
    
    // 返回基准数的最终索引
    return i;
}

// 快速排序主函数 (带递归深度优化)
void quickSort(std::vector<int>& nums, int left, int right) {
    // 使用 while 循环配合单边递归来消除长半区的递归调用
    while (left < right) {
        // 获取基准数索引
        int pivotIndex = partition(nums, left, right);
        
        // 优化策略：比较左右子数组的长度，优先递归处理较短的子数组
        // 这样可以保证递归栈的深度不会超过 O(logN)
        
        // 左半区长度：pivotIndex - 1 - left
        // 右半区长度：right - (pivotIndex + 1)
        
        if (pivotIndex - left < right - pivotIndex) {
            // 左边更短，先递归左边
            quickSort(nums, left, pivotIndex - 1);
            // 剩下的部分是右边 (pivotIndex + 1, right)
            // 更新 left 指针，在下一次 while 循环中处理右边（模拟尾递归）
            left = pivotIndex + 1;
        } else {
            // 右边更短，先递归右边
            quickSort(nums, pivotIndex + 1, right);
            // 剩下的部分是左边 (left, pivotIndex - 1)
            // 更新 right 指针，在下一次 while 循环中处理左边
            right = pivotIndex - 1;
        }
    }
}

// 包装函数，方便调用
void sortArray(std::vector<int>& nums) {
    if (nums.empty()) return;
    quickSort(nums, 0, nums.size() - 1);
}

// 测试用例
int main() {
    std::vector<int> nums = {10, 7, 8, 9, 1, 5, 2, 4, 3, 6};
    
    std::cout << "Original array: ";
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n";
    
    sortArray(nums);
    
    std::cout << "Sorted array:   ";
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n";
    
    return 0;
}

```