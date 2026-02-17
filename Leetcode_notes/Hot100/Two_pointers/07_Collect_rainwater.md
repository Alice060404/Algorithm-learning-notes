# LeetCode 42. 接雨水 (Trapping Rain Water)

## 1. 题目描述 [Original question](https://leetcode.cn/problems/trapping-rain-water/?envType=study-plan-v2&envId=top-100-liked)

给定 `n` 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。

**示例 1：**

> **输入：** height = [0,1,0,2,1,0,1,3,2,1,2,1]
> **输出：** 6
> **解释：** 上面是由数组 [0,1,0,2,1,0,1,3,2,1,2,1] 表示的高度图，在这种情况下，可以接 6 个单位的雨水（蓝色部分表示雨水）。

**示例 2：**

> **输入：** height = [4,2,0,3,2,5]
> **输出：** 9

## 2. 题目分析

解决此类问题需要从物理直觉抽象出数学模型。

这道题的核心原理是**“木桶效应”**。对于数组中任意位置 `i` 的柱子，它能接到的雨水高度，取决于它**左边最高**的柱子和**右边最高**的柱子。

具体来说，位置 `i` 处的存水量计算公式为：
$$\text{water}[i] = \min(\text{max\_left}[i], \text{max\_right}[i]) - \text{height}[i]$$


如果计算结果小于 0，则说明该位置无法存水（即 `water[i] = 0`）。

因此，解题的关键在于如何高效地寻找每个位置的左右最大高度。

## 3. 我的解法

### 3.1 初版解法：按列求（暴力法）

这是最直观的思路：遍历每一列，对于当前列，分别向左和向右扫描找到最高的墙，然后套用公式。

```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        int n=height.size();
        int sum=0;
        // 这里的循环范围 i 从 1 到 n-2，因为第一个和最后一个柱子注定无法接水
        for(int i=1;i<n-1;i++)
        {
            int left=i,right=i;
            int leftMax=height[left],rightMax=height[right];
            // 向左寻找最大值
            while(left>0)
            {
                left--;
                leftMax=max(leftMax,height[left]);
            }
            // 向右寻找最大值
            while(right<n-1)
            {
                right++;
                rightMax=max(rightMax,height[right]);
            }
            // 计算当前列的水量
            sum+=min(leftMax,rightMax)-height[i];
        }
        return sum;
    }
};

```

**分析：**

* **结果：** 超出时间限制 (Time Limit Exceeded)。
* **时间复杂度：**$O(N^2)$ 。对于每个元素，我们都遍历了整个数组。当 $N$ 很大时，计算量会急剧增加。
* **空间复杂度：**$O(1)$ 。只使用了常数个变量。

### 3.2 进阶解法：双指针法（最优解）

为了降低时间复杂度，我们需要避免重复查找。利用双指针技巧，我们可以将空间复杂度保持在 $O(1)$ 的同时，将时间复杂度降低到 $O(N)$ 。

**核心逻辑：**
如果 `leftMax < rightMax`，那么对于 `left` 指针指向的位置，其右侧必然存在一个不低于 `rightMax` 的柱子，因此其短板必然由左侧决定（即 `leftMax`）。反之亦然。

```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        int n=height.size();
        int sum=0;
        int left=0,right=n-1;
        int leftMax=0,rightMax=0;
        
        while(left<right)
        {
            // 每次循环更新当前左右两侧已知的最大值
            leftMax=max(leftMax,height[left]);
            rightMax=max(rightMax,height[right]);
            
            // 短板在左边，处理左边
            if(leftMax<=rightMax)
            {
                // 因为 rightMax 即使更远，也比 leftMax 大，
                // 所以 min(leftMax, rightMax) 一定是 leftMax
                sum+=leftMax-height[left];
                left++;
            }
            // 短板在右边，处理右边
            else
            {
                sum+=rightMax-height[right];
                right--;
            }
        }
        return sum;
    }
};

```

**分析：**

* **结果：** 通过 (Accepted)。
* **时间复杂度：**$O(N)$ 。双指针相遇即结束，每个元素仅访问一次。
* **空间复杂度：**$O(1)$ 。仅使用了几个辅助变量，是空间最优解。

---
### 为什么这段代码能成立？

假设我们在遍历过程中，左指针在 `left` 位置，右指针在 `right` 位置。
此时，`leftMax` 记录的是 `left` 及其左边所有柱子的最大高度；`rightMax` 记录的是 `right` 及其右边所有柱子的最大高度。

