[MindMap](./queue.png)


# 数据结构笔记：队列 (Queue)

## 1. 核心概念

队列（Queue）是一种受限的线性表，其核心特征是**先进先出**（FIFO, First In First Out）。

* **类比**：食堂排队打饭。先来的人先打饭（出队），后来的人只能排在队尾（入队）。
* **逻辑结构**：只允许在表的一端（**队尾 Rear**）进行插入，在另一端（**队头 Front**）进行删除。

## 2. 单向队列 (Single-ended Queue)

### 2.1 常规操作

时间复杂度均为 。

* **push(x) / Enqueue / 入队**：将元素 `x` 添加到队尾。
* **pop() / Dequeue / 出队**：移除队头元素。
* **front() / 获取队头**：查看队头元素的值。
* **back() / 获取队尾**：查看队尾元素的值。

### 2.2 实现方式与痛点分析

#### A. 基于链表实现 (Linked Queue)

* **原理**：使用单链表，维护两个指针 `head` (队头) 和 `tail` (队尾)。
* **入队**：`tail->next = newNode; tail = newNode;`
* **出队**：`Node* temp = head; head = head->next; delete temp;`


* **优点**：
* 不存在“队满”问题（除非内存耗尽）。
* 代码逻辑简单直接。



#### B. 基于数组实现 (Array Queue) —— 必须掌握“循环队列”

* **朴素实现的缺陷**：如果使用普通数组，队头指针 `front` 随着出队向后移动，会导致数组前面的空间被浪费，且无法再次利用，称为**“假溢出” (False Overflow)**。
* **解决方案：循环队列 (Circular Queue)**
* **原理**：将数组看作首尾相接的圆环。利用模运算 `%` 来计算下标。
* **关键公式**：
* 入队下标变化：`rear = (rear + 1) % capacity`
* 出队下标变化：`front = (front + 1) % capacity`
* **判空条件**：`front == rear`
* **判满条件**：`(rear + 1) % capacity == front` （为了区分空和满，通常会浪费一个数组单元）。





## 3. 双端队列 (Deque - Double Ended Queue)

双端队列打破了单向限制，允许在**两端**都进行插入和删除操作。它同时具备栈（Stack）和队列（Queue）的性质。

* **常规操作**：
* `push_front(x)` / `push_back(x)`：头插 / 尾插。
* `pop_front()` / `pop_back()`：头删 / 尾删。
* `front()` / `back()`：访问头 / 尾。


* **实现底层**：
* **链表**：必须使用**双向链表**，否则无法在  时间内完成尾部删除（因为需要知道倒数第二个节点）。
* **数组**：使用中心向两边扩展的动态数组（如 C++ STL 的 `std::deque` 采用分段连续数组 + 中控器 map 的复杂结构）。



## 4. 典型应用场景

1. **广度优先搜索 (BFS)**
* 图论中的层序遍历（如迷宫寻路、社交网络好友推荐）必须使用队列来记录当前层的节点，以便下一层访问。


2. **操作系统任务调度**
* CPU 时间片轮转调度：所有就绪进程排成一个队列，CPU 依次服务。
* 打印机缓冲区：打印任务按提交顺序依次执行。


3. **消息队列 (Message Queue)**
* 在高并发系统中（如秒杀活动），用于削峰填谷，生产者生成消息入队，消费者按顺序处理。


4. **滑动窗口算法 (Sliding Window)**
* **双端队列**的经典应用（如求解“滑动窗口最大值”），用于维护窗口内的单调性。



---

## 5. C++ 代码实现

这段代码包含两个部分：

1. **`CircularQueue`**：基于数组的**循环队列**实现（这是初学者最容易出错、也是最重要的部分）。
2. **`LinkedQueue`**：基于链表的队列实现。

代码均使用了模板，并处理了异常情况。

