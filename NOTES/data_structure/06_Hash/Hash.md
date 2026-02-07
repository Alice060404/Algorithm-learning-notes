[MindMap](./Hash.png)


# C++ 学习笔记：哈希 (Hash) 与哈希表

## 1. 哈希概述

哈希（Hash）是将任意长度的输入（Keys）通过散列算法变换成固定长度的输出（Hash Value/Digest）的过程。其核心思想是**以空间换时间**，通过将 Key 映射到数组下标（Bucket Index），实现对数据的快速访问。

## 2. 哈希算法 (Hash Algorithm)

### 2.1 核心目标

一个优秀的哈希算法应具备以下特征：

* **确定性 (Deterministic)**：对于相同的输入，必须产生相同的哈希值。这是哈希表能够正常工作的前提。
* **高效率 (High Efficiency)**：计算过程应尽量简单快速，时间复杂度应为 O(1)，不应成为查找操作的瓶颈。
* **均匀分布 (Uniform Distribution)**：哈希值应尽可能均匀地分布在地址空间中，以最大程度减少哈希冲突（Avalanche Effect，雪崩效应，即输入微小改变导致输出巨大改变也是理想特性之一）。

### 2.2 设计原理 (理论总结)

* **加法哈希 (Additive Hash)**：将字符串中的所有字符的 ASCII 码相加。简单但分布均匀性较差。

```cpp
/* 加法哈希 */
int addHash(string key) {
    long long hash = 0;
    const int MODULUS = 1000000007;
    for (unsigned char c : key) {
        hash = (hash + (int)c) % MODULUS;
    }
    return (int)hash;
}
```
* **乘法哈希 (Multiplicative Hash)**：利用位运算和乘法（如 JDK 中的 31 乘法），利用了乘法溢出丢弃高位的特性来打散数据。

```cpp
/* 乘法哈希 */
int mulHash(string key) {
    long long hash = 0;
    const int MODULUS = 1000000007;
    for (unsigned char c : key) {
        hash = (31 * hash + (int)c) % MODULUS;
    }
    return (int)hash;
}
```
* **异或哈希 (XOR Hash)**：利用异或运算的特性混合 Key 的各个部分。

```cpp
/* 异或哈希 */
int xorHash(string key) {
    int hash = 0;
    const int MODULUS = 1000000007;
    for (unsigned char c : key) {
        hash ^= (int)c;
    }
    return hash & MODULUS;
}
```
* **旋转哈希 (Rotating Hash)**：结合位移（Left/Right Shift）和异或，不仅处理字符本身，还处理字符的位置顺序。

```cpp
/* 旋转哈希 */
int rotHash(string key) {
    long long hash = 0;
    const int MODULUS = 1000000007;
    for (unsigned char c : key) {
        hash = ((hash << 4) ^ (hash >> 28) ^ (int)c) % MODULUS;
    }
    return (int)hash;
}
```

### 2.3 常见哈希算法

* **MD5**：曾广泛使用的 128 位算法，因存在碰撞风险，目前主要用于完整性校验，不再用于安全验证。
* **SHA-1**：160 位算法，已被证明不安全。
* **SHA-2 / SHA-3**：目前主流的安全散列算法，广泛用于加密和数字签名。

### 2.4 C++ 标准库支持

C++ STL 提供了 `std::hash` 模板结构体，支持基本数据类型（如 `int`, `string`, `double` 等）的哈希计算。

## 3. 哈希冲突 (Hash Collision)

由于哈希值的输出空间通常小于输入的可能空间（抽屉原理），冲突是不可避免的。处理冲突的效率直接决定了哈希表的性能。

### 3.1 负载因子 (Load Factor)

* **定义**：，其中  是表中元素个数， 是哈希表的槽位（Bucket）数量。
* **意义**： 越大，冲突概率越高，查找性能下降； 越小，空间浪费越多。通常当  超过某个阈值（如 0.75）时，需要触发**扩容 (Resizing)**。

### 3.2 解决冲突的策略

#### A. 链式寻址法 (Separate Chaining)

* **原理**：每个 Bucket 维护一个链表（或红黑树）。当发生冲突时，将新元素追加到该 Bucket 对应的链表中。
* **优点**：实现简单，对负载因子不敏感（ 可以大于 1），删除操作简单。
* **缺点**：指针占用额外空间，链表非连续内存导致缓存（Cache）性能较差。

#### B. 开放寻址法 (Open Addressing)

* **原理**：所有元素都存储在哈希表数组中。当发生冲突时，按照某种探测序列寻找下一个空闲位置。
* **线性寻址 (Linear Probing)**：。每次向后移动一步。容易产生“一次聚集”现象。
* **平方寻址 (Quadratic Probing)**：。步长按二次方增长，缓解了一次聚集，但可能产生“二次聚集”。
* **多次哈希 (Double Hashing)**：使用第二个哈希函数计算步长。

### 3.3 扩容 (Resizing / Rehashing)

当负载因子过高时，申请一个更大的数组（通常是原大小的 2 倍），将所有元素重新计算哈希值放入新数组。这是一个 O(N) 的重操作，应尽量避免。

## 4. 哈希表简单实现 (C++ Code)

