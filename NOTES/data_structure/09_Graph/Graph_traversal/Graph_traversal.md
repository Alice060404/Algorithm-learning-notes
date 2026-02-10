[MindMap](./Graph_traversal.png)


# C++ 数据结构笔记：图的遍历 (Graph Traversal)

## 1. 核心概念 (Core Concepts)

图的遍历 (Graph Traversal) 是指从图中的某一顶点出发，按照某种搜索方法沿着图的边对图中的所有顶点访问一次且仅访问一次。

这是图论算法中最基础的操作，是求解连通性、最短路径、拓扑排序等复杂问题的基石。

### 1.1 存储结构前提

* **邻接表 (Adjacency List)**：由于遍历需要频繁获取某个顶点的“所有邻居”，邻接表的时间复杂度为 $O(Degree(V))$ ，优于邻接矩阵的 $O(V)$ 。因此，以下算法均基于邻接表实现。

## 2. 广度优先遍历 (BFS - Breadth-First Search)

### 2.1 算法思想

BFS 是一种“分层搜索”策略，类似于“水波纹扩散”或“剥洋葱”。它从起点开始，优先访问离起点最近的所有顶点，然后是次近的，以此类推。

### 2.2 核心数据结构：队列 (Queue)

* **为什么用队列？** 队列遵循 **FIFO (先进先出)** 原则。
* 当我们访问第一层节点时，将它们的邻居（第二层）加入队尾。
* 由于队列的特性，第一层节点处理完后，一定会接着处理第二层节点，从而保证了“由近及远”的顺序。

### 2.3 算法步骤

1. **初始化**：创建一个队列 `queue` 和一个记录访问状态的集合（或数组） `visited`。
2. **入队起点**：将起始顶点 `start` 加入队列，并标记为“已访问”。
3. **循环处理**：当队列不为空时：
* 从队首取出顶点 `u`。
* **访问/处理**顶点 `u`。
* 遍历 `u` 的所有邻接点 `v`：
* 如果 `v` **未被访问过**，将其加入队列，并立即标记为“已访问”。（注意：必须入队时立即标记，防止重复入队）。





### 2.4 复杂度分析

* **时间复杂度**: $O(V + E)$ 。每个顶点入队出队一次，每条边被扫描一次（无向图是两次）。
* **空间复杂度**: $O(V)$ 。主要消耗在辅助队列和 visited 数组上，最坏情况下（星型图）队列可能存储所有顶点。

---

## 3. 深度优先遍历 (DFS - Depth-First Search)

### 3.1 算法思想

DFS 是一种“不撞南墙不回头”的策略。从起点出发，选定一条路径一直走到底，直到无法继续（所有邻居都已访问），然后**回溯 (Backtrack)** 到上一个节点，换一个方向继续深入。

### 3.2 核心数据结构：栈 (Stack)

* **显式栈**：使用 `std::stack` 数据结构。
* **隐式栈 (递归)**：利用系统调用栈 (Call Stack) 实现递归调用，代码更简洁，是实现 DFS 的主流方式。
* **与树遍历的关系**：树的 **前序遍历 (Pre-order)**、**中序遍历 (In-order)**、**后序遍历 (Post-order)** 本质上都是 DFS 的特例。树是无环图，所以不需要 `visited` 数组（除了防止回访父节点），但普通图必须有 `visited` 记录以防止死循环。

### 3.3 算法步骤 (递归版)

1. **访问**：访问当前顶点 `u`，并标记为“已访问”。
2. **递归**：遍历 `u` 的所有邻接点 `v`：
* 如果 `v` **未被访问过**，则递归调用 `dfs(v)`。


3. **回溯**：当所有邻居都被访问过，函数返回（隐式回溯）。

### 3.4 复杂度分析

* **时间复杂度**: $O(V + E)$ 。
* **空间复杂度**: $O(V)$ 。取决于递归调用的深度，最坏情况下（单链图）深度为 V。

