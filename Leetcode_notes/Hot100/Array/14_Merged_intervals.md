# LeetCode Hot 100: 合并区间 (Merge Intervals)

## 1\. 题目描述 [Original question](https://leetcode.cn/problems/merge-intervals/?envType=study-plan-v2&envId=top-100-liked)

**题目：** 给出一个区间的集合 `intervals`，其中 `intervals[i] = [start_i, end_i]`。请你合并所有重叠的区间，并返回一个不重叠的区间数组，该数组需恰好覆盖输入中的所有区间。

**示例 1：**

> **输入：** `intervals = [[1,3],[2,6],[8,10],[15,18]]`
> **输出：** `[[1,6],[8,10],[15,18]]`
> **解释：** 区间 `[1,3]` 和 `[2,6]` 重叠，将它们合并为 `[1,6]`。

**示例 2：**

> **输入：** `intervals = [[1,4],[4,5]]`
> **输出：** `[[1,5]]`
> **解释：** 区间 `[1,4]` 和 `[4,5]` 可被视为重叠区间。

-----

## 2\. 题目分析

这道题的核心难点在于：如果区间是无序的，我们很难判断任意两个区间是否重叠，可能需要 $O(n^2)$ 的时间复杂度去两两比较。

**破局点：排序**
如果我们按照每个区间的**左端点**进行升序排序，那么所有可能重叠的区间必然会在数组中连续出现。

排序后，我们只需要从前往后遍历一次：

1.  维护一个“当前正在合并的区间”。
2.  如果遍历到的下一个区间的左端点 $\le$ 当前区间的右端点，说明发生了重叠，我们需要更新当前区间的右端点为两者右端点的最大值。
3.  如果遍历到的下一个区间的左端点 $>$ 当前区间的右端点，说明不会再有重叠了（因为后面的区间左端点只会更大），将当前区间放入结果集，并开始维护新的区间。

-----

## 3\. 我的题解 (基础排序法)

这是你提供的解法，逻辑非常清晰，且已经是时间复杂度上的最优解法。

```cpp
class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        if(intervals.size()<=1)
            return intervals;
        
        vector<vector<int>> res;
        // 按照区间的左端点进行默认排序
        sort(intervals.begin(),intervals.end());
        
        for(int i=0;i<intervals.size();i++)
        {
            int l=intervals[i][0],r=intervals[i][1];
            // 如果结果数组为空，或者当前区间的左端点 > 结果集中最后一个区间的右端点
            // 说明没有重叠，直接将当前区间加入结果集
            if(res.empty()||res.back()[1]<l)
                res.push_back(intervals[i]);
            // 发生重叠，更新结果集中最后一个区间的右端点
            else
                res.back()[1]=max(res.back()[1],r);
        }
        return res;
    }
};
```

-----

## 4\. 进阶/更优解法 (原地修改，优化空间复杂度)

在掌握了标准解法后，可以进一步思考：如何在不牺牲时间复杂度的情况下，减少额外的内存开销？

在你的解法中，我们开辟了一个新的 `res` 数组来存放结果，这会带来 $O(n)$ 的空间消耗。如果我们被允许修改输入数组，我们可以使用\*\*双指针（快慢指针）\*\*直接在原数组 `intervals` 上进行合并操作，从而将额外空间复杂度降至 $O(1)$（不考虑排序本身的消耗）。

```cpp
class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        if (intervals.empty()) return {};

        // 依然需要先排序
        sort(intervals.begin(), intervals.end());

        // slow 指针指向当前已经合并好的最后一个区间
        int slow = 0; 
        
        // fast 指针用于遍历寻找需要合并的区间
        for (int fast = 1; fast < intervals.size(); ++fast) {
            // 如果 fast 指向的区间左端点 <= slow 指向的区间右端点，发生重叠
            if (intervals[fast][0] <= intervals[slow][1]) {
                // 原地合并：更新 slow 区间的右端点
                intervals[slow][1] = max(intervals[slow][1], intervals[fast][1]);
            } else {
                // 不重叠：将 slow 指针前移，并将 fast 指向的新区间复制过来
                slow++;
                intervals[slow] = intervals[fast];
            }
        }
        
        // 截断数组，保留 [0, slow] 部分即为最终结果
        intervals.resize(slow + 1);
        return intervals;
    }
};
```

-----

## 5\. 解法分析对比

| 评估维度 | 我的解法 (基础排序) | 进阶解法 (原地双指针) | 详细说明 |
| :--- | :--- | :--- | :--- |
| **时间复杂度** | $O(n \log n)$ | $O(n \log n)$ | 两者的性能瓶颈均在于 `std::sort`，遍历数组仅需 $O(n)$。时间复杂度处于同一量级。 |
| **空间复杂度** | $O(n)$ | $O(1)$ | (注：此处未计算排序底层实现带来的 $O(\log n)$ 栈空间)。你的解法需要额外开辟 `res` 数组；进阶解法复用了原数组内存，空间利用率更高。 |
| **适用场景** | 绝大多数常规场景，且不破坏原始输入数据。 | 内存极度受限，且允许覆盖/破坏原始输入数据的场景。 |

-----

## 6\. 总结

1.  **算法套路积累**：遇到“区间”、“重叠”、“时间安排”类问题时，第一反应应该是**对起点或终点进行排序**。这是一种极为常见的降维打击手段，能将 $O(n^2)$ 的嵌套循环比对降为 $O(n \log n)$ 的排序加一次线性遍历。
2.  **C++ 语言特性应用**：在我的代码中，使用了 `res.back()[1]` 直接获取并修改容器尾部元素的引用，这是非常地道的 C++ 写法，避免了多余的拷贝操作。
3.  **能力进阶方向**：后续刷题时，如果发现时间复杂度已经无法进一步优化，可以尝试去榨干空间复杂度，例如利用原数组进行状态记录或数据覆盖。