代码中最关键的判断是 `if(leftMax <= rightMax)`，它的背后蕴含着严密的逻辑推导：

#### 情况 A：`leftMax <= rightMax`

当我们发现左边的最高点 `leftMax` 小于或等于右边的最高点 `rightMax` 时，**对于 `left` 指针所在的位置来说，它的“短板”已经确定了，就是 `leftMax`。**

为什么？因为：

* `left` 的左侧最高绝对是 `leftMax`。
* `left` 的右侧最高是多少我们暂时不知道（因为中间还有没遍历到的柱子），但是，我们明确知道右边**至少**存在一个高度为 `rightMax` 的柱子。
* 既然 `leftMax <= rightMax`，那么无论中间未遍历的部分有没有更高的柱子，`left` 位置两侧的较小值（也就是木桶的短板）必定是 `leftMax`。

因此，我们可以直接计算 `left` 位置的水量：`leftMax - height[left]`，然后将 `left` 指针安心地向右移动 (`left++`)。

#### 情况 B：`leftMax > rightMax`

反之亦然。如果 `leftMax > rightMax`，说明对于 `right` 指针所在的位置，它的短板必定是 `rightMax`。中间哪怕有通天塔，也不会影响 `right` 位置的水位上限。

因此，我们可以直接计算 `right` 位置的水量：`rightMax - height[right]`，然后将 `right` 指针向左移动 (`right--`)。

---
## 4. 其他可能的解法

除了双指针法，还有两种经典解法值得掌握，它们体现了不同的算法思想。

### 4.1 动态规划 (Dynamic Programming)

为了避免暴力法中的重复计算，我们可以使用两个数组 `max_left[i]` 和 `max_right[i]` 分别记录位置 `i` 左边和右边的最大高度。这是一种典型的“空间换时间”策略。

```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        int n = height.size();
        if (n == 0) return 0;

        vector<int> leftMax(n);
        vector<int> rightMax(n);

        // 初始化左侧最大值数组
        leftMax[0] = height[0];
        for (int i = 1; i < n; i++) {
            leftMax[i] = max(leftMax[i - 1], height[i]);
        }

        // 初始化右侧最大值数组
        rightMax[n - 1] = height[n - 1];
        for (int i = n - 2; i >= 0; i--) {
            rightMax[i] = max(rightMax[i + 1], height[i]);
        }

        int sum = 0;
        for (int i = 0; i < n; i++) {
            sum += min(leftMax[i], rightMax[i]) - height[i];
        }
        return sum;
    }
};

```

**复杂度：**

* 时间复杂度：$O(N)$ 。需要遍历三次数组。
* 空间复杂度：$O(N)$ 。需要额外的数组存储预处理信息。

### 4.2 单调栈 (Monotonic Stack)

这种方法不是按列计算，而是按“层”计算。维护一个单调递减的栈，当遇到一个比栈顶元素高的柱子时，说明形成了一个凹槽，可以计算一段积水。

```cpp
class Solution {
public:
    int trap(vector<int>& height) {
        int sum = 0;
        stack<int> st; // 存储的是下标
        int n = height.size();
        
        for (int i = 0; i < n; i++) {
            // 当当前柱子高度大于栈顶柱子高度时，说明出现了凹槽
            while (!st.empty() && height[i] > height[st.top()]) {
                int top = st.top();
                st.pop();
                
                if (st.empty()) break; // 左边没有墙，无法接水
                
                int left = st.top();
                int currentWidth = i - left - 1;
                int currentHeight = min(height[left], height[i]) - height[top];
                
                sum += currentWidth * currentHeight;
            }
            st.push(i);
        }
        return sum;
    }
};

```

**复杂度：**

* 时间复杂度：$O(N)$。每个元素最多入栈和出栈一次。
* 空间复杂度：$O(N)$。栈的空间。

## 5. 总结

在刷题过程中，我们经历了从直觉到优化的完整过程：

1. **暴力法：** 思路简单直接，但 $O(N^2)$ 的复杂度在数据量大时不可接受。
2. **动态规划：** 利用预处理数组解决了重复计算问题，时间优化至 $O(N)$ ，但付出了 $O(N)$ 的空间代价。
3. **双指针法：** 结合了上述优点，利用左右指针交替收缩的特性，在 $O(N)$ 时间内完成计算，且空间复杂度仅为 $O(1)$ ，是本题的**最佳工程解法**。
4. **单调栈：** 提供了一种完全不同的“横向”切分视角，在处理某些特定的区间极值问题时非常有用。