[MindMap](./MindMap.png)


# 数据结构与计算机基础体系

## 一、 逻辑结构 (Logical Structure)

逻辑结构描述的是数据元素之间的逻辑关系，与它们在计算机内存中如何存储无关。这是算法设计的核心。

### 1. 线性结构 (Linear)

数据元素之间存在“一对一”的线性关系。

* **数组 (Array):**
* **特点:** 元素在逻辑上是连续的。支持**随机访问 (Random Access)**，时间复杂度为 。
* **C++对应:** 原生数组 `int arr[10]` 或 STL容器 `std::vector`, `std::array`。


* **链表 (Linked List):**
* **特点:** 元素在逻辑上相连，但通过指针链接。不支持随机访问，插入和删除操作通常比数组更高效（无需移动后续元素）。
* **C++对应:** `std::list` (双向链表), `std::forward_list` (单向链表)。


* **栈 (Stack):**
* **特点:** 后进先出 (LIFO)。限制只能在一端进行插入和删除。
* **应用:** 函数调用堆栈、表达式求值。
* **C++对应:** `std::stack`。


* **队列 (Queue):**
* **特点:** 先进先出 (FIFO)。一端进，另一端出。
* **C++对应:** `std::queue`, `std::deque` (双端队列)。


* **哈希表 (Hash Table) (线性视角):**
* 虽然哈希表通常被视为集合，但如果处理哈希冲突采用“开放寻址法”，数据在线性数组中排列，也可以视为一种特殊的线性组织形式。



### 2. 非线性结构 (Non-linear)

数据元素之间存在“一对多”或“多对多”的关系。

* **树 (Tree):**
* **特点:** 层次结构，包含根节点、子节点。
* **重点:** 二叉树 (Binary Tree)、二叉搜索树 (BST)、平衡树 (AVL, Red-Black Tree)。


* **堆 (Heap):**
* **特点:** 一种特殊的完全二叉树。大顶堆 (父节点  子节点) 或 小顶堆。
* **应用:** 优先队列 (Priority Queue)。
* **C++对应:** `std::priority_queue`, `std::make_heap`。


* **图 (Graph):**
* **特点:** 节点 (Vertex) 和边 (Edge) 组成，表示多对多关系。
* **分类:** 有向图/无向图，带权图/无权图。


* **哈希表 (Hash Table) (非线性视角):**
* **特点:** 通过哈希函数映射键值对 (Key-Value)。
* **C++对应:** `std::unordered_map`, `std::unordered_set`。



---

## 二、 物理结构 (Physical/Storage Structure)

物理结构描述数据在计算机内存中的实际存储方式。

### 1. 连续存储 (Contiguous)

* **定义:** 占用一块连续的内存地址空间。
* **优势:** 空间利用率高（无额外指针开销），缓存命中率 (Cache Locality) 高。
* **劣势:** 内存分配需要预知大小，扩展困难（通常需要重新分配并复制）。
* **代表:** 数组、`std::vector`。

### 2. 分散存储 (Dispersed/Linked)

* **定义:** 元素可以散落在内存的任意位置，依靠指针 (Pointer) 关联。
* **优势:** 内存利用灵活，不会产生碎片，易于动态扩展。
* **劣势:** 每个元素需要额外的空间存储指针，缓存命中率低，访问速度慢。
* **代表:** 链表、树的节点。

---

## 三、 基本数据类型与数值限制


### 1. 占用空间 (Memory Size)

使用 `sizeof` 运算符查看。

* `char`: 1 byte
* `int`: 通常 4 bytes (取决于编译器和架构)
* `long long`: 8 bytes
* `float`: 4 bytes
* `double`: 8 bytes

### 2. 极值 (Limits)

在 `<limits>` 头文件中定义。了解极值是为了防止**溢出 (Overflow)**。

---

## 四、 数字编码 (Number Encoding)

这是计算机组成原理的核心内容，直接关系到C++中位运算和类型转换的结果。

### 1. 整数编码

计算机内部统一使用**补码**存储整数。

