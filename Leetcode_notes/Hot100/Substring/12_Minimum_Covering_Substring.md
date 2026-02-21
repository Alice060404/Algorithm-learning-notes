# LeetCode Hot 100: 76. 最小覆盖子串

## 1\. 题目描述 [Original question](https://leetcode.cn/problems/minimum-window-substring/?envType=study-plan-v2&envId=top-100-liked)

给你一个字符串 `s` 、一个字符串 `t` 。返回 `s` 中涵盖 `t` 所有字符的最小子串。如果 `s` 中不存在涵盖 `t` 所有字符的子串，则返回空字符串 `""` 。

**注意：**

- 对于 `t` 中重复字符，我们寻找的子字符串中该字符数量必须不少于 `t` 中该字符数量。
- 如果 `s` 中存在这样的子串，我们保证它是唯一的答案。

## 2\. 题目分析

这道题是典型的“滑动窗口”问题，非常适合用来建立对双指针算法和动态区间维护的直觉。

核心思想是维护一个窗口（由左右两个指针界定），通过移动左右边界来寻找满足条件的解。具体步骤可以归纳为两步：

1.  **扩张窗口（寻找可行解）**：右指针 `right` 不断向右移动，把新遍历到的字符加入窗口，直到窗口包含了 `t` 中的所有字符。
2.  **收缩窗口（优化可行解）**：一旦窗口满足了条件，就尝试把左指针 `left` 向右移动。这一步是为了在保证满足条件的前提下，尽可能缩小窗口的长度，并更新记录的最小子串长度。当窗口不再满足条件时，再次回到第一步继续扩张。

简单概括就是：“右指针主动出击寻找解，左指针步步紧逼优化解”。

## 3\. 我的解法 (滑动窗口 + 数组优化)

```cpp
class Solution {
public:
    string minWindow(string s, string t) {
        int need[128]={0};
        int window[128]={0};
        int require=0,valid=0;
        for(const auto&c:t)
            need[c]++;
        for(const auto&count:need)
        {
            if(count>0)
                require++;
        }
        int left=0,right=0;
        int start=0,minLen=INT_MAX;
        while(right<s.size())
        {
            char c=s[right];
            right++;
            if(need[c]>0)
            {
                window[c]++;
                if(window[c]==need[c])
                    valid++;
            }
            while(valid==require)
            {
                if(right-left<minLen)
                {
                    start=left;
                    minLen=right-left;
                }
                char d=s[left];
                left++;
                if(need[d]>0)
                {
                    if(window[d]==need[d])
                        valid--;
                    window[d]--;
                }
            }
        }
        return minLen==INT_MAX?"":s.substr(start,minLen);
    }
};
```

## 4\. 其他/更优解法分析

客观地说，上述代码已经是该问题的**最优解法**。

在数据结构的选择上，巧妙地利用了长度为 128 的定长数组 `need` 和 `window` 来代替标准库中的哈希表。因为题目涉及的字符一般在 ASCII 码范围内，使用数组可以保证内存在物理上是连续的，省去了哈希函数的计算和哈希表内部的冲突处理开销。这极大降低了常数时间，是非常扎实且高效的底层优化。

为了拓宽思路，这里提供一种在特定极端场景下的**变体优化解法：过滤字符串法**。

### 重点分析：过滤字符串优化

**场景**：如果字符串 `s` 非常长，且其中大部分字符都不在 `t` 中（也就是无效字符极多）。
**思路**：我们可以先对 `s` 进行一次预处理，将 `s` 中所有出现在 `t` 里的有效字符及其对应的索引提取出来，存入一个新的列表中。之后的滑动窗口操作只在这个极度压缩的列表上进行，从而跳过大量无用的字符遍历。

```cpp
#include <string>
#include <vector>
#include <climits>

class Solution {
public:
    std::string minWindow(std::string s, std::string t) {
        int need[128] = {0};
        int require = 0;
        for (char c : t) {
            if (need[c] == 0) require++;
            need[c]++;
        }

        // 预处理：只记录 s 中在 t 里出现过的有用字符及其索引
        std::vector<std::pair<int, char>> filtered_s;
        for (int i = 0; i < s.size(); i++) {
            if (need[s[i]] > 0) {
                filtered_s.push_back({i, s[i]});
            }
        }

        int left = 0, right = 0, valid = 0;
        int window[128] = {0};
        int start = 0, minLen = INT_MAX;

        // 在压缩后的列表上执行滑动窗口
        while (right < filtered_s.size()) {
            char c = filtered_s[right].second;
            int end_idx = filtered_s[right].first;
            right++;

            window[c]++;
            if (window[c] == need[c]) {
                valid++;
            }

            while (valid == require) {
                int start_idx = filtered_s[left].first;
                // 注意这里计算长度需要用原字符串的索引
                if (end_idx - start_idx + 1 < minLen) {
                    start = start_idx;
                    minLen = end_idx - start_idx + 1;
                }

                char d = filtered_s[left].second;
                left++;
                if (window[d] == need[d]) {
                    valid--;
                }
                window[d]--;
            }
        }

        return minLen == INT_MAX ? "" : s.substr(start, minLen);
    }
};
```

## 5\. 解法对比分析

| 解法名称                       | 时间复杂度 | 空间复杂度 | 适用场景分析                                                                                                                                  |
| :----------------------------- | :--------- | :--------- | :-------------------------------------------------------------------------------------------------------------------------------------------- |
| **我的解法 (定长数组优化)**    | $O(M + N)$ | $O(1)$     | **全局最优/首选方案**。由于字符集仅限 ASCII，常数空间开销极小。代码逻辑清晰，执行速度最快，是算法机试和面试的标准答案。                       |
| **通用哈希表 (unordered_map)** | $O(M + N)$ | $O(K)$     | 若字符集巨大（如包含全部 Unicode 字符）无法使用定长数组时必须采用的方法。实际运行速度因为哈希开销会慢于数组。                                 |
| **过滤字符串优化法**           | $O(M + N)$ | $O(M)$     | 属于针对性优化。仅在 `s` 极长且 `t` 的字符在 `s` 中出现频率极低的特殊测试用例下，能减少内层循环的执行次数。牺牲了部分空间换取特定场景的时间。 |

_注：$M$ 和 $N$ 分别为字符串 `s` 和 `t` 的长度。$K$ 为字符集大小。_

## 6\. 总结

最小覆盖子串是滑动窗口的进阶模板题。掌握这个模板后，你可以将其无缝推广到其他子串或子数组问题中。

前者代码展示了非常清晰的逻辑分层：

1.  巧妙使用了 `valid` 变量来记录当前窗口内满足 `need` 条件的字符**种类数**，这样在判断窗口是否合规时，只需比较 `valid == require`，避免了每次都要去遍历长度为 128 的数组，时间效率极高。
2.  边界条件处理准确，`right` 和 `left` 的移动与状态更新没有发生越界和错位。

继续保持这种严谨的编码习惯，这种对底层数据结构开销敏感的思维，对日后的计算机专业学习有极大帮助。
