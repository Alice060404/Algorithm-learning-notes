[MindMap](./Traverse_tree.png)


# C++ 数据结构笔记：二叉树遍历 (Binary Tree Traversal)

## 1. 概述

二叉树遍历是指按照某种特定的规则，依次访问二叉树中的所有节点，使得每个节点被访问且仅被访问一次。这是数据结构与算法中最基础也是最重要的操作之一。

遍历方式主要分为两大类：

1. **深度优先搜索 (DFS, Depth-First Search)**：沿着树的深度遍历树的节点，尽可能深的搜索树的分支。
2. **广度优先搜索 (BFS, Breadth-First Search)**：又称层序遍历，从根节点开始，一层一层地遍历。

---

## 2. 深度优先搜索 (DFS)

DFS 主要包括三种遍历顺序，它们的区别在于访问“根节点”的时机不同：

### 2.1 遍历顺序

* **前序遍历 (Pre-order Traversal)**
* 顺序：**根** -> 左 -> 右
* 应用：打印目录结构、快速复制二叉树。


* **中序遍历 (In-order Traversal)**
* 顺序：左 -> **根** -> 右
* 特性：对于**二叉搜索树 (BST)**，中序遍历可以得到有序的数列。


* **后序遍历 (Post-order Traversal)**
* 顺序：左 -> 右 -> **根**
* 应用：目录容量计算（先计算子目录再汇总）、二叉树的销毁（先删子节点再删父节点）。



### 2.2 实现方法

* **递归法 (Recursion)**：代码最简洁，利用系统调用栈（Call Stack）自动处理回溯。
* **迭代法 (Iteration)**：手动维护一个显式的栈 (`std::stack`) 来模拟递归过程。这在防止栈溢出或需要精细控制遍历流程时非常有用。

---

## 3. 层序遍历 (BFS)

### 3.1 原理

层序遍历从根节点开始，按照从上到下、从左到右的顺序逐层访问。

### 3.2 实现细节

* **核心数据结构**：队列 (`std::queue`)。利用队列“先进先出” (FIFO) 的特性，保证上一层的节点先被处理，其子节点随后入队排在后面。
* **复杂度分析**：
* **时间复杂度**：。每个节点进队、出队各一次。
* **空间复杂度**：（或更准确地说是 ，其中  是树的最大宽度）。最坏情况下（完全二叉树），最后一层节点数约为 。



---

## 4. C++ 代码实现 (Copy-Paste Ready)

以下代码包含一个完整的 C++ 源文件。它定义了二叉树节点结构，并实现了上述提到的所有遍历方法（递归 DFS、迭代 DFS、层序遍历 BFS）。

**文件建议命名：** `binary_tree_traversal.cpp`

