[MindMap](./Binary_search.png)


# C++ 算法笔记：二分查找 (Binary Search)

## 1. 核心概念与前提条件

二分查找是一种在**有序数组**中查找某一特定元素的搜索算法。搜索过程从数组的中间元素开始，如果中间元素正好是要查找的元素，则搜索过程结束；如果某一特定元素大于或者小于中间元素，则在数组大于或小于中间元素的那一半中查找，而且跟开始一样从中间元素开始比较。

### 前提条件 (Prerequisites)

根据思维导图及算法特性，二分查找的使用需要满足以下条件：

* **单调性**：数组必须是排好序的（升序或降序）。如果是无序数组，必须先进行排序。
* **随机访问**：数据结构必须支持 $O(1)$ 的随机访问（如数组、`std::vector`）。链表结构由于不支持随机访问，不适合直接使用二分查找。
* **无溢出风险**：计算中间位置 `mid` 时，需防范整数溢出问题。

### 复杂度分析 (Complexity Analysis)

* **时间复杂度**：$O(\log n)$ 。每次比较都将搜索范围缩小一半。
* **空间复杂度**：$O(1)$ 。采用迭代写法仅需常数级别的额外空间。

---

## 2. 算法分类与实现逻辑

### 2.1 普通二分查找 (Standard Binary Search)

* **场景**：数组中**不含重复元素**，查找目标值是否存在。
* **区间定义**：通常采用**左闭右闭**区间 `[left, right]`。
* **循环条件**：`left <= right`。
* **指针更新**：
* `target < nums[mid]`  `right = mid - 1`
* `target > nums[mid]`  `left = mid + 1`



### 2.2 二分查找插入点 (Insertion Point)

* **场景**：在有序数组中找到 `target` 应该插入的位置，以保持数组有序。
* **等价逻辑**：这等同于查找**第一个大于等于** `target` 的元素的下标（即 C++ STL 中的 `std::lower_bound` 语义）。
* **处理重复元素**：
* 如果数组**不含**重复元素：找到第一个大于 `target` 的位置。
* 如果数组**含**重复元素：找到第一个等于 `target` 的位置（左边界）。



### 2.3 二分查找边界 (Finding Boundaries)

这是二分查找的难点，也是思维导图中强调的重点。

#### 查找左边界 (Left Boundary)

* **目标**：寻找数组中第一个等于 `target` 的元素的下标。
* **策略**：当 `nums[mid] == target` 时，不要立即返回，而是**收缩右边界** (`right = mid - 1`)，继续在左半部分搜索，尝试找到更靠左的相等元素。

#### 查找右边界 (Right Boundary) - 复用策略

* **目标**：寻找数组中最后一个等于 `target` 的元素的下标。
* **思维导图核心技巧**：**复用查找左边界的逻辑**。
* **转化公式**：
$$\text{RightBoundary}(target) = \text{LeftBoundary}(target + 1) - 1$$

* **解释**：
1. 查找 `target` 的右边界，本质上就是找“大于 `target` 的第一个数字”的前一个位置。
2. 对于整数数组，“大于 `target` 的第一个数字”等价于 “大于等于 `target + 1` 的第一个数字”。
3. 因此，我们可以直接调用“查找左边界”的函数，传入 `target + 1`，然后将结果减 1 即可。这样做的好处是**只需要维护一套核心代码**，减少了编写不同变体时的逻辑错误。



---

## 3. C++ 代码实现 (可以直接运行)

以下代码采用 C++ 标准写法，封装在一个完整的示例文件中。代码中严格处理了边界条件和溢出问题。