* **原码 (Sign-Magnitude):** 最高位为符号位，其余为数值位。
* **反码 (Ones' Complement):** 正数同原码；负数符号位不变，数值位按位取反。
* **补码 (Two's Complement):**
* 正数：同原码。
* 负数：反码 + 1。
* **意义:** 解决了 +0 和 -0 的二义性，并将减法运算统一为加法运算。



**相关公式 (LaTeX源码):**

```latex
% 原码 (x为真值)
[x]_{\text{原}} = \begin{cases} x, & 0 \le x < 2^{n-1} \\ 2^{n-1} - x, & - 2^{n-1} < x \le 0 \end{cases}

% 反码
[x]_{\text{反}} = \begin{cases} [x]_{\text{原}}, & x \ge 0 \\ \text{按位取反}(\text{数值位}), & x < 0 \end{cases}

% 补码计算公式
[x]_{\text{补}} = \begin{cases} [x]_{\text{原}}, & x \ge 0 \\ [x]_{\text{反}} + 1, & x < 0 \end{cases}

% 补码的核心优势：模运算
A - B = A + (-B) = [A]_{\text{补}} + [-B]_{\text{补}}

```

### 2. 浮点数编码

遵循 **IEEE 754** 标准。

* **结构:** 符号位 (S) + 指数位 (E) + 尾数位 (M)。
* `float`: 1位符号 + 8位指数 + 23位尾数。
* `double`: 1位符号 + 11位指数 + 52位尾数。

**浮点数真值公式 (LaTeX源码):**

```latex
% IEEE 754 浮点数值
V = (-1)^S \times M \times 2^{E - \text{Bias}}

% Bias (偏移量)
\text{Bias}_{float} = 127
\text{Bias}_{double} = 1023

```

---

## 五、 字符编码 (Character Encoding)

处理文本（尤其是中文）时的常见痛点。

* **ASCII:** 7位编码，覆盖英文字母和符号。
* **GBK:** 汉字内码扩展规范，双字节编码，专门针对简体中文。
* **Unicode:** 万国码，为世界上所有字符分配唯一的ID (Code Point)，但未规定存储方式。
* **UTF-8:** Unicode的一种变长存储实现。英文1字节，中文通常3字节。
* **C++:** 现代C++推荐使用 UTF-8，C++20 引入了 `char8_t`。



---

## 六、 C++ 代码示例与验证

这段代码演示了笔记中的核心概念：类型限制、补码验证、以及底层内存查看。

```cpp
#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <bitset>
#include <cstring>

// 辅助函数：打印变量的二进制表示（逐字节）
// 针对大一学生：这能帮你看到数据在内存里到底长什么样
template<typename T>
void printBinary(T val) {
    // 使用 unsigned char 指针读取内存的每一个字节
    const unsigned char* bytePtr = reinterpret_cast<const unsigned char*>(&val);
    std::cout << "Value: " << val << " | Binary (Memory): ";
    for (size_t i = 0; i < sizeof(T); ++i) {
        // C++中低地址通常先打印（小端序机器上，低地址存低位）
        // 这里为了阅读习惯，我们可能需要反向打印，但直接打印内存顺序更直观地反应物理结构
        std::bitset<8> byte(bytePtr[i]);
        std::cout << byte << " ";
    }
    std::cout << std::endl;
}

int main() {
    // ---------------------------------------------------------
    // 1. 物理结构对比：连续 vs 分散
    // ---------------------------------------------------------
    std::cout << "=== Physical Structure Analysis ===" << std::endl;
    
    // 连续存储 (Array/Vector)
    std::vector<int> vec = {1, 2, 3};
    std::cout << "Vector address of [0]: " << &vec[0] << std::endl;
    std::cout << "Vector address of [1]: " << &vec[1] << std::endl;
    std::cout << "Difference: " << (char*)&vec[1] - (char*)&vec[0] << " bytes (Continuous)" << std::endl;

    // 分散存储 (List)
    std::list<int> lst = {1, 2, 3};
    auto it = lst.begin();
    int* addr1 = &(*it);
    it++;
    int* addr2 = &(*it);
    std::cout << "List node 1 address: " << addr1 << std::endl;
    std::cout << "List node 2 address: " << addr2 << std::endl; 
    std::cout << "(Addresses are not contiguous and likely far apart)" << std::endl;

    // ---------------------------------------------------------
    // 2. 基本数据类型与极值
    // ---------------------------------------------------------
    std::cout << "\n=== Data Types Limits ===" << std::endl;
    std::cout << "int max: " << std::numeric_limits<int>::max() << std::endl;
    std::cout << "int min: " << std::numeric_limits<int>::min() << std::endl;
    
    // ---------------------------------------------------------
    // 3. 数字编码：补码验证
    // ---------------------------------------------------------
    std::cout << "\n=== Number Encoding (Two's Complement) ===" << std::endl;
    int a = 5;
    int b = -5;
    
    // 5 的二进制: 00000101 (假设8位简化演示)
    // -5 的二进制: 11111011 (反码 11111010 + 1)
    printBinary(a);
    printBinary(b); // 观察输出，你会看到所有的1都变成了0并加了1

    // ---------------------------------------------------------
    // 4. 字符编码 (C++ Handling)
    // ---------------------------------------------------------
    std::cout << "\n=== Character Encoding ===" << std::endl;
    // 这是一个普通的 char 数组，具体编码取决于源文件保存格式（通常是UTF-8）和编译器设置
    const char* str = "中"; 
    // 在UTF-8中，“中”占用3个字节: E4 B8 AD
    std::cout << "String: \"中\"" << std::endl;
    std::cout << "Bytes length: " << strlen(str) << std::endl;
    
    std::cout << "Hex dump: ";
    for(size_t i = 0; i < strlen(str); ++i) {
        // 强转为 unsigned char 以避免打印出负数的补码形式
        printf("%02X ", (unsigned char)str[i]);
    }
    std::cout << std::endl;
    // 如果输出是 E4 B8 AD，说明是 UTF-8
    // 如果输出是 D6 D0，说明是 GBK (取决于你的系统环境)

    return 0;
}

```