```cpp
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

// 定义二叉树节点结构体
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class BinaryTreeTraversal {
public:
    // ==========================================
    // 1. 递归版本 (Recursive DFS)
    // ==========================================

    // 前序遍历 (递归)
    void preOrderRecursive(TreeNode* root, std::vector<int>& result) {
        if (root == nullptr) return;
        result.push_back(root->val);        // 根
        preOrderRecursive(root->left, result);  // 左
        preOrderRecursive(root->right, result); // 右
    }

    // 中序遍历 (递归)
    void inOrderRecursive(TreeNode* root, std::vector<int>& result) {
        if (root == nullptr) return;
        inOrderRecursive(root->left, result);   // 左
        result.push_back(root->val);        // 根
        inOrderRecursive(root->right, result);  // 右
    }

    // 后序遍历 (递归)
    void postOrderRecursive(TreeNode* root, std::vector<int>& result) {
        if (root == nullptr) return;
        postOrderRecursive(root->left, result); // 左
        postOrderRecursive(root->right, result); // 右
        result.push_back(root->val);        // 根
    }

    // ==========================================
    // 2. 迭代版本 (Iterative DFS) - 使用 Stack
    // ==========================================

    // 前序遍历 (迭代): 根 -> 左 -> 右
    // 技巧：栈是后进先出，所以先压入右子节点，再压入左子节点
    std::vector<int> preOrderIterative(TreeNode* root) {
        std::vector<int> result;
        if (root == nullptr) return result;

        std::stack<TreeNode*> st;
        st.push(root);

        while (!st.empty()) {
            TreeNode* node = st.top();
            st.pop();
            result.push_back(node->val);

            // 先右后左
            if (node->right) st.push(node->right);
            if (node->left) st.push(node->left);
        }
        return result;
    }

    // 中序遍历 (迭代): 左 -> 根 -> 右
    // 技巧：一路向左走到尽头，入栈；无法再走时，出栈访问，转向右子节点
    std::vector<int> inOrderIterative(TreeNode* root) {
        std::vector<int> result;
        std::stack<TreeNode*> st;
        TreeNode* curr = root;

        while (curr != nullptr || !st.empty()) {
            // 不断深入左子树
            while (curr != nullptr) {
                st.push(curr);
                curr = curr->left;
            }
            // 此时 curr 为空，栈顶是左子树最深处的节点
            curr = st.top();
            st.pop();
            result.push_back(curr->val);
            // 转向右子树
            curr = curr->right;
        }
        return result;
    }

    // 后序遍历 (迭代): 左 -> 右 -> 根
    // 技巧：这是一个难点。可以使用 "prev" 指针记录上一次访问的节点，
    // 判断是从左子树返回还是从右子树返回。
    std::vector<int> postOrderIterative(TreeNode* root) {
        std::vector<int> result;
        if (root == nullptr) return result;

        std::stack<TreeNode*> st;
        TreeNode* curr = root;
        TreeNode* prev = nullptr; // 记录上一次访问并输出的节点

        while (curr != nullptr || !st.empty()) {
            while (curr != nullptr) {
                st.push(curr);
                curr = curr->left;
            }
            
            curr = st.top();
            
            // 如果右子树存在，且尚未被处理（prev != curr->right）
            if (curr->right != nullptr && curr->right != prev) {
                curr = curr->right; // 进入右子树
            } else {
                // 右子树为空，或者右子树已经处理过了
                st.pop();
                result.push_back(curr->val);
                prev = curr; // 更新 prev
                curr = nullptr; // 重置 curr 以防再次进入上面的 while 循环
            }
        }
        return result;
    }

    // ==========================================
    // 3. 层序遍历 (BFS) - 使用 Queue
    // ==========================================
    
    std::vector<std::vector<int>> levelOrder(TreeNode* root) {
        std::vector<std::vector<int>> result;
        if (root == nullptr) return result;

        std::queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {
            int levelSize = q.size(); // 当前层的节点数量
            std::vector<int> currentLevel;

            for (int i = 0; i < levelSize; ++i) {
                TreeNode* node = q.front();
                q.pop();
                currentLevel.push_back(node->val);

                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            result.push_back(currentLevel);
        }
        return result;
    }
};

// 辅助函数：打印 vector
void printVector(const std::vector<int>& v, const std::string& name) {
    std::cout << name << ": [ ";
    for (int num : v) std::cout << num << " ";
    std::cout << "]" << std::endl;
}

int main() {
    // 手动构建一个简单的二叉树用于测试
    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5
    
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);

    BinaryTreeTraversal solver;
    std::vector<int> result;

    std::cout << "=== 递归 DFS 测试 ===" << std::endl;
    result.clear();
    solver.preOrderRecursive(root, result);
    printVector(result, "前序 (递归)");

    result.clear();
    solver.inOrderRecursive(root, result);
    printVector(result, "中序 (递归)");

    result.clear();
    solver.postOrderRecursive(root, result);
    printVector(result, "后序 (递归)");

    std::cout << "\n=== 迭代 DFS 测试 ===" << std::endl;
    printVector(solver.preOrderIterative(root), "前序 (迭代)");
    printVector(solver.inOrderIterative(root), "中序 (迭代)");
    printVector(solver.postOrderIterative(root), "后序 (迭代)");

    std::cout << "\n=== 层序遍历 BFS 测试 ===" << std::endl;
    std::vector<std::vector<int>> levels = solver.levelOrder(root);
    std::cout << "层序遍历:" << std::endl;
    for (size_t i = 0; i < levels.size(); ++i) {
        std::cout << "Level " << i << ": ";
        for (int num : levels[i]) std::cout << num << " ";
        std::cout << std::endl;
    }

    // 实际项目中记得释放内存，这里为演示简略跳过
    return 0;
}

```