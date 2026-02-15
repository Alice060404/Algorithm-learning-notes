# LeetCode Hot 100 刷题笔记：最长连续序列

## 题目描述 [Original question](https://leetcode.cn/problems/longest-consecutive-sequence/description/?envType=study-plan-v2&envId=top-100-liked)

**题目：** 最长连续序列 (LeetCode 128)

**描述：**
给定一个未排序的整数数组 `nums` ，找出数字连续的最长序列（不要求序列元素在原数组中连续）的长度。

**进阶要求：**
请你设计并实现时间复杂度为 $O(n)$ 的算法解决此问题。

**示例：**

* 输入：`nums = [100, 4, 200, 1, 3, 2]`
* 输出：`4`
* 解释：最长数字连续序列是 `[1, 2, 3, 4]`。它的长度为 4。

---

## 分析

在数据结构与算法的入门学习阶段，处理“未排序”且需要查找“连续/大小关系”的问题时，第一直觉通常是先对数组进行排序。然而，基于比较的排序算法（如快速排序、归并排序）的平均时间复杂度为 $O(n \log n)$ ，这无法满足题目严格的 $O(n)$ 线性时间复杂度要求。

为了突破 $O(n \log n)$ 的限制，我们需要采用**“空间换时间”**的核心策略。C++ 标准库中的哈希表（`unordered_set`）是实现这一策略的绝佳工具。哈希表能够在 $O(1)$ 的平均时间复杂度内完成元素的查找与插入。

这道题的核心逻辑在于：**如何避免重复计算？**
如果已知一个数字是某个序列的一部分，我们不需要从这个数字开始向后匹配，而是应该找到这个序列的**起点**。
对于数组中的任意数字 `num`，如果 `num - 1` 存在于哈希表中，说明 `num` 只是某个序列的中间节点，直接跳过即可；只有当 `num - 1` 不存在时，`num` 才是当前连续序列的起点。从起点开始不断匹配 `num + 1`，记录最大长度，这样就能确保每个数字最多只被访问两次（一次存入哈希表，一次在 `while` 循环中被匹配），从而实现严格的 $O(n)$ 时间复杂度。

---

## 我的解法

符合题目 $O(n)$ 时间复杂度的要求，通过 `!map.count(num-1)` 解题


```cpp
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        // 将所有元素存入哈希集合，自动去重并提供 O(1) 的查询效率
        unordered_set<int> map(nums.begin(), nums.end());
        int maxlen = 0;
        
        for(const int& num : map) {
            // 核心剪枝逻辑：只有当当前数字是序列的起点时才开始计算
            if(!map.count(num - 1)) {
                int curNum = num;
                int curLen = 1;
                
                // 不断向后寻找连续的数字
                while(map.count(curNum + 1)) {
                    curNum++;
                    curLen++;
                }
                
                // 更新最长序列的长度
                maxlen = max(maxlen, curLen);
            }
        }
        return maxlen;
    }
};

```

**复杂度分析：**

* **时间复杂度：*$O(n)$* 。虽然 `for` 循环内部嵌套了 `while` 循环，但由于有 `if(!map.count(num-1))` 的限制，`while` 循环只会在序列的起点处触发。数组中的每个元素最多被遍历两次（外层循环一次，内层 `while` 匹配一次），因此整体时间复杂度为线性。
* **空间复杂度：*$O(n)$* 。使用了一个 `unordered_set` 来存储数组中的元素，最坏情况下（所有元素互不相同）需要 $O(n)$ 的额外空间。

*提示：在 C++ 中，`map` 通常指的是键值对映射（`std::map` 或 `std::unordered_map`），而这里使用的是集合（`std::unordered_set`）。在日常编码中，将变量名命名为 `hashSet` 或 `numSet` 会更符合语意规范，有助于养成良好的工程习惯。*

---

## 其他可能的解法：排序法

虽然题目要求 $O(n)$ 的时间复杂度，但在实际面试或笔试中，如果一时想不出哈希表的解法，或者题目对空间复杂度有极高要求（要求 $O(1)$ 额外空间），排序法是一个非常扎实的保底方案，也是必须掌握的基本功。

排序法会将相同的元素放在相邻位置，将连续的元素也放在相邻位置。我们只需要遍历一次排序后的数组，跳过重复元素，并统计连续元素的长度即可。

```cpp
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        if (nums.empty()) {
            return 0;
        }
        
        // 先对数组进行升序排序，时间复杂度 O(n log n)
        sort(nums.begin(), nums.end());
        
        int longestStreak = 1;
        int currentStreak = 1;
        
        for (int i = 1; i < nums.size(); i++) {
            // 跳过重复元素
            if (nums[i] != nums[i-1]) {
                // 如果当前元素与前一个元素连续
                if (nums[i] == nums[i-1] + 1) {
                    currentStreak += 1;
                } else {
                    // 如果不连续，重置当前连续长度
                    longestStreak = max(longestStreak, currentStreak);
                    currentStreak = 1;
                }
            }
        }
        
        return max(longestStreak, currentStreak);
    }
};

```

**复杂度分析：**

* **时间复杂度：**$O(n \log n)$* 。排序的开销主导了总时间复杂度。
* **空间复杂度：**$O(1)$ 或 $O(\log n)$。取决于 C++ 标准库 `sort` 函数的底层实现（通常是内省排序），不需要像哈希表那样开辟 $O(n)$ 的堆空间。

---

## 总结

1. **算法思想：** 本题是演示**空间换时间**思想的经典例题。面对查找和去重需求，优先考虑哈希结构。
2. **代码技巧：** 寻找连续序列或连通块时，寻找“起点”进行单向遍历，是避免重复计算（即剪枝）、降低时间复杂度的通用技巧。
3. **基础功底：** 熟悉 C++ STL 中 `unordered_set` 的基本 API（如构造函数初始化、`count` 查找等），这对于提升算法题的解题速度至关重要。