# LeetCode 438. 找到字符串中所有字母异位词

## 1. 题目描述 [Original question](https://leetcode.cn/problems/find-all-anagrams-in-a-string/?envType=study-plan-v2&envId=top-100-liked)

给定两个字符串 `s` 和 `p`，找到 `s` 中所有 `p` 的 **异位词** 的子串，返回这些子串的起始索引。不考虑答案输出的顺序。

**异位词** 指由相同字母重排列形成的字符串（包括相同的字符串）。

**示例 1:**

> 输入: s = "cbaebabacd", p = "abc"
> 输出: [0,6]
> 解释:
> 起始索引等于 0 的子串是 "cba", 它是 "abc" 的异位词。
> 起始索引等于 6 的子串是 "bac", 它是 "abc" 的异位词。

**示例 2:**

> 输入: s = "abab", p = "ab"
> 输出: [0,1,2]
> 解释:
> 起始索引等于 0 的子串是 "ab", 它是 "ab" 的异位词。
> 起始索引等于 1 的子串是 "ba", 它是 "ab" 的异位词。
> 起始索引等于 2 的子串是 "ab", 它是 "ab" 的异位词。

---

## 2. 题目分析

这道题是《数据结构与算法》中非常标准的**定长滑动窗口**案例。对于刚接触算法体系的初学者来说，对比之前的“无重复字符的最长子串”（变长滑动窗口），这道题的窗口大小是严格固定为字符串 `p` 的长度的。

核心逻辑如下：

1. **异位词的等价条件**：两个字符串互为异位词，等价于它们包含的“字符种类”以及“每种字符的数量”完全相同。
2. **窗口滑动**：在字符串 `s` 上维护一个长度为 `p.size()` 的窗口。窗口每次向右滑动一格，本质上就是“移出一个左边缘字符，加入一个右边缘字符”。
3. **状态校验**：每次滑动后，只需校验当前窗口内的字符频率统计是否与字符串 `p` 的统计完全一致即可。

---

## 3. 我的解法（定长滑动窗口 + 频率数组比对）

利用了 C++ 的 `vector<int>` 来存储 26 个小写字母的频率，并通过直接重载的 `==` 运算符比较两个 `vector` 是否相等。

```cpp
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int> res;
        vector<int> s_count(26);
        vector<int> p_count(26);
        
        // 统计字符串 p 的字符频率
        for(int i = 0; i < p.size(); i++)
            p_count[p[i] - 'a']++;
            
        // 滑动窗口遍历字符串 s
        for(int left = 0, right = 0; right < s.size(); right++) {   
            s_count[s[right] - 'a']++;
            
            // 维护定长窗口：如果窗口长度超过了 p 的长度，左边界右移
            if(right - left + 1 > p.size()) {
                s_count[s[left] - 'a']--;
                left++;
            }
            
            // 每次比较两个频率数组是否完全相同
            if(s_count == p_count)
                res.push_back(left);
        }
        return res;
    }
};

```

---

## 4. 更优解法（滑动窗口 + 差异度变量优化）

我的解法在每次判断 `s_count == p_count` 时，底层其实执行了一个长度为 26 的数组遍历。虽然 26 是常数，但在极限情况下（如 `s` 极长），这会带来不必要的开销。

我们可以引入一个**差异度变量 `differ**`，用来记录当前窗口与 `p` 在字符频率上有差异的字母个数。这样，每次窗口滑动时，我们只需用 $O(1)$ 的时间更新 `differ`，当 `differ == 0` 时，说明找到了一个异位词。这是算法设计中通过“维护状态量”来降低时间复杂度常数的经典技巧。

```cpp
class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        int sLen = s.size(), pLen = p.size();
        if (sLen < pLen) return vector<int>();

        vector<int> ans;
        // count 数组记录当前窗口频率与 p 频率的差值
        vector<int> count(26, 0);
        
        // 初始化第一个窗口的差值数组
        for (int i = 0; i < pLen; ++i) {
            ++count[s[i] - 'a'];
            --count[p[i] - 'a'];
        }

        // 统计初始状态下，频率不同的字符种数
        int differ = 0;
        for (int j = 0; j < 26; ++j) {
            if (count[j] != 0) ++differ;
        }

        if (differ == 0) ans.push_back(0);

        // 窗口开始滑动，每次滑动一步（移除左边字符，加入右边新字符）
        for (int i = 0; i < sLen - pLen; ++i) {
            // 1. 处理移出窗口的左侧字符 s[i]
            if (count[s[i] - 'a'] == 1) {  
                --differ; // 移出后差值归零，差异度减一
            } else if (count[s[i] - 'a'] == 0) {  
                ++differ; // 移出前一致，移出后出现差异，差异度加一
            }
            --count[s[i] - 'a'];

            // 2. 处理进入窗口的右侧新字符 s[i + pLen]
            if (count[s[i + pLen] - 'a'] == -1) {  
                --differ; // 加入后差值归零，差异度减一
            } else if (count[s[i + pLen] - 'a'] == 0) {  
                ++differ; // 加入前一致，加入后出现差异，差异度加一
            }
            ++count[s[i + pLen] - 'a'];

            // 3. 判断当前窗口是否匹配
            if (differ == 0) {
                ans.push_back(i + 1);
            }
        }
        return ans;
    }
};

```

---

## 5. 解法分析

| 解法 | 时间复杂度 | 空间复杂度 | 优缺点分析 |
| --- | --- | --- | --- |
| **我的解法 (定长窗口+数组比对)** | $O(N \times \Sigma)$ | $O(\Sigma)$ | **优点**：逻辑直白，利用 C++ 的 `vector` 判等特性代码极其简洁。是理解滑动窗口原理的极佳代码范本。<br>**缺点**：内层存在隐形的 $O(\Sigma)$ 遍历，虽然 ，常数项在严苛评测下仍有压缩空间。 |
| **优化解法 (窗口+差异度维护)** | $O(N)$ | $O(\Sigma)$ | **优点**：执行效率极高。将内层的 26 次比较优化到了严格的常数次加减判断（$O(1)$），这是力扣上的极致执行效率写法。<br>**缺点**：逻辑嵌套变多，尤其是对 `differ` 增减边界条件的判断（加一还是减一），编写时容易出现细节失误。 |

*注：$N$ 为字符串 `s` 的长度，$\Sigma$ 为字符集的大小（本题为 26 个小写英文字母）。*