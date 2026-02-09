/*
 * 项目：C++ 数据结构笔记 - 堆 (Heap) 实现与应用
 * 日期：2026-02-09
 * 描述：
 * 1. 手动实现 MaxHeap 类，包含 O(n) 建堆优化。
 * 2. 演示 std::priority_queue 的基本用法。
 * 3. 演示利用堆进行 Top-K 问题求解思路。
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <queue>      // 用于演示 STL 优先队列
#include <functional> // 用于 std::greater

// ==========================================
// 第一部分：手动实现大顶堆 (Max Heap)
// ==========================================
class MyMaxHeap
{
private:
    std::vector<int> vec;

    // 索引计算辅助函数
    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }

    // 上滤操作：用于插入元素
    // 时间复杂度：O(log n)
    void siftUp(int i)
    {
        while (i > 0)
        {
            int p = parent(i);
            if (vec[p] >= vec[i])
                break; // 满足堆性质：父 >= 子
            std::swap(vec[i], vec[p]);
            i = p;
        }
    }

    // 下滤操作：用于删除堆顶或建堆
    // 时间复杂度：O(log n)
    void siftDown(int i)
    {
        int n = vec.size();
        while (true)
        {
            int l = left(i);
            int r = right(i);
            int maxIndex = i;

            if (l < n && vec[l] > vec[maxIndex])
                maxIndex = l;
            if (r < n && vec[r] > vec[maxIndex])
                maxIndex = r;

            if (maxIndex == i)
                break; // 满足堆性质

            std::swap(vec[i], vec[maxIndex]);
            i = maxIndex;
        }
    }

public:
    // 默认构造
    MyMaxHeap() = default;

    // 优化构造：O(n) 建堆
    // 接收无序数组，直接转化为堆
    MyMaxHeap(const std::vector<int> &items) : vec(items)
    {
        // 从最后一个非叶子节点开始遍历
        for (int i = parent(vec.size() - 1); i >= 0; --i)
        {
            siftDown(i);
        }
    }

    // 入堆
    void push(int val)
    {
        vec.push_back(val);
        siftUp(vec.size() - 1);
    }

    // 访问堆顶
    int top() const
    {
        if (empty())
            throw std::out_of_range("Heap is empty");
        return vec[0];
    }

    // 出堆
    void pop()
    {
        if (empty())
            throw std::out_of_range("Heap is empty");
        std::swap(vec[0], vec[vec.size() - 1]);
        vec.pop_back();
        if (!empty())
            siftDown(0);
    }

    bool empty() const { return vec.empty(); }
    int size() const { return vec.size(); }
};

// ==========================================
// 第二部分：功能测试与应用演示
// ==========================================

// 辅助函数：寻找数组中前 K 个最大的元素 (Top-K 问题)
// 策略：使用小顶堆维护当前最大的 K 个值
void findTopK(const std::vector<int> &data, int k)
{
    if (k <= 0 || k > data.size())
        return;

    // 使用 STL 的优先队列实现小顶堆
    // 参数含义：数据类型 int，底层容器 vector，比较方式 greater(小的优先级高)
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    for (int x : data)
    {
        if (minHeap.size() < k)
        {
            minHeap.push(x);
        }
        else
        {
            // 如果新元素比堆顶(目前前K大里最小的)还要大，替换它
            if (x > minHeap.top())
            {
                minHeap.pop();
                minHeap.push(x);
            }
        }
    }

    std::cout << "数组中最大的 " << k << " 个元素为: ";
    // 注意：打印出来是递增顺序，因为是从小顶堆依次弹出的
    while (!minHeap.empty())
    {
        std::cout << minHeap.top() << " ";
        minHeap.pop();
    }
    std::cout << std::endl;
}

int main()
{
    try
    {
        std::cout << "=== 1. 手动实现大顶堆测试 ===" << std::endl;
        std::vector<int> rawData = {3, 1, 4, 1, 5, 9, 2, 6};
        MyMaxHeap myHeap(rawData); // 测试 O(n) 建堆

        std::cout << "建堆后堆顶元素: " << myHeap.top() << " (Expected: 9)" << std::endl;

        myHeap.push(10);
        std::cout << "Push(10) 后堆顶元素: " << myHeap.top() << " (Expected: 10)" << std::endl;

        std::cout << "依次出堆 (堆排序逻辑): ";
        while (!myHeap.empty())
        {
            std::cout << myHeap.top() << " ";
            myHeap.pop();
        }
        std::cout << std::endl;

        std::cout << "\n=== 2. STL priority_queue 演示 ===" << std::endl;
        std::priority_queue<int> stlMaxHeap; // 默认大顶堆
        for (int x : rawData)
            stlMaxHeap.push(x);
        std::cout << "STL 堆顶: " << stlMaxHeap.top() << std::endl;

        std::cout << "\n=== 3. Top-K 问题实战 ===" << std::endl;
        // 寻找 rawData 中最大的 3 个数
        findTopK(rawData, 3);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}