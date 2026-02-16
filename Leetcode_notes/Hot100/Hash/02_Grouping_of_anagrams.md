# LeetCode 49. 字母异位词分组 (Group Anagrams)

## 题目描述 [Original question](https://leetcode.cn/problems/group-anagrams/description/?envType=study-plan-v2&envId=top-100-liked)

给你一个字符串数组 `strs`，请你将 **字母异位词** 组合在一起。可以按任意顺序返回结果列表。

**字母异位词** 是由重新排列源单词的所有字母得到的一个新单词（即包含的字母种类和数量完全相同，只是顺序不同）。

**示例 1：**
> 输入: strs = ["eat", "tea", "tan", "ate", "nat", "bat"]
> 输出: [["bat"],["nat","tan"],["ate","eat","tea"]]

**示例 2：**
> 输入: strs = [""]
> 输出: [[""]]

**示例 3：**
> 输入: strs = ["a"]
> 输出: [["a"]]

---

## 分析思路

面对这道题，核心任务是**“分组”**。在计算机科学中，处理分组问题的首选数据结构通常是**哈希表 (Hash Table)**。

我们需要在遍历字符串数组时，把属于同一组的单词放到一起。这就要求我们提取出一个**特征值**作为哈希表的键（Key），把单词组成的数组作为哈希表的值（Value）。
问题的关键转化为：**如何让 "eat"、"tea"、"ate" 提取出完全相同的特征值（Key）？**



有两种经典的设计思路：
1. **统一排序：** 不管字母顺序如何，只要排序后的字符串相同，它们就是字母异位词。
2. **统计词频：** 既然字母异位词的字符种类和数量相同，我们可以统计每个字母出现的次数，将这个频次记录作为特征值。

---

## 我的解法：排序字符串作为哈希键

这是非常符合直觉且逻辑严密的解法。通过 C++ STL 中的 `sort` 函数，将每个字符串重新排序后作为键存入 `unordered_map`。你很好地运用了引用 `&` 来避免不必要的字符串拷贝，这在大一阶段是很好的工程习惯。

```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>> res;
        unordered_map<string,vector<string>> map;
        
        // 遍历所有字符串
        for(string& str : strs)
        {
            string key = str;
            // 将排序后的字符串作为哈希表的 key
            sort(key.begin(), key.end());
            map[key].push_back(str);
        }
        
        // 遍历哈希表，提取分组结果
        for(auto& pair : map)
        {
            res.push_back(pair.second);
        }
        return res;
    }
};

```

* **时间复杂度：*$O(N \cdot K \log K)$* 。其中 $N$ 是 `strs` 的字符串数量，$K$ 是 `strs` 中字符串的最大长度。外层遍历 $N$ 次，内部使用 `sort` 排序字符串的时间复杂度为 $O(K \log K)$ 。
* **空间复杂度：*$O(N \cdot K)$* 。哈希表需要存储所有的字符串及其对应的键。

---

## 更优解法：字符频次作为哈希键 (计数法)

上述解法的性能瓶颈在于**排序**。如果字符串非常长， 的排序开销会比较大。
由于题目限定了字符串仅包含小写字母，我们可以用一个长度为 26 的数组记录每个字母的出现次数，然后将这个频次记录转化为一个字符串作为键。这就把  的排序过程降维打击成了  的一次遍历。

```cpp
class Solution {
public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> map;
        
        for (string& str : strs) {
            // 利用一个长度为26的string模拟频次数组，初始值为0
            // 这种方法避免了手写哈希函数，巧妙利用了 string 可以直接作为 unordered_map 的 key
            string key(26, 0); 
            
            // 统计每个字符的出现次数
            for (char c : str) {
                key[c - 'a']++; 
            }
            
            // 将原始字符串加入对应的频次特征键下
            map[key].push_back(str);
        }
        
        vector<vector<string>> res;
        for (auto& pair : map) {
            res.push_back(pair.second);
        }
        return res;
    }
};

```

* **时间复杂度：*$O(N \cdot K)$* 。遍历数组 $N$ 次，每次仅需遍历一遍字符串（长度为 $K$ ）进行计数，消除了排序的对数时间开销。
* **空间复杂度：*$O(N \cdot K)$* 。哈希表的空间消耗与排序法一致。

---

## 总结

| 解法 | 核心键 (Key) 设计 | 时间复杂度 | 空间复杂度 | 适用场景分析 |
| --- | --- | --- | --- | --- |
| **排序法** (我的解法) | 将单词按字典序排列后的字符串 | $O(N \cdot K \log K)$ | $O(N \cdot K)$ | 编码简单直观，适用于字符串平均长度较短的情况。 |
| **计数法** (更优解法) | 记录26个字母频次的定长字符串 | $O(N \cdot K)$ | $O(N \cdot K)$ | 算法进阶，用空间/巧妙的结构换取时间，适合长字符串处理。 |

**学习心得：**
这道题目极大地锻炼了对**哈希表映射关系**的理解。对于计算机专业的学生来说，做这道题最大的收获应该是建立**“特征提取”**的思维。当无法直接进行归类时，思考“如何通过处理（排序、计数），让同一类的对象暴露出一模一样的特征标识”，这是未来学习复杂数据库索引、数据挖掘聚类等高阶技术的基础模型。