以下是一个基于 **链式寻址法 (Separate Chaining)** 的 C++ 哈希表实现。为了体现大一计算机学生的专业性，代码使用了模板（Template）以支持不同类型，并实现了动态扩容。

```cpp
/**
 * @brief 基于拉链法（Separate Chaining）的哈希表简单实现
 * @note 仅用于学习哈希原理，生产环境请使用 std::unordered_map
 */

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <functional> // for std::hash

// 定义哈希表节点
template <typename K, typename V>
struct HashNode {
    K key;
    V value;

    HashNode(K k, V v) : key(k), value(v) {}
};

// 哈希表类
template <typename K, typename V>
class HashTable {
private:
    // 使用 vector 存储桶（Bucket），每个桶是一个 list（链表）
    std::vector<std::list<HashNode<K, V>>> buckets;
    size_t current_size; // 当前元素个数
    const double LOAD_FACTOR_THRESHOLD = 0.75; // 扩容阈值

    // 计算哈希索引
    size_t hashFunction(const K& key) const {
        std::hash<K> hasher;
        return hasher(key) % buckets.size();
    }

    // 扩容操作
    void rehash() {
        std::cout << "[System] Load factor exceeded, resizing table..." << std::endl;
        auto old_buckets = buckets;
        
        // 容量翻倍
        buckets.resize(buckets.size() * 2);
        // 清空当前的新桶，因为 resize 后可能会保留旧数据或默认初始化
        for (auto& bucket : buckets) {
            bucket.clear();
        }
        current_size = 0;

        // 将旧数据重新插入
        for (const auto& bucket : old_buckets) {
            for (const auto& node : bucket) {
                insert(node.key, node.value);
            }
        }
    }

public:
    // 构造函数，初始化桶的大小
    HashTable(size_t size = 10) : current_size(0) {
        buckets.resize(size);
    }

    // 添加元素 (Insert)
    void insert(const K& key, const V& value) {
        size_t index = hashFunction(key);
        auto& bucket = buckets[index];

        // 检查键是否存在，若存在则更新值
        for (auto& node : bucket) {
            if (node.key == key) {
                node.value = value;
                return;
            }
        }

        // 不存在，插入新节点
        bucket.emplace_back(key, value);
        current_size++;

        // 检查负载因子
        if (static_cast<double>(current_size) / buckets.size() > LOAD_FACTOR_THRESHOLD) {
            rehash();
        }
    }

    // 查找元素 (Find)
    bool find(const K& key, V& outValue) {
        size_t index = hashFunction(key);
        auto& bucket = buckets[index];

        for (const auto& node : bucket) {
            if (node.key == key) {
                outValue = node.value;
                return true;
            }
        }
        return false;
    }

    // 删除元素 (Delete)
    bool remove(const K& key) {
        size_t index = hashFunction(key);
        auto& bucket = buckets[index];

        // list 的 erase 需要迭代器
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->key == key) {
                bucket.erase(it);
                current_size--;
                return true;
            }
        }
        return false;
    }

    // 打印当前哈希表结构（调试用）
    void printTable() {
        std::cout << "--- Hash Table Content ---" << std::endl;
        for (size_t i = 0; i < buckets.size(); ++i) {
            std::cout << "Bucket " << i << ": ";
            if (buckets[i].empty()) {
                std::cout << "(empty)";
            } else {
                for (const auto& node : buckets[i]) {
                    std::cout << "[" << node.key << ":" << node.value << "] -> ";
                }
                std::cout << "NULL";
            }
            std::cout << std::endl;
        }
        std::cout << "--------------------------" << std::endl;
    }
};

// 测试代码
int main() {
    // 演示 std::hash 的基本用法
    std::string str = "Hello C++";
    std::cout << "Std::hash for '" << str << "': " << std::hash<std::string>{}(str) << std::endl;

    // 演示自定义哈希表
    HashTable<std::string, int> myMap;

    std::cout << "\nInserting elements..." << std::endl;
    myMap.insert("Alice", 95);
    myMap.insert("Bob", 89);
    myMap.insert("Charlie", 70);
    myMap.insert("David", 92); // 此时应该还没触发扩容

    myMap.printTable();

    std::cout << "\nInserting trigger for resize..." << std::endl;
    // 假设初始大小为 10，阈值 0.75，插入第 8 个元素时可能会触发（取决于具体实现逻辑）
    // 这里为了演示，我们手动添加更多元素
    myMap.insert("Eve", 88);
    myMap.insert("Frank", 76);
    myMap.insert("Grace", 99);
    myMap.insert("Heidi", 60); 

    myMap.printTable();

    std::cout << "\nSearching for 'Alice'..." << std::endl;
    int score;
    if (myMap.find("Alice", score)) {
        std::cout << "Alice's score is: " << score << std::endl;
    } else {
        std::cout << "Alice not found." << std::endl;
    }

    std::cout << "\nRemoving 'Bob'..." << std::endl;
    myMap.remove("Bob");
    if (myMap.find("Bob", score)) {
        std::cout << "Bob found." << std::endl;
    } else {
        std::cout << "Bob successfully removed." << std::endl;
    }

    return 0;
}

```