```cpp
/**
 * BinarySearchNotes.cpp
 * * 功能：二分查找算法合集
 * 包含：
 * 1. 标准二分查找 (存在性检查)
 * 2. 查找左边界 (lower_bound / 插入点)
 * 3. 查找右边界 (利用左边界逻辑复用)
 */

#include <iostream>
#include <vector>
#include <algorithm> // 用于验证结果

using namespace std;

class BinarySearchSolution {
public:
    /**
     * 1. 普通二分查找
     * 场景：数组有序，无重复元素
     * 返回：目标值的下标，不存在返回 -1
     */
    int standardBinarySearch(const vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;

        // 采用 [left, right] 闭区间写法
        while (left <= right) {
            // 防止 (left + right) 溢出
            int mid = left + (right - left) / 2;

            if (nums[mid] == target) {
                return mid; // 找到目标，直接返回
            } else if (nums[mid] < target) {
                left = mid + 1; // 目标在右区间
            } else {
                right = mid - 1; // 目标在左区间
            }
        }
        return -1; // 未找到
    }

    /**
     * 2. 查找左边界 (核心函数)
     * 语义：寻找第一个 >= target 的元素的下标
     * * 应用：
     * - 如果 target 存在，返回第一个 target 的下标
     * - 如果 target 不存在，返回 target 应当插入的位置 (Insertion Point)
     * - 如果所有元素都小于 target，返回 nums.size()
     */
    int searchLeftBoundary(const vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;
        
        // 记录结果，默认为 nums.size() (表示插入到末尾或未找到)
        int ans = nums.size(); 

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (nums[mid] >= target) {
                ans = mid;      // 记录当前位置，可能是答案
                right = mid - 1; // 继续向左收缩，看左边是否还有符合条件的
            } else {
                left = mid + 1;
            }
        }
        return ans;
    }

    /**
     * 3. 查找右边界
     * 技巧：复用 searchLeftBoundary
     * 逻辑：查找最后一个 target 的位置，等同于查找 (target + 1) 的左边界位置减 1
     */
    int searchRightBoundary(const vector<int>& nums, int target) {
        // 步骤 1: 找 target + 1 的左边界
        int leftOfNext = searchLeftBoundary(nums, target + 1);
        
        // 步骤 2: 减 1 得到 target 的右边界
        int rightBoundary = leftOfNext - 1;

        // 步骤 3: 校验合法性
        // 如果 rightBoundary < 0，说明数组所有元素都比 target 大
        // 如果 nums[rightBoundary] != target，说明 target 根本不存在于数组中
        if (rightBoundary < 0 || nums[rightBoundary] != target) {
            return -1;
        }
        
        return rightBoundary;
    }
};

int main() {
    BinarySearchSolution solution;

    // 测试数据：含重复元素的有序数组
    vector<int> nums = {1, 3, 3, 3, 5, 7};
    int target = 3;

    cout << "数组: {1, 3, 3, 3, 5, 7}, Target: " << target << endl;
    cout << "----------------------------------------" << endl;

    // 1. 测试左边界
    int leftIdx = solution.searchLeftBoundary(nums, target);
    cout << "左边界 (第一个 " << target << " 的位置): " << leftIdx << endl;
    // 预期输出: 1

    // 2. 测试右边界 (复用逻辑)
    int rightIdx = solution.searchRightBoundary(nums, target);
    cout << "右边界 (最后一个 " << target << " 的位置): " << rightIdx << endl;
    // 预期输出: 3

    // 3. 测试插入点 (查找不存在的元素 4)
    int insertVal = 4;
    int insertIdx = solution.searchLeftBoundary(nums, insertVal);
    cout << "元素 " << insertVal << " 的插入点: " << insertIdx << endl;
    // 预期输出: 4 (即元素 5 的位置)

    return 0;
}

```

---

## 4. 关键细节总结 (Key Takeaways)

1. **区间选择**：初学者建议统一使用 `while(left <= right)` 的闭区间写法，思维负担最小。
2. **防溢出**：永远使用 `mid = left + (right - left) / 2` 而不是 `(left + right) / 2`。
3. **复用思想**：不要为了找左边界写一个函数，找右边界又重写一个逻辑略有不同的函数。利用 **“查找第一个 `>= target` 的位置”** 这一核心逻辑，可以解决绝大多数二分问题：
* 找左边界  直接调用。
* 找右边界  找 `target + 1` 的左边界位置减 1。
* 找插入点  直接调用。