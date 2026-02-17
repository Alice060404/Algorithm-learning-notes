# LeetCode Hot 100: 盛最多水的容器 (Container With Most Water)

## 一、 题目描述 [Original question](https://leetcode.cn/problems/container-with-most-water/?envType=study-plan-v2&envId=top-100-liked)

给定一个长度为  的整数数组 `height`。有 $n$ 条垂线，第 $i$ 条线的两个端点是 $(i, 0)$ 和 $(i, height[i])$ 。

找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。
返回容器可以储存的最大水量。

**说明：** 你不能倾斜容器。

## 二、 题目分析

这是一个经典的“木桶效应”问题。容器的容量由两条线的距离（宽度）和较短的那条线（高度）决定。

设左指针为 `left`，右指针为 `right`，则容器的容量公式为：

$$S(left, right) = \min(height[left], height[right]) \times (right - left)$$


如果使用双指针法，初始状态下，我们让 `left` 指向最左端，`right` 指向最右端。此时容器的宽度最大。
接下来我们需要向内移动指针来寻找更大的面积：

1. 如果我们移动**较高**的那条线的指针，容器的宽度必然减小，而高度受限于较低的那条线，所以高度不可能增加。因此，总面积一定会**变小**。
2. 如果我们移动**较低**的那条线的指针，容器的宽度必然减小，但新的线可能会更高，从而可能弥补宽度减小带来的损失，使得总面积**变大**。

**核心贪心策略：** 每次移动高度较小的那个指针。这本质上是一种“消减状态空间”的做法，通过每次排除掉必定不会是最大面积的状态，最终在线性时间内找到最优解。

## 三、 我的解法

我的解法使用了双指针从两端向中间遍历，每次移动较短的板。

```cpp
class Solution {
public:
    int minHeight(int a,int b)
    {
        return (a>b?b:a);
    }
    int maxArea(vector<int>& height) {
        int left=0,right=height.size()-1;
        int res=(right-left)*minHeight(height[left],height[right]);
        while(left<right)
        {
            if(height[left]<height[right])
            {
                left++;
                res=max(res,(right-left)*minHeight(height[left],height[right]));
            }
            else
            {
                right--;
                res=max(res,(right-left)*minHeight(height[left],height[right]));
            }
        }
        return res;
    }
};

```

**复杂度分析：**

* **时间复杂度：**$O(N)$ ，其中 $N$ 是数组的长度。左右指针最多相遇一次，遍历整个数组。
* **空间复杂度：**$O(1)$ ，只使用了常数个变量。

## 四、 其他可能的或更优的解法

上述算法逻辑已经是理论上的最优解（$O(N)$ 时间复杂度）。不过，在代码实现和工程规范上，有几点可以优化，使得代码更加简洁、易读，这也是在写代码时需要培养的习惯。

### 1. 代码精简优化版 (推荐)

* **避免重复造轮子：** C++标准库 `<algorithm>` 中已经提供了 `std::min` 和 `std::max` 函数，不需要自己额外写一个 `minHeight` 函数。
* **提取公共逻辑：** 在你的代码中，`if` 和 `else` 分支里计算 `res` 的逻辑是重复的。我们可以每次循环先计算当前面积，然后再移动指针，这样代码会精简很多。

```cpp
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    int maxArea(vector<int>& height) {
        int left = 0, right = height.size() - 1;
        int res = 0; // 初始面积设为0
        
        while (left < right) {
            // 先计算当前双指针构成的面积，并更新最大值
            int current_area = min(height[left], height[right]) * (right - left);
            res = max(res, current_area);
            
            // 贪心策略：移动较短的那根柱子的指针
            if (height[left] < height[right]) {
                left++;
            } else {
                right--;
            }
        }
        
        return res;
    }
};

```

### 2. 暴力枚举法 (了解即可)

为了构建完整的知识体系，我们需要知道这道题最朴素的解法。直接使用两层 `for` 循环，枚举所有可能的左右边界组合，计算面积并维护最大值。

```cpp
class Solution {
public:
    int maxArea(vector<int>& height) {
        int n = height.size();
        int res = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                int current_area = min(height[i], height[j]) * (j - i);
                res = max(res, current_area);
            }
        }
        return res;
    }
};

```

* **时间复杂度：**$O(N^2)$ 。这种解法在 LeetCode 上提交会得到 Time Limit Exceeded (超时) 的结果。
* **空间复杂度：**$O(1)$ 。

## 五、 总结

* **算法思想：** 双指针、贪心算法。
* **核心难点：** 理解**为什么移动较短的板子是正确的**。这也是数据结构与算法学习中很重要的一环：不仅要知道怎么写，还要能在逻辑上证明其正确性（即排除法，移动短板意味着排除了以当前短板为边界的所有内部组合，因为这些组合的面积绝对不可能超过当前面积）。
* **编码细节：** 善用 C++ 标准库中的 `min` 和 `max` 可以让代码更加紧凑。在处理分支逻辑时，注意提取出不受分支影响的公共计算步骤。