# LeetCode 1. 两数之和 (Two Sum)

## 题目描述 [Original question](https://leetcode.cn/problems/two-sum/description/?envType=study-plan-v2&envId=top-100-liked)

给定一个整数数组 `nums` 和一个整数目标值 `target`，请你在该数组中找出 **和为目标值** `target` 的那 **两个** 整数，并返回它们的数组下标。

你可以假设每种输入只会对应一个答案。但是，数组中同一个元素在答案里不能重复出现。
你可以按任意顺序返回答案。

**示例 1：**
> 输入：nums = [2,7,11,15], target = 9
> 输出：[0,1]
> 解释：因为 nums[0] + nums[1] == 9 ，返回 [0, 1] 。

**示例 2：**
> 输入：nums = [3,2,4], target = 6
> 输出：[1,2]

---

## 分析思路

在寻找两个数相加等于目标值时，最直观的数学逻辑是：
$$x + y = \text{target} \implies y = \text{target} - x$$

当我们遍历数组拿到一个数字 $x$ 时，问题就转化成了：**在数组中查找是否存在一个数字 $y$**。
查找操作的效率决定了整个算法的性能。如果每次都重新遍历数组去寻找 $y$，时间开销会非常大。为了优化查找速度，我们需要引入能在 $O(1)$ 时间复杂度内完成查找的数据结构——**哈希表 (Hash Table)**。



在 C++ 中，`std::unordered_map` 的底层就是基于哈希表实现的。通过将数组的值作为 Key，索引作为 Value 存入哈希表，我们可以极大地提升程序的运行效率。这也就是算法设计中极其重要的**“空间换时间”**思想。

---

## 我的解法：两遍哈希表

第一遍遍历将所有数据录入“字典”（哈希表），第二遍遍历通过查字典来寻找配对的数字。逻辑严密，并且处理了元素不能自己和自己配对的边界情况（`j != map[target-num]`）。

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) 
    {
        unordered_map<int,int> map;
        // 第一遍：将所有元素及其索引存入哈希表
        for(int i = 0; i < nums.size(); i++)
        {
            map[nums[i]] = i;
        }
        
        // 第二遍：查找 target - num 是否在哈希表中
        for(int j = 0; j < nums.size(); j++)
        {
            int num = nums[j];
            // 确保找到的元素不是当前元素本身
            if(map.find(target - num) != map.end() && j != map[target - num])
                return {j, map[target - num]};
        }
        return {};
    }
};

```

* **时间复杂度：*$O(N)$* ，其中 $N$ 是数组中的元素数量。由于 `unordered_map` 的每次查找和插入的时间复杂度平均为 $O(1)$ ，两次循环各遍历一次数组，总时间复杂度为线性的 $O(N)$ 。
* **空间复杂度：*$O(N)$* ，哈希表最多需要存储 $N$ 个键值对。

---

## 更优解法：一遍哈希表

还可以进一步优化为**一次遍历**。
在将元素存入哈希表的过程中，我们可以**边存边查**。当我们遍历到元素 $x$ 时，直接去哈希表里查询是否已经存在 $\text{target} - x$ 。

* 如果存在，说明找到了答案，直接返回。
* 如果不存在，将当前的 $x$ 和它的索引存入哈希表，供后续的数字查找使用。

这种写法不仅代码更简洁，并且在遇到类似 `nums = [3, 3], target = 6` 这种有重复元素的测试用例时，也能自然避开自己匹配自己的问题（因为查询发生时，当前元素还没被加入哈希表）。

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> map;
        for (int i = 0; i < nums.size(); i++) {
            int complement = target - nums[i];
            
            // 如果在哈希表中找到了互补的数字，直接返回它们的索引
            if (map.find(complement) != map.end()) {
                return {map[complement], i};
            }
            
            // 如果没找到，就把当前数字和它的索引存入哈希表
            map[nums[i]] = i;
        }
        return {};
    }
};

```

* **时间复杂度：*$O(N)$* ，只遍历了一次数组。
* **空间复杂度：*$O(N)$* 。

---

## 基础解法：暴力枚举 (作为对比)

作为算法基础对照，这里给出不使用额外数据结构的暴力解法。通过两层嵌套循环，枚举所有的数字对并检查它们的和是否等于目标值。

```cpp
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (nums[i] + nums[j] == target) {
                    return {i, j};
                }
            }
        }
        return {};
    }
};

```

* **时间复杂度：*$O(N^2)$* ，嵌套循环导致了平方级别的时间开销。当数组规模变大时，运行时间会急剧增加（在 LeetCode 中可能会导致超时）。
* **空间复杂度：*$O(1)$* ，除了用于循环的控制变量，没有使用额外的存储空间。

---

## 总结

| 解法 | 时间复杂度 | 空间复杂度 | 核心思想 | 适用场景 |
| --- | --- | --- | --- | --- |
| **暴力枚举** | $O(N^2)$ | $O(1)$ | 穷举所有可能性 | 内存极度受限，且数据量极小 |
| **两遍哈希表** (我的解法) | $O(N)$ | $O(N)$ | 空间换时间，分离构建和查找逻辑 | 逻辑清晰，适合梳理思路 |
| **一遍哈希表** (最优) | $O(N)$ | $O(N)$ | 空间换时间，边遍历边查验 | 日常刷题与面试首选 |

**学习心得：**
这道题是理解 C++ STL 中 `std::unordered_map` 机制的绝佳切入点。在未来的算法设计中，如果遇到需要频繁执行“查找”的操作，第一时间考虑引入哈希表来降低时间复杂度是一个非常好的习惯。