```cpp
/*
 * Queue Implementation in C++
 * Description: Implementation of Circular Queue (Array-based) and Linked Queue.
 */

#include <iostream>
#include <vector>
#include <stdexcept>

// ==========================================
// 1. 循环队列 (Circular Queue) - 基于数组
// 重点：解决数组实现的"假溢出"问题
// ==========================================
template <typename T>
class CircularQueue {
private:
    std::vector<T> data;
    int front;    // 指向队头元素
    int rear;     // 指向队尾元素的下一个位置（即下一个可写入的位置）
    int capacity; // 数组总容量

public:
    // 初始化时 capacity + 1，是因为循环队列需要空出一个位置来区分"空"和"满"
    CircularQueue(int k) : data(k + 1), front(0), rear(0), capacity(k + 1) {}

    // 入队操作 -> O(1)
    bool enqueue(const T& value) {
        if (isFull()) {
            return false; // 队列已满
        }
        data[rear] = value;
        // 使用模运算实现循环回绕
        rear = (rear + 1) % capacity;
        return true;
    }

    // 出队操作 -> O(1)
    bool dequeue() {
        if (isEmpty()) {
            return false; // 队列为空
        }
        // 只需要移动指针，不需要实际删除数据
        front = (front + 1) % capacity;
        return true;
    }

    // 获取队头 -> O(1)
    T getFront() {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data[front];
    }

    // 获取队尾 -> O(1)
    // 注意：rear 指向的是下一个插入位，所以队尾是 (rear - 1 + capacity) % capacity
    T getRear() {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return data[(rear - 1 + capacity) % capacity];
    }

    // 判空条件：front == rear
    bool isEmpty() {
        return front == rear;
    }

    // 判满条件：(rear + 1) % capacity == front
    bool isFull() {
        return (rear + 1) % capacity == front;
    }
};

// ==========================================
// 2. 链式队列 (Linked Queue) - 基于链表
// 重点：维护 head 和 tail 两个指针
// ==========================================
template <typename T>
class LinkedQueue {
private:
    struct Node {
        T value;
        Node* next;
        Node(T v) : value(v), next(nullptr) {}
    };

    Node* head; // 队头指针，负责出队
    Node* tail; // 队尾指针，负责入队

public:
    LinkedQueue() : head(nullptr), tail(nullptr) {}

    ~LinkedQueue() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // 入队：操作 tail -> O(1)
    void push(const T& value) {
        Node* newNode = new Node(value);
        if (tail == nullptr) {
            // 如果队列为空，head 和 tail 都指向新节点
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    // 出队：操作 head -> O(1)
    void pop() {
        if (head == nullptr) {
            return; // 队列为空
        }
        Node* temp = head;
        head = head->next;
        
        // 如果出队后队列变空，tail 也要置空
        if (head == nullptr) {
            tail = nullptr;
        }
        delete temp;
    }

    // 获取队头
    T front() {
        if (head == nullptr) {
            throw std::out_of_range("Queue is empty");
        }
        return head->value;
    }

    bool empty() const {
        return head == nullptr;
    }
};

// ==========================================
// 测试代码
// ==========================================
int main() {
    // 1. 测试循环队列
    std::cout << "--- Testing Circular Queue (Capacity 3) ---" << std::endl;
    CircularQueue<int> cq(3);
    
    cq.enqueue(10);
    cq.enqueue(20);
    cq.enqueue(30);
    
    if (!cq.enqueue(40)) {
        std::cout << "Queue is full, cannot enqueue 40" << std::endl;
    }

    std::cout << "Front: " << cq.getFront() << std::endl; // 10
    std::cout << "Rear: "  << cq.getRear() << std::endl;  // 30

    cq.dequeue(); // 移除 10
    cq.enqueue(40); // 现在应该能放进去了（因为腾出了空间）
    
    std::cout << "Front after dequeue/enqueue: " << cq.getFront() << std::endl; // 20
    std::cout << "Rear after dequeue/enqueue: "  << cq.getRear() << std::endl;  // 40

    // 2. 测试链式队列
    std::cout << "\n--- Testing Linked Queue ---" << std::endl;
    LinkedQueue<std::string> lq;
    lq.push("C++");
    lq.push("Java");
    lq.push("Python");

    std::cout << "Front: " << lq.front() << std::endl; // C++
    lq.pop();
    std::cout << "Front after pop: " << lq.front() << std::endl; // Java

    return 0;
}

```