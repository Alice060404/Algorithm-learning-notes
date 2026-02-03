[MindMap](./List.png)


## 1. 核心概念

`std::vector` 是 C++ 标准模板库 (STL) 中最常用的序列容器。它本质上是一个**动态数组**。

* **连续内存**：元素在内存中是连续存储的（这一点非常重要），这意味着它支持  时间复杂度的随机访问，且对 CPU 缓存友好。
* **动态扩容**：与普通数组（`int arr[10]`）不同，`vector` 可以根据需要自动管理内存，动态调整大小。

## 2. 初始化 (Initialization)

`vector` 提供了多种构造方式，适应不同的场景：

* **默认构造**：创建一个空 vector。
* **填充构造**：指定大小和初始值。
* **列表初始化** (C++11)：使用 `{}` 直接赋值。
* **拷贝构造**：从另一个 vector 复制。

## 3. 元素操作 (Element Operations)

### 3.1 访问元素

* **下标访问 `[]`**：效率最高，但不进行边界检查。如果越界，行为未定义（通常导致程序崩溃）。
* **`at()` 方法**：比 `[]` 慢一点，但更安全。如果越界，会抛出 `std::out_of_range` 异常。
* **`front()` / `back()`**：直接访问第一个和最后一个元素。

### 3.2 插入元素

* **尾部插入**：
* `push_back(val)`: 将元素拷贝或移动到末尾。
* `emplace_back(args...)`: (C++11) 直接在 vector 尾部构造元素，省去了拷贝/移动过程，效率更高。


* **任意位置插入**：
* `insert(iter, val)`: 在迭代器指向的位置前插入元素。**注意**：这会导致插入点之后的所有元素向后移动，开销较大 ()。



### 3.3 删除元素

* **尾部删除**：`pop_back()`， 时间复杂度。
* **任意位置删除**：`erase(iter)`，删除迭代器指向的元素，后续元素前移，。
* **清空**：`clear()`，移除所有元素，size 变为 0，但 capacity（容量）通常不变。

## 4. 列表操作 (List Operations)

### 4.1 遍历列表

* **索引循环**：传统 `for(int i=0; ...)` 方式。
* **迭代器**：使用 `begin()` 和 `end()` 进行遍历。
* **范围 For 循环** (C++11)：`for(auto& x : vec)`，语法最简洁，推荐使用。

### 4.2 拼接列表

将一个 vector 的内容追加到另一个 vector 后面，通常使用 `insert` 方法配合迭代器范围：
`vec1.insert(vec1.end(), vec2.begin(), vec2.end());`

### 4.3 排序列表

`vector` 本身没有 `sort` 成员函数，必须使用 `<algorithm>` 头文件中的 `std::sort`。

* 默认为升序。
* 可以使用自定义比较函数（或 Lambda 表达式）实现降序或复杂对象排序。

## 5. 底层实现与内存机制 (Implementation Details)

这是作为计算机专业学生必须理解的重点：

* **Size vs Capacity**：
* `size()`: 当前容器中实际元素的个数。
* `capacity()`: 当前容器在不需要重新分配内存的情况下能容纳的元素个数。


* **扩容机制**：当 `push_back` 导致 `size > capacity` 时，vector 会申请一块更大的新内存（通常是原来的 1.5 倍或 2 倍），将旧数据**拷贝**（或移动）过去，然后释放旧内存。
* **性能隐患**：频繁扩容代价极高。
* **优化**：如果预先知道数据量，应使用 `reserve(n)` 提前预留空间，避免多次内存分配。



---

## 代码示例

你可以直接复制以下代码到 `vector_study.cpp` 文件中并在 WSL 环境下编译运行。

