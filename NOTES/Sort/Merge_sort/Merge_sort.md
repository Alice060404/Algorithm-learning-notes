[MindMap](./Merge_sort.png)


# 归并排序 (Merge Sort) 学习笔记

归并排序是基于**分治思想 (Divide and Conquer)** 的一种经典排序算法。它的核心逻辑是将一个大问题分解为小问题，解决小问题后，再将结果合并以解决大问题。

## 一、 算法原理

归并排序的执行过程严格遵循分治策略，主要分为两个关键阶段：

### 1. 划分阶段 (Divide)

* **概念**：将当前数组（或链表）从中间一分为二，直到子数组的长度为1。
* **细节**：长度为1的数组可以天然被认为是有序的。划分的过程实际上是一个递归建立递归树的过程，每次计算中点 `mid = left + (right - left) / 2`，防止索引越界溢出。

### 2. 合并阶段 (Merge / Conquer)

* **概念**：将两个已经有序的子数组（或子链表）合并成一个更大的有序数组。
* **细节**：这是归并排序的核心。需要借助双指针（分别指向两个子序列的起始位置），比较两个指针所指元素的大小，将较小的元素依次放入一个临时容器中。当一个子序列遍历完毕后，将另一个子序列的剩余部分直接追加到临时容器的末尾。最后，将临时容器中的有序数据拷贝回原数组。

## 二、 算法特点与性能分析

* **时间复杂度：*$O(n \log n)$*
* 无论输入数据是随机的、正序的还是逆序的，归并排序的划分过程都会将递归树建立到 $\log n$ 层。而在每一层中，合并操作都需要遍历所有的 $n$ 个元素。因此，最好、最坏、平均时间复杂度均严格为 $O(n \log n)$ 。


* **空间复杂度**
* **数组排序**：由于合并阶段需要一个与原数组大小相同的临时数组（辅助空间）来暂存合并后的数据，空间复杂度为 $O(n)$ 。
* **链表排序**：合并链表不需要额外的节点空间，只需要常数级别的指针，但递归调用会消耗栈空间，空间复杂度为 $O(\log n)$。


* **非原地排序 (Non-in-place)**
* 针对数组而言，因为依赖 $O(n)$ 的额外辅助空间，所以它不是原地排序算法。


* **非自适应排序 (Non-adaptive)**
* 即便输入数组已经是完全有序的，归并排序依然会机械地进行一分为二的划分和随后的合并，执行时间不会因为初始状态的有序而显著减少。


* **稳定排序 (Stable)**
* 在合并两个有序子序列时，如果遇到两个相等的元素，优先将**左边子序列**中的元素放入临时数组。这保证了相等元素的相对顺序不会发生改变，因此归并排序是稳定的。



## 三、 C++ 代码实现


### 1. 数组排序实现

在数组实现中，为了避免在每次递归调用中频繁地动态申请和释放内存（这会导致巨大的性能开销并可能引发内存碎片），最佳实践是在外部一次性分配好一个同等大小的临时数组 `temp`，并在整个递归过程中复用它。

```cpp
#include <iostream>
#include <vector>

class MergeSorter {
private:
    // 合并两个有序子数组的函数
    void merge(std::vector<int>& arr, int left, int mid, int right, std::vector<int>& temp) {
        int i = left;      // 左半区起始指针
        int j = mid + 1;   // 右半区起始指针
        int t = 0;         // 临时数组索引

        // 步骤1：比较左右两部分，将较小的元素按顺序填入临时数组
        while (i <= mid && j <= right) {
            // 注意这里的 <= 是归并排序成为稳定排序的关键
            if (arr[i] <= arr[j]) {
                temp[t++] = arr[i++];
            } else {
                temp[t++] = arr[j++];
            }
        }

        // 步骤2：处理左半区剩余元素（如果右半区先遍历完）
        while (i <= mid) {
            temp[t++] = arr[i++];
        }

        // 步骤3：处理右半区剩余元素（如果左半区先遍历完）
        while (j <= right) {
            temp[t++] = arr[j++];
        }

        // 步骤4：将临时数组中的有序元素拷贝回原数组的对应位置
        t = 0;
        while (left <= right) {
            arr[left++] = temp[t++];
        }
    }

    // 递归划分函数
    void mergeSortRecursive(std::vector<int>& arr, int left, int right, std::vector<int>& temp) {
        // 递归终止条件：子数组长度为1
        if (left >= right) return;

        // 计算中点，避免 (left + right) 直接相加导致 int 溢出
        int mid = left + (right - left) / 2;

        // 划分阶段
        mergeSortRecursive(arr, left, mid, temp);      // 处理左半区
        mergeSortRecursive(arr, mid + 1, right, temp); // 处理右半区

        // 合并阶段
        merge(arr, left, mid, right, temp);
    }

public:
    // 供外部调用的公有接口
    void sort(std::vector<int>& arr) {
        if (arr.empty()) return;
        // 提前分配好临时数组内存，避免递归深处频繁申请内存的开销
        std::vector<int> temp(arr.size());
        mergeSortRecursive(arr, 0, arr.size() - 1, temp);
    }
};

// 测试代码
int main() {
    std::vector<int> arr = {38, 27, 43, 3, 9, 82, 10};
    MergeSorter sorter;
    sorter.sort(arr);
    
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    return 0;
}

```

### 2. 单链表排序实现

在链表中实现归并排序是非常经典的面试题。难点在于：

1. 无法像数组那样直接通过索引 `(left + right) / 2` 找到中点。解决思路是使用**快慢指针 (Fast/Slow Pointers)**。
2. 合并时为了简化对头节点的特殊判断，可以使用**虚拟头节点 (Dummy Head)** 技巧。

```cpp
#include <iostream>

// 单链表节点结构体定义
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class LinkedListMergeSorter {
private:
    // 合并两个有序单链表
    ListNode* merge(ListNode* l1, ListNode* l2) {
        ListNode dummy(0); // 虚拟头节点，作为合并后链表的锚点
        ListNode* current = &dummy;
        
        while (l1 != nullptr && l2 != nullptr) {
            if (l1->val <= l2->val) { // <= 保证稳定性
                current->next = l1;
                l1 = l1->next;
            } else {
                current->next = l2;
                l2 = l2->next;
            }
            current = current->next;
        }
        
        // 将剩余的非空链表直接拼接在末尾
        if (l1 != nullptr) current->next = l1;
        if (l2 != nullptr) current->next = l2;
        
        return dummy.next;
    }

public:
    // 链表归并排序主函数
    ListNode* sortList(ListNode* head) {
        // 递归终止条件：链表为空或只有一个节点
        if (head == nullptr || head->next == nullptr) {
            return head;
        }
        
        // 1. 划分阶段：利用快慢指针寻找链表中点
        ListNode* slow = head;
        ListNode* fast = head->next; // 注意这里fast比slow多走一步，确保偶数个节点时slow停在前半部分末尾
        
        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        ListNode* mid = slow->next;
        slow->next = nullptr; // 切断链表，分成左右两半
        
        // 递归对左右两半部分进行排序
        ListNode* left = sortList(head);
        ListNode* right = sortList(mid);
        
        // 2. 合并阶段
        return merge(left, right);
    }
};

```