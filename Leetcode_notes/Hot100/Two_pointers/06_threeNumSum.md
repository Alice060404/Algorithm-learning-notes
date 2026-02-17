# LeetCode 15. 三数之和 (3Sum)

## 1. 题目描述 [Original question](https://leetcode.cn/problems/3sum/?envType=study-plan-v2&envId=top-100-liked)

给你一个整数数组 `nums` ，判断是否存在三元组 `[nums[i], nums[j], nums[k]]` 满足 `i != j`、`i != k` 且 `j != k` ，同时还满足 `nums[i] + nums[j] + nums[k] == 0` 。

请你返回所有和为 `0` 且不重复的三元组。

**注意：** 答案中不可以包含重复的三元组。

**示例 1：**

> **输入：** nums = [-1,0,1,2,-1,-4]
> **输出：** [[-1,-1,2],[-1,0,1]]
> **解释：**
> nums[0] + nums[1] + nums[2] = (-1) + 0 + 1 = 0
> nums[1] + nums[2] + nums[4] = 0 + 1 + (-1) = 0
> nums[0] + nums[3] + nums[4] = (-1) + 2 + (-1) = 0
> 不同的三元组是 [-1,-1,2] 和 [-1,0,1] 。

## 2. 题目分析

在面对这道经典题目时，我们首先要避免的是暴力求解。直接使用三层 `for` 循环遍历数组的时间复杂度是 $O(N^3)$ ，这在 LeetCode 中一定会导致超出时间限制 (Time Limit Exceeded)。

因此，我们需要对时间复杂度进行降维。这道题的核心难点在于**不重复**。如果使用哈希表来记录，去重操作会非常繁琐且容易出错。

最优的解题思路是：**排序 + 双指针**。

1. **排序：** 首先将数组从小到大进行排序。排序不仅为双指针的移动提供了单调性的基础，更重要的是，它将相同的元素聚集在了一起，极大地方便了后续的“去重”操作。
2. **固定一点，双指针向内收缩：** * 遍历数组，使用指针 `i` 指向当前选定作为三元组第一个数的元素。
* 在 `i` 之后的子数组中，设置两个指针：`left` 指向 `i + 1`，`right` 指向数组末尾。
* 计算三数之和 `sum = nums[i] + nums[left] + nums[right]`。
* 如果 `sum == 0`，记录答案，并且 `left` 和 `right` 同时向内移动，同时跳过所有重复的元素。
* 如果 `sum < 0`，说明整体偏小，需要增大，`left` 右移。
* 如果 `sum > 0`，说明整体偏大，需要减小，`right` 左移。



## 3. 我的解法（排序 + 双指针）

使用了“排序+双指针”框架。

```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        sort(nums.begin(),nums.end());
        vector<vector<int>> res;
        for(int i=0;i<nums.size()-2;i++)
        {
            if(i>0&&nums[i]==nums[i-1])
                continue;
            int left=i+1,right=nums.size()-1;
            while(left<right)
            {
                if(nums[left]+nums[right]==-nums[i])
                {
                    res.push_back({nums[i],nums[left],nums[right]});
                    left++;
                    right--;
                    while(left<right&&nums[left]==nums[left-1])
                    {
                        left++;
                    }
                    while(left<right&&nums[right]==nums[right+1])
                    {
                        right--;
                    }
                }
                else if(nums[left]+nums[right]>-nums[i])
                {
                    right--;
                }
                else
                {
                    left++;
                }
            }
        }
        return res;
    }
};

```

**复杂度分析：**

* **时间复杂度：**$O(N^2)$ 。其中 $N$ 是数组的长度。排序的时间复杂度为 $O(N \log N)$ ，外层遍历的时间复杂度是 $O(N)$ ，内部双指针遍历的时间复杂度是 $O(N)$ ，整体为 $O(N \log N + N^2) = O(N^2)$ 。
* **空间复杂度：**$O(\log N)$ 。主要为 `std::sort` 排序所需的额外空间。

## 4. 更优解法（细节优化与避坑）

你的解法在算法思路上已经是最优的了。但在 C++ 的工程实践和刷题细节中，还有几个大一新生容易踩坑的地方以及可以进一步剪枝的优化点：

1. **C++ 的 `size()` 无符号整型溢出陷阱：** 在你的代码 `for(int i=0;i<nums.size()-2;i++)` 中，`nums.size()` 返回的是 `size_t` (无符号整型)。如果传入的数组长度小于 2，`nums.size() - 2` 会发生下溢出，变成一个巨大的正数，从而进入循环导致数组越界或段错误 (Segmentation Fault)。**规范做法是先将其转为 `int`，或者提前进行长度判断。**
2. **极值剪枝：** 因为数组已经排好序，如果 `nums[i] > 0`，那么它后面的数肯定也都大于 0，三者相加绝对不可能等于 0，此时可以直接 `break` 结束整个循环。
3. **去重逻辑的直观性：** 找到答案后，对比当前元素和**下一个**即将遍历的元素是否相等来进行去重，逻辑上更不容易越界出错。

**优化后的 C++ 代码：**

```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        int n = nums.size();
        // 1. 边界条件判断，防止后续逻辑出错
        if (n < 3) return {};
        
        vector<vector<int>> res;
        sort(nums.begin(), nums.end());
        
        for (int i = 0; i < n - 2; i++) {
            // 2. 剪枝：如果排好序后的第一个数就大于0，后面的数相加不可能为0
            if (nums[i] > 0) break;
            
            // 对 i 去重
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            
            int left = i + 1;
            int right = n - 1;
            
            while (left < right) {
                // 使用 sum 变量统一计算，避免 -(nums[i]) 这种思维绕弯
                int sum = nums[i] + nums[left] + nums[right];
                
                if (sum == 0) {
                    res.push_back({nums[i], nums[left], nums[right]});
                    
                    // 3. 对 left 和 right 去重（注意这里与你的区别，先判断下一个再移动）
                    while (left < right && nums[left] == nums[left + 1]) left++;
                    while (left < right && nums[right] == nums[right - 1]) right--;
                    
                    // 找到答案并跳过重复项后，双指针向内收缩寻找下一组可能的解
                    left++;
                    right--;
                } else if (sum < 0) {
                    left++; // 和太小，左指针右移使和增大
                } else {
                    right--; // 和太大，右指针左移使和减小
                }
            }
        }
        return res;
    }
};

```

## 5. 总结

* **算法核心：** 三数之和是“双指针”技巧的经典应用。通过先固定一个元素，将 $O(N^3)$ 的三维问题降级为 $O(N^2)$ 的二维问题（即在一个有序数组中寻找两个数使其和为特定值）。
* **前置条件：** 排序是使用双指针的前提，它不仅保证了指针单向移动的正确性，也让去除重复解变得极其简单（相邻元素比较即可）。
* **代码素养：** 针对 C++ 语言特性，在处理涉及 `.size()` 的减法运算时，务必警惕无符号整型下溢出的致命错误；同时，合理利用数组的有序性进行 `nums[i] > 0` 的提前剪枝，能体现出对算法细节的极致把控能力。