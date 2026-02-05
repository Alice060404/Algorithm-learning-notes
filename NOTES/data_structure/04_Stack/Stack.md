[MindMap](./Stack.png)


# 数据结构笔记：栈 (Stack)

## 1. 核心概念

栈（Stack）是一种线性数据结构，其最显著的特征是**后进先出**（LIFO, Last In First Out）。

* **类比**：就像是一摞盘子，你只能把新盘子放在最上面，也只能从最上面取走盘子。
* **逻辑结构**：限制了插入和删除位置的线性表，只允许在表的一端（栈顶 Top）进行操作。

## 2. 核心操作 (常规操作)

无论底层如何实现，栈对外暴露的接口通常包含以下几个，且时间复杂度均为 ：

* **push(x) / 入栈**：将元素 `x` 压入栈顶。
* **pop() / 出栈**：移除栈顶元素（注意：通常不返回该元素，但在某些标准库实现中可能不同，STL中是`void`返回类型）。
* **top() / peek() / 获取栈顶**：查看栈顶元素的值，但不移除它。
* **empty() / 判空**：检查栈是否为空。
* **size() / 大小**：返回栈中元素的个数。

## 3. 实现方式对比

### 3.1 基于数组实现 (Array-based / Sequential Stack)

这是利用一段连续的内存空间来存储栈元素。

* **实现细节**：
* **栈顶位置**：通常将**数组的尾部**作为栈顶。
* **原因**：在数组尾部插入（push）和删除（pop）元素不需要移动其他元素，时间复杂度为 。如果以数组头部为栈顶，每次操作都需要移动整个数组，复杂度会退化为 。
* **动态扩容**：使用动态数组（如 C++ 的 `std::vector`）可以解决固定容量的问题。当空间不足时，通常会以 2 倍的方式进行扩容（Amortized ）。


* **优点**：
* 内存紧凑，缓存命中率（Cache Locality）高。
* 不需要为每个元素额外存储指针空间。



### 3.2 基于链表实现 (Linked List-based / Chain Stack)

利用非连续的内存空间，通过指针将节点连接起来。

* **实现细节**：
* **栈顶位置**：必须将**链表的头节点（Head）**作为栈顶。
* **原因**：单链表的头插和头删操作复杂度为 。如果以尾部为栈顶，入栈和出栈（特别是删除尾节点）需要遍历整个链表（除非使用双向链表且维护尾指针），会导致效率低下。


* **优点**：
* 不存在“栈满”的问题（只要内存足够）。
* 动态分配内存，不会像数组那样可能存在大量未使用的预留空间。



### 3.3 两种实现的效率对比

| 特性 | 数组实现 (Array/Vector) | 链表实现 (Linked List) |
| --- | --- | --- |
| **时间效率** | 绝大多数操作为 。但在动态扩容时刻可能产生  的瞬时开销。 | 所有操作严格稳定在 ，无扩容抖动。但每次 `new`/`delete` 节点有系统开销。 |
| **空间效率** | 可能会有未使用的预留容量（Capacity > Size）。但每个元素无指针开销。 | 每个元素需要额外的指针域存储地址（32位系统4字节，64位系统8字节）。 |
| **适用场景** | 元素数量主要已知，或关注访问速度与内存局部性。 | 元素数量变化剧烈，且无法预估上限。 |

## 4. 栈的典型应用场景

思维导图中提到的应用场景的底层逻辑如下：

1. **程序内存管理 (函数调用栈 Call Stack)**
* **原理**：这是栈最广泛的应用。每当发生函数调用时，系统会将当前函数的上下文（局部变量、返回地址、参数）压入系统栈（Stack Frame）。函数执行完毕后，栈帧弹出，程序恢复到调用点继续执行。这也解释了为什么无限递归会导致 `Stack Overflow`。


2. **浏览器/编辑器的 后退与前进 (Undo/Redo)**
* **原理**：通常使用**两个栈**来实现。
* **栈 A (History)**：记录用户的操作路径。点击“后退”时，从栈 A 弹出当前页面，放入栈 B。
* **栈 B (Forward)**：存储被“后退”操作移除的页面。点击“前进”时，从栈 B 弹出，重新压入栈 A。




