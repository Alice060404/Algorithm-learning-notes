# LeetCode 3. 无重复字符的最长子串

## 1. 题目描述 [Original question](https://leetcode.cn/problems/longest-substring-without-repeating-characters/description/?envType=study-plan-v2&envId=top-100-liked)

给定一个字符串 `s` ，请你找出其中不含有重复字符的**最长子串**的长度。

**示例 1:**

> 输入: s = "abcabcbb"
> 输出: 3
> 解释: 因为无重复字符的最长子串是 "abc"，所以其长度为 3。

**示例 2:**

> 输入: s = "bbbbb"
> 输出: 1
> 解释: 因为无重复字符的最长子串是 "b"，所以其长度为 1。

**示例 3:**

> 输入: s = "pwwkew"
> 输出: 3
> 解释: 因为无重复字符的最长子串是 "wke"，所以其长度为 3。请注意，你的答案必须是子串的长度，"pwke" 是一个子序列，不是子串。

---

## 2. 题目分析

这道题考察的是基础的数据结构与算法思想，对于大一阶段夯实算法基础非常重要。

寻找“连续”且“满足特定条件（无重复）”的元素集合，最经典的解法就是**滑动窗口（Sliding Window）**。你可以把滑动窗口想象成一个大小可变的框，这个框在字符串上从左向右滑动。

具体机制如下：

1. 我们维护一个窗口 `[left, right]`，包含当前正在考察的子串。
2. 窗口的右边界 `right` 不断向右移动，把新的字符加入窗口。
3. 每次加入新字符时，检查窗口内是否出现了重复字符。
4. 如果出现重复，说明当前窗口已经不再满足条件，我们需要将左边界 `left` 向右移动，将字符移出窗口，直到窗口内不再有重复字符为止。
5. 在这个过程中，不断记录和更新窗口的最大长度。

---

## 3. 我的解法（基于频率统计的滑动窗口）

这是你提供的解法，逻辑非常清晰，使用了 `unordered_map` 来记录窗口内每个字符出现的次数。

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        unordered_map<char,int> charCount;
        int res = 0;
        int n = s.size();
        for(int left = 0, right = 0; right < n; right++) {
            charCount[s[right]]++;
            // 当新加入的字符导致出现重复时，不断移动左边界
            while(charCount[s[right]] > 1) {
                charCount[s[left]]--;
                left++;
            }
            // 更新最大子串长度
            res = max(res, right - left + 1);
        }
        return res;
    }
};

```

---

## 4. 更优解法

虽然上述解法已经能够正确解决问题，但在 C++ 实际运行中，还有进一步优化的空间。

### 优化解法一：基于索引跳转的滑动窗口

在我的解法中，当遇到重复字符时，`left` 指针是一步一步向右移动的。实际上，如果我们在 Map 中记录的是**字符最后一次出现的索引**，当遇到重复字符时，`left` 可以直接跳到该字符上次出现位置的下一个位置，从而省去中间的循环步数。

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        // map 记录字符以及它最后一次出现的索引
        unordered_map<char, int> charIndex;
        int res = 0;
        int n = s.size();
        
        for (int left = 0, right = 0; right < n; right++) {
            char currentChar = s[right];
            // 如果字符在 map 中出现过，并且其索引在当前窗口的范围内
            if (charIndex.count(currentChar) && charIndex[currentChar] >= left) {
                // 直接将 left 跳转到重复字符的下一个位置
                left = charIndex[currentChar] + 1;
            }
            // 更新字符的最新索引
            charIndex[currentChar] = right;
            // 更新最大长度
            res = max(res, right - left + 1);
        }
        return res;
    }
};

```

### 优化解法二：利用定长数组替代哈希表（极致优化）

在 C++ 中，`std::unordered_map` 虽然单次操作的时间复杂度是 $O(1)$ ，但由于哈希冲突处理和动态内存分配，其常数开销是比较大的。考虑到题目中的字符通常为 ASCII 字符（共 128 个），我们可以直接使用一个大小为 128 的整型数组来代替哈希表。这在底层是直接的内存地址偏移计算，速度会快非常多，是 C++ 算法刷题中极其常用的提速技巧。

```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        // 使用数组模拟哈希表，初始化为 -1，表示字符尚未出现
        vector<int> charIndex(128, -1);
        int res = 0;
        int n = s.size();
        
        for(int left = 0, right = 0; right < n; right++) {
            char currentChar = s[right];
            // 如果该字符之前出现过，并且位置在当前窗口内
            if(charIndex[currentChar] >= left) {
                left = charIndex[currentChar] + 1;
            }
            charIndex[currentChar] = right;
            res = max(res, right - left + 1);
        }
        return res;
    }
};

```

---

## 5. 解法分析

| 解法 | 时间复杂度 | 空间复杂度 | 优缺点分析 |
| --- | --- | --- | --- |
| **我的解法 (频率统计)** | $O(N)$ | $O(\Sigma)$ | **优点**：逻辑直观，符合第一直觉，代码可读性好。<br>**缺点**：最坏情况下（如全是相同字符），`left` 和 `right` 会分别遍历字符串一次，实际执行步数为 。 |
| **优化解法一 (索引跳转)** | $O(N)$ | $O(\Sigma)$ | **优点**：`left` 指针直接跳转，避免了内层 `while` 循环的一步步递进，无论何种情况都只遍历  步。<br>**缺点**：`unordered_map` 的常数开销依然存在。 |
| **优化解法二 (数组模拟)** | $O(N)$ | $O(\Sigma)$ | **优点**：执行速度最快。利用 C++ 数组在内存中连续的特性，寻址极快，是力扣提交中能打败 100% 用户的标准解法。<br>**缺点**：适用范围受限，仅适用于字符集较小的情况（如 ASCII）。如果是极大规模的 Unicode 字符集，依然需要使用哈希表。 |

*注： 为字符串长度， 为字符集的大小（本题中通常为 128 或 256）。*
