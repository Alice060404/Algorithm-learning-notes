[MIndMap](./Bucket_sort.png)


# 深入理解桶排序 (Bucket Sort)

桶排序是一种分布式排序算法。它的核心思想是将要排序的数据分到几个有序的“桶”里，每个桶里的数据再单独进行排序。桶内排完序之后，再把每个桶里的数据按照顺序依次取出，组成的序列就是有序的了。

桶排序可以看作是**计数排序的升级版**。计数排序要求每个桶只存储一个特定的值，而桶排序允许每个桶存储一定范围内的值。

---

## 一、 算法原理详细步骤

桶排序的执行流程可以抽象为“Scatter-Gather”（分散-聚集）模型：

1. **初始化 $k$ 个桶**
   * 根据输入数据的取值范围（最大值和最小值）和数据量 $n$，预先划分出 $k$ 个区间，每个区间对应一个“桶”（通常在 C++ 中使用 `std::vector<std::vector<int>>` 来表示）。

2. **分配元素到桶中 (Scatter)**
   * 遍历原始数组，利用一个**映射函数 (Mapping Function)** 将每个元素计算出一个索引值，并放入对应的桶中。映射函数的设计直接决定了桶排序的效率。

3. **对每个桶内部执行排序**
   * 对每个非空的桶进行独立的排序。这里通常采用编程语言内置的排序算法，例如 C++ 的 `std::sort`（底层为内省式排序），或者针对小数据量采用插入排序。

4. **合并结果 (Gather)**
   * 按照桶的索引顺序（从小到大），依次将各个桶内的元素追加到原数组中，完成整个排序过程。

---

## 二、 算法特性分析

* **时间复杂度：最佳 $O(n + k)$，最坏 $O(n \log n)$ 或 $O(n^2)$**
  * **最佳/平均情况 $O(n + k)$**：当 $n$ 个元素被均匀地分配到 $k$ 个桶中时，每个桶大约有 $n/k$ 个元素。如果桶内采用 $O(m \log m)$ 的排序算法，总排序时间为 $k \cdot (n/k \log(n/k)) = n \log(n/k)$。当 $k$ 接近 $n$ 时，时间复杂度趋近于 $O(n)$。加上分配和合并的 $O(n)$ 时间，总时间复杂度为 $O(n + k)$。
  * **最坏情况**：当所有元素全部分配到了同一个桶中，桶排序退化为该桶内使用的比较排序算法的时间复杂度，例如 $O(n \log n)$（用 `std::sort`）或 $O(n^2)$（用插入排序）。

* **空间复杂度：$O(n + k)$**
  * **非原地排序 (Out-of-place)**：需要额外创建 $k$ 个桶，并且这 $k$ 个桶总共需要存储 $n$ 个元素。因此空间复杂度为 $O(n + k)$。这也是桶排序以空间换时间思想的体现。

* **稳定性：取决于桶内排序算法**
  * 桶排序本身的分配和收集过程是不会改变相同元素原始顺序的。因此，**桶排序是否稳定，完全取决于第 3 步中桶内排序使用的算法**。
  * 如果桶内使用稳定的**插入排序**或 `std::stable_sort`，则整体桶排序是**稳定**的。
  * 如果桶内使用非稳定的快速排序或 `std::sort`，则整体桶排序是**非稳定**的。

---

## 三、 核心探讨：如何实现平均分配以达到 O(n) 时间复杂度？

思维导图中提到了这个关键问题。要让桶排序达到线性的时间复杂度，核心在于**避免数据倾斜，确保数据均匀分布在各个桶中**。

1. **前提假设：数据分布相对均匀**
   * 桶排序最适合处理那些在特定范围内**均匀分布**的数据（例如：随机生成的浮点数、考试成绩等）。如果数据极端聚集（例如：99个元素是 1，1个元素是 10000），常规的桶划分会导致大部分元素落入同一个桶。

2. **设计合理的映射函数和动态计算桶数量**
   * 不能简单地固定桶的数量。通常需要先遍历一遍数组，找到最大值 `max_val` 和最小值 `min_val`。
   * 设定一个合理的**桶容量 (Bucket Size)**，例如设为 5。
   * **动态计算桶数量 $k$**：$k = \lfloor \frac{max\_val - min\_val}{bucket\_size} \rfloor + 1$。
   * **映射函数**：对于任意元素 $x$，其对应的桶索引计算公式为：$index = \lfloor \frac{x - min\_val}{bucket\_size} \rfloor$。这个公式保证了数值范围等距划分，有助于均匀散列。

---

## 四、 C++ 代码实现

下面是一个健壮的桶排序 C++ 实现，采用了动态计算桶数量和映射函数的策略，桶内排序采用 C++ 标准库的高效排序。

```cpp
#include <iostream>
#include <vector>
#include <algorithm> // 包含 std::min_element, std::max_element, std::sort

using namespace std;

/**
 * @brief 桶排序实现
 * @param arr 待排序的数组引用
 * @param bucketSize 每个桶的预期数据跨度（非桶的绝对容量），默认值为 5
 */
void bucketSort(vector<int>& arr, int bucketSize = 5) {
    if (arr.empty() || arr.size() < 2) {
        return;
    }

    // 1. 寻找数组中的最大值和最小值
    int min_val = *min_element(arr.begin(), arr.end());
    int max_val = *max_element(arr.begin(), arr.end());

    // 2. 动态计算需要的桶数量 (k)
    // 确保 (max - min) / size 能够覆盖所有范围，+1 是为了处理边界
    int bucketCount = (max_val - min_val) / bucketSize + 1;
    
    // 初始化桶，外层 vector 表示所有桶，内层 vector 表示具体的某一个桶
    vector<vector<int>> buckets(bucketCount);

    // 3. 将元素分配到各个桶中 (Scatter)
    for (int i = 0; i < arr.size(); i++) {
        // 核心：映射函数，计算当前元素应该放入哪个桶的索引
        int bucketIndex = (arr[i] - min_val) / bucketSize;
        buckets[bucketIndex].push_back(arr[i]);
    }

    // 4. 对每个非空的桶进行内部排序，并回写原数组 (Gather)
    int arrIndex = 0; // 记录原数组回写到的位置
    for (int i = 0; i < buckets.size(); i++) {
        if (buckets[i].empty()) {
            continue; // 跳过空桶
        }
        
        // 桶内排序，这里使用标准库的 std::sort (底层是 IntroSort，非稳定)
        // 如果需要稳定排序，可以换成 std::stable_sort
        sort(buckets[i].begin(), buckets[i].end());

        // 将排序好的桶内元素依次按序写回原数组
        for (int j = 0; j < buckets[i].size(); j++) {
            arr[arrIndex++] = buckets[i][j];
        }
    }
}

int main() {
    // 测试用例：包含重复元素和无序分布
    vector<int> data = {29, 25, 3, 49, 9, 37, 21, 43, 11, 29};
    
    cout << "排序前的数组: ";
    for (int num : data) {
        cout << num << " ";
    }
    cout << "\n";

    // 执行桶排序
    bucketSort(data);

    cout << "排序后的数组: ";
    for (int num : data) {
        cout << num << " ";
    }
    cout << "\n";

    return 0;
}

```