```cpp
#include <iostream>
#include <vector>
#include <algorithm> // 用于 std::sort
#include <stdexcept> // 用于异常处理

// 辅助函数：打印 vector 信息
void printVector(const std::vector<int>& v, const std::string& name) {
    std::cout << name << ": { ";
    for (const auto& elem : v) {
        std::cout << elem << " ";
    }
    std::cout << "} | Size: " << v.size() << " | Capacity: " << v.capacity() << std::endl;
}

int main() {
    // ==========================================
    // 1. 初始化 (Initialization)
    // ==========================================
    std::cout << "=== 1. 初始化 ===" << std::endl;
    
    std::vector<int> v1;                        // 默认构造：空 vector
    std::vector<int> v2(5, 10);                 // 填充构造：5个元素，值都是10
    std::vector<int> v3 = {1, 2, 3, 4, 5};      // 列表初始化 (C++11)
    std::vector<int> v4(v3);                    // 拷贝构造

    printVector(v3, "v3");

    // ==========================================
    // 2. 元素操作 (Element Operations)
    // ==========================================
    std::cout << "\n=== 2. 元素操作 ===" << std::endl;

    // --- 插入 ---
    // push_back: 将元素拷贝/移动到末尾
    v1.push_back(100);
    v1.push_back(200);
    
    // emplace_back: 在末尾直接构造元素 (通常比 push_back 更高效)
    v1.emplace_back(300); 
    
    // insert: 在开头插入 (效率较低，因为后续元素都要后移)
    v1.insert(v1.begin(), 50); 
    printVector(v1, "v1 (插入后)");

    // --- 访问 ---
    std::cout << "v1[0] (下标访问): " << v1[0] << std::endl;
    try {
        // at() 会进行边界检查，越界会抛出异常
        std::cout << "v1.at(10) (at访问): " << v1.at(10) << std::endl; 
    } catch (const std::out_of_range& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
    }
    std::cout << "Front: " << v1.front() << ", Back: " << v1.back() << std::endl;

    // --- 删除 ---
    v1.pop_back(); // 删除最后一个元素
    // erase: 删除第二个元素 (迭代器指向的位置)
    v1.erase(v1.begin() + 1); 
    printVector(v1, "v1 (删除后)");

    // ==========================================
    // 3. 列表操作 (List Operations)
    // ==========================================
    std::cout << "\n=== 3. 列表操作 ===" << std::endl;

    // --- 遍历 (Traversal) ---
    std::cout << "遍历 v3: ";
    // 方式一：范围 for 循环 (推荐)
    for (int x : v3) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    // 方式二：迭代器遍历
    std::cout << "迭代器遍历: ";
    for (std::vector<int>::iterator it = v3.begin(); it != v3.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // --- 拼接 (Concatenation) ---
    // 将 v3 的内容追加到 v1 后面
    std::cout << "拼接前 v1 size: " << v1.size() << std::endl;
    v1.insert(v1.end(), v3.begin(), v3.end());
    printVector(v1, "v1 (拼接 v3 后)");

    // --- 排序 (Sorting) ---
    // 引入乱序数据
    v1.push_back(5);
    v1.push_back(99);
    
    // 使用 std::sort 进行升序排序
    std::sort(v1.begin(), v1.end());
    printVector(v1, "v1 (升序排序后)");

    // 使用 Lambda 表达式进行降序排序
    std::sort(v1.begin(), v1.end(), [](int a, int b) {
        return a > b;
    });
    printVector(v1, "v1 (降序排序后)");

    // ==========================================
    // 4. 内存机制演示 (Memory Mechanics)
    // ==========================================
    std::cout << "\n=== 4. 内存扩容机制 ===" << std::endl;
    std::vector<int> v_demo;
    
    // 预留空间 (Reserve)
    // 如果不写这行，你会看到 Capacity 多次改变，发生多次内存重分配
    // 写了这行，Capacity 会直接锁定，提升性能
    v_demo.reserve(10); 
    
    std::cout << "初始状态: Size=" << v_demo.size() << ", Capacity=" << v_demo.capacity() << std::endl;
    
    for (int i = 0; i < 6; ++i) {
        v_demo.push_back(i);
        std::cout << "Push " << i << ": Size=" << v_demo.size() 
                  << ", Capacity=" << v_demo.capacity() << std::endl;
    }

    return 0;
}

```