3. **表达式求值与语法解析**
* **原理**：编译器使用栈来处理运算符的优先级（如中缀表达式转后缀表达式），以及检查括号匹配（`{ [ ( ) ] }`）。



---

## 5. C++ 代码实现 (GitHub 笔记版)

这段代码展示了两种实现方式：

1. **`ArrayStack`**：基于 `std::vector` 的数组栈。
2. **`LinkedStack`**：基于自定义节点的链表栈。

代码使用了模板（Template），适用于任何数据类型，并遵循 C++ 标准库的命名习惯。

```cpp
/*
 * Stack Implementation in C++
 * Description: Implementation of Stack using both Dynamic Array (std::vector) and Linked List.
 */

#include <iostream>
#include <vector>
#include <stdexcept> // For std::out_of_range

// ==========================================
// 1. 基于动态数组的实现 (Array Stack)
// 利用 std::vector 作为底层容器
// ==========================================
template <typename T>
class ArrayStack {
private:
    std::vector<T> data;

public:
    // 入栈：在数组尾部添加元素 -> O(1)
    void push(const T& value) {
        data.push_back(value);
    }

    // 出栈：移除数组尾部元素 -> O(1)
    void pop() {
        if (empty()) {
            throw std::out_of_range("Stack is empty, cannot pop.");
        }
        data.pop_back();
    }

    // 获取栈顶：返回数组尾部元素 -> O(1)
    T& top() {
        if (empty()) {
            throw std::out_of_range("Stack is empty, cannot access top.");
        }
        return data.back();
    }

    // 判空
    bool empty() const {
        return data.empty();
    }

    // 获取大小
    size_t size() const {
        return data.size();
    }
};

// ==========================================
// 2. 基于链表的实现 (Linked List Stack)
// 自定义 Node 结构体，头插法入栈
// ==========================================
template <typename T>
class LinkedStack {
private:
    // 定义链表节点
    struct Node {
        T value;
        Node* next;
        Node(T v, Node* n = nullptr) : value(v), next(n) {}
    };

    Node* head; // 栈顶指针
    size_t count;

public:
    LinkedStack() : head(nullptr), count(0) {}

    // 析构函数：释放所有节点内存
    ~LinkedStack() {
        while (!empty()) {
            pop();
        }
    }

    // 入栈：头插法 -> O(1)
    // 新节点指向原来的 head，然后 head 更新为新节点
    void push(const T& value) {
        head = new Node(value, head);
        count++;
    }

    // 出栈：头删法 -> O(1)
    // 保存当前 head，head 移向 next，释放原 head
    void pop() {
        if (empty()) {
            throw std::out_of_range("Stack is empty, cannot pop.");
        }
        Node* temp = head;
        head = head->next;
        delete temp;
        count--;
    }

    // 获取栈顶 -> O(1)
    T& top() {
        if (empty()) {
            throw std::out_of_range("Stack is empty, cannot access top.");
        }
        return head->value;
    }

    bool empty() const {
        return head == nullptr;
    }

    size_t size() const {
        return count;
    }
};

// ==========================================
// 测试代码 (Test Case)
// ==========================================
int main() {
    // 1. 测试数组栈
    std::cout << "--- Testing ArrayStack ---" << std::endl;
    ArrayStack<int> aStack;
    aStack.push(10);
    aStack.push(20);
    aStack.push(30);

    std::cout << "Top element: " << aStack.top() << std::endl; // Should be 30
    aStack.pop();
    std::cout << "Top after pop: " << aStack.top() << std::endl; // Should be 20
    std::cout << "Size: " << aStack.size() << std::endl;         // Should be 2

    // 2. 测试链表栈
    std::cout << "\n--- Testing LinkedStack ---" << std::endl;
    LinkedStack<std::string> lStack;
    lStack.push("Hello");
    lStack.push("Data");
    lStack.push("Structure");

    std::cout << "Top element: " << lStack.top() << std::endl; // Should be Structure
    lStack.pop();
    std::cout << "Top after pop: " << lStack.top() << std::endl; // Should be Data
    
    // 演示异常处理
    // while (!lStack.empty()) lStack.pop();
    // try {
    //     lStack.pop(); 
    // } catch (const std::exception& e) {
    //     std::cout << "Exception caught: " << e.what() << std::endl;
    // }

    return 0;
}

```