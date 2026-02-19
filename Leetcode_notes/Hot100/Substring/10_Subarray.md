# LeetCode 560. 和为 K 的子数组 (Subarray Sum Equals K)

## 1. 题目描述

给你一个整数数组 `nums` 和一个整数 `k` ，请你统计并返回 *该数组中和为 `k` 的子数组的个数* 。

子数组是数组中元素的连续非空序列。

**示例 1：**

```text
输入：nums = [1,1,1], k = 2
输出：2

```

**示例 2：**

```text
输入：nums = [1,2,3], k = 3
输出：2

```

**提示：**

* `1 <= nums.length <= 2 * 10^4`
* `-1000 <= nums[i] <= 1000`
* `-10^7 <= k <= 10^7`

---

## 2. 题目分析

这道题的核心在于如何快速求解子数组的和。

### 核心概念：前缀和 (Prefix Sum)

对于任意子数组 `[i..j]` (即下标从 `i` 到 `j`) 的和，我们可以利用前缀和数组 `preSum` 快速求出：
$$Sum[i..j] = preSum[j] - preSum[i-1]$$


其中 `preSum[x]` 表示 `nums[0]...nums[x]` 的和。

### 问题转化

题目要求找到和为 `k` 的子数组，即寻找满足以下条件的下标对 `(i, j)`：
$$preSum[j] - preSum[i-1] = k$$

移项可得：
$$preSum[i-1] = preSum[j] - k$$

### 优化思路

如果我们遍历数组计算当前的前缀和 `preSum[j]`，我们只需要知道在当前位置之前，有多少个前缀和的值等于 `preSum[j] - k`。
这通过一个哈希表 (Hash Map) 就可以在 $O(1)$ 时间内查到。这与经典的 "Two Sum" 问题思路非常相似。

---

## 3. 我的解法 (前缀和 + 哈希表优化)

该解法利用哈希表存储 `前缀和 -> 该前缀和出现的次数`，从而将时间复杂度降低到线性。

```cpp
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        // key: 前缀和, value: 该前缀和出现的次数
        unordered_map<int,int> hashMap;
        // 初始化：前缀和为0出现1次（处理从下标0开始的子数组情况）
        hashMap[0]=1;
        
        int res=0;
        int preSum=0;
        
        for(int val:nums)
        {
            preSum+=val;
            // 查找是否存在 preSum[i-1] = preSum[j] - k
            // 注意：contains 是 C++20 新特性，若编译器版本较低可用 find() != end()
            if(hashMap.contains(preSum-k))
                res+=hashMap[preSum-k];
            
            // 更新当前前缀和的计数
            hashMap[preSum]++;
        }
        return res;
    }
};

```

---

## 4. 其他可能的解法

虽然上述解法已是时间复杂度最优，但在刷题笔记中记录暴力解法有助于理解优化的必要性。

### 解法二：暴力枚举 (枚举所有子数组)

这是最直观的解法，通过两层循环枚举所有可能的子数组起点和终点，计算其和。

```cpp
class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        int count = 0;
        int n = nums.size();
        
        // 枚举起点
        for (int start = 0; start < n; ++start) {
            int sum = 0;
            // 枚举终点
            for (int end = start; end < n; ++end) {
                sum += nums[end];
                if (sum == k) {
                    count++;
                }
            }
        }
        return count;
    }
};

```

---

## 5. 解法分析与对比

| 解法 | 时间复杂度 | 空间复杂度 | 分析 |
| --- | --- | --- | --- |
| **我的解法 (前缀和+哈希)** | $O(N)$ | $O(N)$ | **最优解**。只需要遍历一次数组。空间主要用于哈希表存储前缀和，最坏情况下所有前缀和都不同，需要 $O(N)$ 空间。 |
| **解法二 (暴力枚举)** | $O(N^2)$ | $O(1)$ | 这种解法不需要额外的空间（除了几个变量），但是在数组长度达到 $2 \times 10^4$ 时，运算次数约为 $2 \times 10^8$ 级别，大概率会**超时 (Time Limit Exceeded)**。 |

### 关键点解析

1. **为什么 `hashMap[0]=1`？**
* 这是为了处理那些从数组下标 `0` 开始且和为 `k` 的子数组。
* 例如 `nums = [2], k = 2`。`preSum` 变为 2，我们需要找 `preSum - k = 0`。如果不预置 `0` 出现 1 次，就会漏掉这种情况。


2. **C++ 版本注意**
* `hashMap.contains()` 是 C++20 引入的语法，非常简洁。如果是 C++17 或更早的标准（例如部分旧的笔试环境），需要写成 `if (hashMap.find(preSum - k) != hashMap.end())`。



---

## 6. 总结

* **前缀和技巧**：遇到“子数组和”相关的问题，首先考虑前缀和。
* **哈希表优化**：当我们需要在一个数组中快速查找“两个数之差等于定值”或“两个数之和等于定值”时，哈希表可以将查找时间从 $O(N)$ 优化到 $O(1)$ 。
* **本题本质**：其实是“两数之和 (Two Sum)”问题的变体。Two Sum 是找 `a + b = target`，本题是找 `preSum[j] - preSum[i] = k`。