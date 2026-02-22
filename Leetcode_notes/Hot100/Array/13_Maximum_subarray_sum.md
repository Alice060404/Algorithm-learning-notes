# LeetCode 53. 最大子数组和 (Maximum Subarray)

## 题目描述 [Original question](https://leetcode.cn/problems/maximum-subarray/?envType=study-plan-v2&envId=top-100-liked)

给你一个整数数组 `nums` ，请你找出一个具有最大和的连续子数组（子数组最少包含一个元素），返回其最大和。

> **子数组** 是数组中的一个连续部分。

**示例 1：**
* **输入：** `nums = [-2,1,-3,4,-1,2,1,-5,4]`
* **输出：** `6`
* **解释：** 连续子数组 `[4,-1,2,1]` 的和最大，为 `6`。

---

## 题目分析

这道题是计算机科学中非常经典的算法入门题。最直观的想法可能是暴力枚举所有子数组的起点和终点，计算它们的和并求最大值，但这种做法的时间复杂度高达 $O(N^2)$ 甚至 $O(N^3)$，在数组较大时会导致超时（Time Limit Exceeded）。

要优化时间复杂度，核心思想在于观察子数组的连续性特性：当我们遍历到第 $i$ 个元素时，我们需要做一个决策——是将这个元素加入到前面的连续子数组中，还是让它自立门户作为新子数组的起点？这个决策取决于前面的子数组对当前元素是起到了“增益”还是“拖累”的作用。

---

## 我的解法：贪心思想 / 优化后的动态规划

### 代码实现

```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        if(nums.empty())
            return 0;
        int curSum=0,maxSum=nums[0];
        for(int i=0;i<nums.size();i++)
        {
            if(curSum<0)
                curSum=nums[i];
            else
                curSum+=nums[i];
            maxSum=max(maxSum,curSum);
        }
        return maxSum;
    }
};
````

### 解法分析

这段代码实际上是非常经典的 **Kadane 算法**。它不仅代码简洁，而且效率极高，是解决此类问题的最优解之一。

  * **逻辑解析：** 维护一个 `curSum` 表示当前连续子数组的和。如果 `curSum < 0`，说明前面的子数组和只会越加越小，起到了副作用，此时果断放弃前面的累加结果，将 `curSum` 重置为当前元素 `nums[i]`；反之，则将当前元素累加到 `curSum` 中。在每次迭代时，都用 `curSum` 去更新全局最大值 `maxSum`。
  * **时间复杂度：** $O(N)$。只需要对数组进行一次线性遍历。
  * **空间复杂度：** $O(1)$。只使用了常数级别的额外变量，没有开辟新的空间。

-----

## 其他可能与更优解法

为了拓展算法思维，在学习《算法导论》或核心数据结构课程时，了解以下两种解法会对建立算法体系非常有帮助。

### 解法二：标准动态规划 (Dynamic Programming)

虽然前面的解法已经是动态规划的空间优化版本，但写出标准的 DP 状态转移方程有助于理解状态递推的本质。

#### 代码实现

```cpp
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int n = nums.size();
        if (n == 0) return 0;
        
        // dp[i] 表示以 nums[i] 结尾的连续子数组的最大和
        vector<int> dp(n, 0);
        dp[0] = nums[0];
        int maxSum = dp[0];
        
        for (int i = 1; i < n; i++) {
            dp[i] = max(dp[i - 1] + nums[i], nums[i]);
            maxSum = max(maxSum, dp[i]);
        }
        
        return maxSum;
    }
};
```

#### 解法分析

  * **逻辑解析：** 定义一个数组 `dp`，其中 $dp[i]$ 表示以 $i$ 结尾的最大子数组和。状态转移方程为：$dp[i]=\max(dp[i-1]+nums[i],nums[i])$。
  * **时间复杂度：** $O(N)$。遍历一次数组。
  * **空间复杂度：** $O(N)$。使用了一个长度为 $N$ 的一维数组。在实际工程中，由于 $dp[i]$ 只依赖于 $dp[i-1]$，因此可以用常数级别的变量代替（即退化为前文的最优解法），但在分析状态转移时，保留数组形式更直观。

### 解法三：分治法 (Divide and Conquer)

分治法在处理这道单次查询的题目时在常数时间上没有优势，但它是高级数据结构（如线段树）的理论基础。

#### 代码实现

```cpp
class Solution {
public:
    struct Status {
        int lSum; // 以左区间为端点的最大子段和
        int rSum; // 以右区间为端点的最大子段和
        int mSum; // 区间内的最大子段和
        int iSum; // 区间所有元素的总和
    };

    Status pushUp(Status l, Status r) {
        int iSum = l.iSum + r.iSum;
        int lSum = max(l.lSum, l.iSum + r.lSum);
        int rSum = max(r.rSum, r.iSum + l.rSum);
        int mSum = max(max(l.mSum, r.mSum), l.rSum + r.lSum);
        return (Status) {lSum, rSum, mSum, iSum};
    }

    Status get(vector<int> &a, int l, int r) {
        if (l == r) {
            return (Status) {a[l], a[l], a[l], a[l]};
        }
        int m = (l + r) >> 1;
        Status lSub = get(a, l, m);
        Status rSub = get(a, m + 1, r);
        return pushUp(lSub, rSub);
    }

    int maxSubArray(vector<int>& nums) {
        return get(nums, 0, nums.size() - 1).mSum;
    }
};
```

#### 解法分析

  * **逻辑解析：** 将数组从中间切分开，最大子数组要么完全在左半边，要么完全在右半边，要么跨越了中间节点。为了能够合并左右两边的结果，我们需要维护四个状态来解决跨越中点的情况：`lSum`, `rSum`, `mSum`, `iSum`。
  * **时间复杂度：** $O(N)$。分治树的高度为 $\log N$，但因为这里每次合并代价是 $O(1)$，总时间复杂度为 $O(N)$。这里展示的是借鉴了线段树思想的最优合并方式。
  * **空间复杂度：** $O(\log N)$。主要是递归调用栈的深度开销。

-----

## 总结

1.  **实用性与最优解：** 你提供的 **Kadane 算法** 是在机试和日常刷题中最推荐的写法。它以极低的空间开销和线性的时间复杂度优雅地解决了问题。
2.  **思维基础：** 标准的 **动态规划** 能够帮助建立严格的状态转移思维，这是后续解决更复杂的变种问题（如最大子矩阵和、打家劫舍系列）的基石。
3.  **架构视野：** **分治法** 虽然代码偏长，但它引入了区间合并的概念。在面对频繁修改数组元素并要求查询最大子数组和的进阶场景时，这种分治逻辑可以直接转化为线段树操作，具有极强的可扩展性。