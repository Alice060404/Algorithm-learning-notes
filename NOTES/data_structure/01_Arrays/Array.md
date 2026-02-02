[MindMap](./Array.png)


### 原始数组 (Primitive Array)

#### 1. 核心定义与初始化

原始数组是一种**线性表**数据结构，它用一组**连续的内存空间**，来存储一组具有**相同类型**的数据。

* **连续内存**：这是数组最本质的特征，也是实现“随机访问”的物理基础。
* **类型固定**：C++作为强类型语言，原始数组中的每个元素占用的字节数必须一致。

**初始化方式的拓展：**

* **静态分配 (Stack)**：编译时确定大小，内存分配在栈上。生命周期随作用域结束而结束。
* **动态分配 (Heap)**：运行时确定大小，使用 `new` 关键字在堆上分配。需要手动 `delete[]` 释放，否则会导致内存泄漏。

#### 2. 核心操作与复杂度分析

##### 2.1 访问元素 (Access)

* **机制**：利用数组下标（Index）进行直接定位。
* **原理**：由于内存连续且元素大小固定，计算机可以通过一个简单的数学公式计算出任意下标元素的内存地址。
* **寻址公式**：

```latex
$$ Address[i] = BaseAddress + i \times sizeof(DataType) $$

```

* **时间复杂度**：

```latex
$$ O(1) $$

```

##### 2.2 查找元素 (Search)

* **按值查找**：需要遍历数组比对。
* **普通查找**：时间复杂度为 。
* **二分查找 (Binary Search)**：如果数组已经有序，时间复杂度可优化至 。

##### 2.3 插入与删除 (Insert & Delete)

* **痛点**：由于数组要求内存连续，在数组中间插入或删除元素时，必须移动该位置之后的所有元素。
* **插入**：需要将插入点及之后的数据向后移动一位，腾出空位。
* **删除**：需要将删除点之后的数据向前移动一位，覆盖空位。
* **时间复杂度**：
* 最好情况（尾部操作）：
* 最坏情况（头部操作）：
* 平均复杂度：



```latex
$$ O(N) $$

```

#### 3. 扩容机制 (Resizing)

原始数组在C++中一旦创建，其**物理内存大小即固定**。

* **不可扩容性**：你不能直接在原内存块后面“拼接”一段新内存，因为后面的地址可能已经被其他程序或变量占用了。
* **扩容的本质（伪扩容）**：
1. **Allocate**：在堆区申请一块更大的新内存空间。
2. **Copy**：将原数组的数据复制到新数组中。
3. **Free**：释放原数组的内存空间。
4. **Update**：将指针指向新数组。


* **代价**：扩容操作涉及大量的数据复制，性能开销极大。

#### 4. 优缺点与计算机系统视角

##### 4.1 优点

1. **空间效率高**：除了存储数据本身，不需要像链表那样额外存储“下一个节点的地址（指针）”。
2. **支持随机访问**：这是数组相比链表最大的优势，也是许多高效算法（如二分查找、快速排序、哈希表）的基础。
3. **CPU缓存友好 (Cache Locality)**：
* **空间局部性 (Spatial Locality)**：CPU读取数据时，会把相邻的数据通过Cache Line一同加载到高速缓存中。因为数组内存连续，遍历数组时命中率极高，能大幅提升程序运行速度。



##### 4.2 局限性

1. **插入删除低效**：频繁的增删操作会导致大量数据搬移，不适合作为动态变化频繁的数据容器。
2. **长度不可变**：使用前必须预估大小。估大了浪费空间，估小了需要耗时的扩容。
3. **空间碎片风险**：若申请超大数组（如几百MB），即使内存总量足够，但如果没有足够大的**连续**内存块，申请也会失败。

---

### C++ 代码实现 (GitHub 笔记版)

以下代码展示了原始数组的创建、增删改查逻辑以及底层的扩容模拟。

