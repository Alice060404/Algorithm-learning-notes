[MindMap](./Linked_list.png)


# 链表 (Linked List) 学习笔记

## 1. 核心概念与定义

链表是一种物理存储单元上**非连续、非顺序**的存储结构。数据元素的逻辑顺序是通过链表中的**指针**链接次序实现的。

* **节点 (Node)**：链表的基本组成单位。每个节点通常包含两部分：
* **数据域 (Data)**：存储数据元素。
* **指针域 (Next/Prev)**：存储指向下一个（或上一个）节点的地址。


* **头节点 (Head)**：链表的第一个节点，通常作为整个链表的入口。

## 2. 常见操作详解

### 2.1 创建与初始化

不同于数组在声明时需要确定大小，链表是**动态分配**内存的。初始化通常意味着创建一个空指针或者一个只包含头结点的链表。

### 2.2 插入节点 (Insertion)

* **头部插入**：时间复杂度 `O(1)` , 只需修改新节点的 `next` 指向原头节点，更新头指针。
* **尾部插入**：时间复杂度  `O(n)`（若无尾指针）。需要遍历整个链表找到最后一个节点。
* **中间插入**：时间复杂度  `O(n)`（查找位置）+ （插入操作）。

### 2.3 删除节点 (Deletion)

* **常规思路**：找到目标节点的前驱节点（Pre-node），将其 `next` 指向目标节点的后继节点，释放目标节点内存。
* **技巧：栈上虚拟头节点 (Dummy Head)**
* **痛点**：通常删除第一个节点需要特殊处理（因为它没有前驱节点），导致代码逻辑分叉。
* **解决方案**：构建一个虚拟的头节点（Dummy Head）指向真实的 `head`。这样所有节点（包括原来的头节点）都有了前驱节点，统一了逻辑。
* **栈上分配**：在函数栈帧中直接声明 `ListNode dummy` 而非 `new ListNode`，利用RAII机制自动回收，避免内存泄漏，且无需手动 `delete` 虚拟节点。



### 2.4 访问与查找

* **访问/查找**：链表不支持随机访问（Random Access）。必须从头节点开始逐个遍历，时间复杂度为 `O(n)`。

## 3. 链表 vs 数组 (Linked List vs Array)

| 特性 | 数组 (Array) | 链表 (Linked List) |
| --- | --- | --- |
| **内存布局** | 连续内存空间 | 离散内存空间，通过指针连接 |
| **大小调整** | 固定（静态数组）或 扩容成本高（动态数组） | 动态扩展，无固定上限（受限于内存） |
| **访问效率** |  `O(1)`，支持下标随机访问 |  `O(n)`，必须顺序遍历 |
| **插入/删除** |  `O(n)`，因为需要移动后续元素 |  `O(1)`，仅需修改指针（前提是已找到位置） |
| **空间利用** | 较高，无额外开销 | 较低，每个数据需额外存储指针 |

## 4. 类型与应用场景

### 4.1 单向链表 (Singly Linked List)

节点仅包含指向后继的指针。

* **栈与队列 (Stack & Queue)**：利用链表头部/尾部插入删除  的特性实现。
* **哈希表 (Hash Table)**：解决哈希冲突（拉链法/链地址法）。
* **图 (Graph)**：邻接表（Adjacency List）的底层实现。

### 4.2 双向链表 (Doubly Linked List)

节点包含 `prev` 和 `next` 指针，可双向遍历。

* **高级数据结构**：红黑树、B树的某些实现细节。
* **浏览器历史**：前进（Next）和后退（Prev）功能。
* **LRU缓存算法 (Least Recently Used)**：利用双向链表实现  的节点移动和删除。

### 4.3 环形链表 (Circular Linked List)

尾节点指向头节点，形成闭环。

* **时间片轮转调度 (Round Robin Scheduling)**：操作系统进程调度。
* **数据缓冲区**：如音频/视频流的环形缓冲。

---

## C++ 代码示例 (Code Implementation)

本示例包含单向链表的定义、基于**栈上虚拟头节点**的删除操作、插入操作及遍历打印。代码已针对C++初学者优化，注重内存管理与代码规范。