---

## 4. C++ 代码实现 (Implementation)

代码采用了 `vector<vector<int>>` 作为邻接表。
**关于 `visited` 数组的说明**：

* 思维导图中提到了“哈希集合 (`unordered_set`)”。
* **工程优化**：如果顶点是 0 到 N-1 的连续整数，使用 `vector<bool>` 或 `int visited[]` 数组的性能远好于哈希表（数组寻址是 $O(1)$ 且常数极小，无哈希冲突开销）。本代码采用数组方式。

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class Graph {
private:
    int numVertices; // 顶点数量
    vector<vector<int>> adj; // 邻接表：adj[i] 存储顶点 i 的所有邻居

    // DFS 的递归辅助函数
    void dfsRecursive(int v, vector<bool>& visited) {
        // 1. 标记当前节点已访问
        visited[v] = true;
        cout << v << " "; // 处理节点（此处仅打印）

        // 2. 遍历所有邻接点
        for (int neighbor : adj[v]) {
            if (!visited[neighbor]) {
                // 3. 递归访问未被访问的邻接点
                dfsRecursive(neighbor, visited);
            }
        }
    }

public:
    // 构造函数
    Graph(int v) : numVertices(v) {
        adj.resize(v);
    }

    // 添加边（无向图）
    void addEdge(int src, int dest) {
        adj[src].push_back(dest);
        adj[dest].push_back(src); 
    }

    // === 广度优先遍历 (BFS) ===
    void BFS(int startVertex) {
        if (startVertex >= numVertices) return;

        // 记录访问状态，避免死循环
        vector<bool> visited(numVertices, false);
        
        // 使用队列存储待访问的节点
        queue<int> q;

        // 起点入队并标记
        visited[startVertex] = true;
        q.push(startVertex);

        cout << "BFS Starting from " << startVertex << ": ";

        while (!q.empty()) {
            // 1. 取出队首元素
            int curr = q.front();
            q.pop();
            cout << curr << " ";

            // 2. 将所有未访问的邻居加入队列
            // 注意：通常建议在这里对邻居进行排序，以保证遍历顺序的确定性（非必须）
            // sort(adj[curr].begin(), adj[curr].end()); 
            
            for (int neighbor : adj[curr]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true; // 关键：入队时立即标记
                    q.push(neighbor);
                }
            }
        }
        cout << endl;
    }

    // === 深度优先遍历 (DFS) ===
    void DFS(int startVertex) {
        if (startVertex >= numVertices) return;

        vector<bool> visited(numVertices, false);
        cout << "DFS Starting from " << startVertex << ": ";
        
        // 调用递归辅助函数
        dfsRecursive(startVertex, visited);
        cout << endl;
    }
};

int main() {
    // 创建一个示例图
    // 结构如下：
    //      0
    //     / \
    //    1   2
    //   / \   \
    //  3   4   5
    
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);

    // 执行 BFS
    // 预期输出: 0 1 2 3 4 5 (顺序可能因邻接表存储顺序略有不同，但一定是分层的)
    g.BFS(0);

    // 执行 DFS
    // 预期输出: 0 1 3 4 2 5 (这也是前序遍历的顺序)
    g.DFS(0);

    return 0;
}

```

### 5. 总结与对比 (Summary)

| 特性 | BFS (广度优先) | DFS (深度优先) |
| --- | --- | --- |
| **数据结构** | 队列 (Queue) | 栈 (Stack) / 递归 |
| **路径特性** | 总是能找到**无权图**中的**最短路径** | 路径不一定是最短的 |
| **空间效率** | 较差 (需存储整层节点) | 较好 (仅需存储一条路径) |
| **适用场景** | 最短路径、层序遍历、社交网络六度分隔 | 迷宫寻路、连通性判断、拓扑排序 |
| **对应树遍历** | 层序遍历 (Level Order) | 前序/中序/后序遍历 |