```cpp
/**
 * 原始数组操作演示
 * Description: 演示数组的内存布局、CRUD操作及手动扩容机制
 */

#include <iostream>
#include <stdexcept> // 用于抛出异常

// 为了方便演示，我们封装一个简单的结构体来管理动态数组
// 在实际工程中，这正是 std::vector 的雏形
struct DynamicArray {
    int* data;      // 指向堆区连续内存的指针
    int capacity;   // 数组总容量
    int size;       // 当前元素个数

    // 构造函数：初始化数组
    DynamicArray(int cap) {
        if (cap <= 0) cap = 10; // 默认容量
        capacity = cap;
        size = 0;
        data = new int[capacity]; // 在堆上申请连续内存
        std::cout << "[Init] Array created with capacity: " << capacity << std::endl;
    }

    // 析构函数：释放内存
    ~DynamicArray() {
        if (data != nullptr) {
            delete[] data; // 必须使用 delete[] 释放数组
            data = nullptr;
        }
        std::cout << "[Destroy] Memory released." << std::endl;
    }

    // 1. 访问元素 (Access) - O(1)
    int get(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        // 指针运算：*(address + index)
        return data[index];
    }

    // 扩容逻辑 (Resize) - O(N)
    // 当容量不足时触发
    void resize() {
        int newCapacity = capacity * 2; // 通常按2倍扩容
        std::cout << "[Resize] Expanding from " << capacity << " to " << newCapacity << "..." << std::endl;
        
        // A. 申请新空间
        int* newData = new int[newCapacity];
        
        // B. 复制数据
        for (int i = 0; i < size; ++i) {
            newData[i] = data[i];
        }
        
        // C. 释放旧空间
        delete[] data;
        
        // D. 更新指针和属性
        data = newData;
        capacity = newCapacity;
    }

    // 2. 插入元素 (Insert) - 平均 O(N)
    void insert(int index, int value) {
        if (index < 0 || index > size) {
            std::cout << "Insert failed: Invalid index." << std::endl;
            return;
        }

        // 检查是否需要扩容
        if (size == capacity) {
            resize();
        }

        // 数据搬移：从后往前，将 index 之后的元素向后移
        // 必须从后往前，否则会覆盖数据
        for (int i = size; i > index; --i) {
            data[i] = data[i - 1];
        }

        data[index] = value;
        size++;
        std::cout << "[Insert] Value " << value << " inserted at index " << index << std::endl;
    }

    // 尾部插入 (Push Back) - O(1) (Amortized)
    void push_back(int value) {
        insert(size, value);
    }

    // 3. 删除元素 (Delete) - 平均 O(N)
    void remove(int index) {
        if (index < 0 || index >= size) {
            std::cout << "Remove failed: Invalid index." << std::endl;
            return;
        }

        int removedVal = data[index];

        // 数据搬移：从前往后，将 index 之后的元素向前移
        for (int i = index; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }

        size--;
        std::cout << "[Remove] Value " << removedVal << " removed from index " << index << std::endl;
    }

    // 打印数组详情
    void print() {
        std::cout << "Array Content (Size: " << size << "/" << capacity << "): [ ";
        for (int i = 0; i < size; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << "]" << std::endl;
    }
};

int main() {
    // 创建一个容量为3的数组
    DynamicArray arr(3);

    // 插入数据
    arr.push_back(10); // index 0
    arr.push_back(20); // index 1
    arr.push_back(30); // index 2
    arr.print();

    // 触发扩容
    std::cout << "\n--- Triggering Resize ---" << std::endl;
    arr.push_back(40); // 此时容量不足，将触发 resize
    arr.print();

    // 中间插入
    std::cout << "\n--- Inserting in Middle ---" << std::endl;
    arr.insert(1, 99); // 在索引1插入99
    arr.print();

    // 删除数据
    std::cout << "\n--- Removing Element ---" << std::endl;
    arr.remove(2); // 删除索引2的元素
    arr.print();

    // 随机访问
    std::cout << "\n--- Random Access ---" << std::endl;
    std::cout << "Element at index 2 is: " << arr.get(2) << std::endl;

    return 0;
}

```

### 学习建议

理解 `Primitive Array` 是理解后续高级数据结构（如 `std::vector`, `std::string`, 哈希表底层）的关键。建议在运行上述代码时，使用调试器（GDB 或 VS Code Debugger）逐行跟踪，特别是观察 `resize` 函数执行前后 `data` 指针地址的变化，这能直观地验证“数组扩容就是换了一块新内存”这一结论。