```cpp
/*
 * 链表基础操作演示 (C++ Version)
 * Description: 包含结构体定义、创建、插入、查找、删除(虚拟头节点技巧)
 */

#include <iostream>

// 1. 节点定义
struct ListNode {
    int val;            // 数据域
    ListNode* next;     // 指针域

    // 构造函数：方便初始化
    ListNode(int x) : val(x), next(nullptr) {}
};

class LinkedList {
private:
    ListNode* head; // 头指针

public:
    LinkedList() : head(nullptr) {}

    // 析构函数：释放所有节点内存，防止内存泄漏
    ~LinkedList() {
        ListNode* current = head;
        while (current != nullptr) {
            ListNode* temp = current;
            current = current->next;
            delete temp;
        }
        std::cout << "Memory released." << std::endl;
    }

    // 2. 头部插入节点 (O(1))
    void insertAtHead(int val) {
        ListNode* newNode = new ListNode(val);
        newNode->next = head;
        head = newNode;
    }

    // 3. 尾部插入节点 (O(n))
    void insertAtTail(int val) {
        ListNode* newNode = new ListNode(val);
        if (head == nullptr) {
            head = newNode;
            return;
        }
        ListNode* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }

    // 4. 删除指定值的节点 (使用栈上虚拟头节点技巧)
    void deleteValue(int val) {
        // 技巧：创建一个在栈上的虚拟头节点 (Dummy Head)
        // 这样可以避免单独处理“删除头节点”的特殊情况
        ListNode dummy(0); 
        dummy.next = head; 

        ListNode* current = &dummy;

        while (current->next != nullptr) {
            if (current->next->val == val) {
                ListNode* temp = current->next;
                current->next = current->next->next; // 跨过待删除节点
                delete temp; // 释放内存
                // 注意：这里删除了一个节点后，是否继续删除重复值取决于需求
                // 如果只删第一个匹配项，这里可以 return
            } else {
                current = current->next;
            }
        }

        // 更新真正的 head 指针 (因为 head 可能被删除了)
        head = dummy.next;
    }

    // 5. 打印链表
    void printList() {
        ListNode* current = head;
        std::cout << "List: ";
        while (current != nullptr) {
            std::cout << current->val << " -> ";
            current = current->next;
        }
        std::cout << "NULL" << std::endl;
    }
};

int main() {
    LinkedList list;

    // 测试插入
    list.insertAtTail(10);
    list.insertAtTail(20);
    list.insertAtHead(5);
    list.insertAtTail(30);
    list.insertAtTail(20); // 重复值测试

    std::cout << "Initial ";
    list.printList(); // 5 -> 10 -> 20 -> 30 -> 20 -> NULL

    // 测试删除 (删除所有值为20的节点)
    std::cout << "Deleting 20..." << std::endl;
    list.deleteValue(20);
    list.printList(); // 5 -> 10 -> 30 -> NULL

    // 测试删除头节点
    std::cout << "Deleting 5 (Head)..." << std::endl;
    list.deleteValue(5);
    list.printList(); // 10 -> 30 -> NULL

    return 0;
}

```

### 代码笔记要点 (Key Takeaways)

1. **构造函数**：在 `struct` 中使用构造函数 `ListNode(int x)` 可以极大地简化节点创建代码（`new ListNode(10)` vs `node->val=10; node->next=nullptr`）。
2. **虚拟头节点 (Dummy Node)**：
* 在 `deleteValue` 函数中，我们定义了 `ListNode dummy(0)`。这是一个**栈变量**。
* 它的 `next` 指向当前的 `head`。
* 无论真正的 `head` 是否被删除，`dummy.next` 永远指向操作后的新链表头。
* **优势**：消除了 `if (head->val == val)` 这种针对头节点的特殊判断逻辑，使代码更通用、简洁。


3. **内存管理**：链表操作涉及大量的 `new`，在 C++ 中务必记得对应的 `delete`。示例中的析构函数 `~LinkedList()` 演示了如何遍历释放整个链表。