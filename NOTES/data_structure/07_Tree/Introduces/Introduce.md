[MindMap](./introduces.png)


# 数据结构笔记：初识二叉树 (Introduction to Binary Trees)

## 1. 基本概念 (Basic Concepts)

二叉树（Binary Tree）是计算机科学中一种极其重要的数据结构。它是一种树形结构，特点是每个节点最多只有两个子节点，通常被称为“左子节点”和“右子节点”。

### 核心术语定义

* **根节点 (Root Node)**
* 树的最顶层节点。它是整棵树的唯一起点，没有父节点。
* *拓展*：在C++中，通常用一个指向根节点的指针来代表整棵树。


* **叶节点 (Leaf Node)**
* 没有子节点的节点。
* *判定条件*：`node->left == nullptr && node->right == nullptr`。


* **边 (Edge)**
* 连接两个节点的线段。
* *性质*：如果不包含环， 个节点的树有  条边。


* **节点所在的层 (Level)**
* 根节点为第1层（或第0层，取决于教材定义，通常算法题中视根为第1层居多），其子节点为第2层，以此类推。


* **节点的度 (Degree)**
* 该节点拥有的子节点数量。
* 对于二叉树，度的取值范围是 0, 1, 2。



### 高度与深度的区别 (易混淆点)

这两个概念在大一阶段非常容易混淆，请务必区分清楚：

* **节点的深度 (Depth of a node)**
* **定义**：从**根节点**到该节点的唯一路径上的边数（或节点数）。
* **方向**：从上往下看。根节点的深度通常定义为 0 或 1。


* **节点的高度 (Height of a node)**
* **定义**：从该节点到**最远叶节点**的最长路径上的边数（或节点数）。
* **方向**：从下往上看。叶节点的高度为 0（或 1）。


* **树的高度 (Height of the Tree)**
* **定义**：根节点的高度。也就是整棵树中最深节点的深度。



## 2. 常见二叉树类型 (Common Types)

根据节点的分布情况，二叉树有几种特殊的形态，它们在算法复杂度分析中非常关键。

### 完美二叉树 (Perfect Binary Tree)

* **定义**：除了叶节点之外的每一个节点都有两个子节点，且所有叶节点都在同一层上。
* **性质**：
* 层数为  的完美二叉树，节点总数为 。
* 这是最“丰满”的树。



### 完全二叉树 (Complete Binary Tree)

* **定义**：对一颗具有  个节点的二叉树按层序编号，如果编号为  () 的节点与同样深度的完美二叉树中编号为  的节点在二叉树中位置完全相同，则称为完全二叉树。
* **通俗理解**：除了最后一层外，其他各层节点数都达到最大值；最后一层的节点都连续集中在**最左边**。
* **应用**：二叉堆 (Binary Heap) 就是一种完全二叉树，常用于实现优先队列。

### 完满二叉树 (Full Binary Tree)

* **定义**：所有的节点要么没有子节点（叶子），要么有两个子节点。不存在只有一个子节点的节点。
* **特点**：节点的度只能是 0 或 2。

### 平衡二叉树 (Balanced Binary Tree)

* **定义**：任意节点的左右子树高度差的绝对值不超过 1。
* **重要性**：保证了树的高度维持在  级别，从而保证查找、插入、删除操作的高效性。常见的实现有 AVL 树和红黑树。

## 3. 二叉树退化 (Degeneration)

### 链表化

* **现象**：当二叉树的每个节点都只有左孩子（或都只有右孩子）时，树就退化成了一个**链表**。
* **影响**：
* **空间**：虽然存储结构没变，但利用率降低。
* **时间**：查找操作的时间复杂度从理想的  退化为 。
* 这也是为什么我们需要“平衡二叉树”的原因——为了防止退化。



---

## 4. C++ 代码示例

以下代码展示了二叉树节点的定义、创建、以及计算树的高度（递归思想的典型应用）。

```cpp
/**
 * * 包含二叉树的结构体定义、节点创建、以及基础属性计算（高度、节点数）。
 * 适合用于理解递归在树形结构中的应用。
 */

#include <iostream>
#include <algorithm> // for std::max

// 1. 定义二叉树节点结构体
struct TreeNode {
    int val;                // 节点存储的数据
    TreeNode* left;         // 指向左子节点的指针
    TreeNode* right;        // 指向右子节点的指针

    // 构造函数，方便初始化
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/**
 * 计算树的高度 (Height of Tree)
 * 定义：根节点到最远叶子节点的最长路径上的节点数。
 * * 思路 (递归):
 * 1. 终止条件：如果是空节点，高度为 0。
 * 2. 递推公式：当前节点高度 = max(左子树高度, 右子树高度) + 1。
 */
int getTreeHeight(TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    int leftHeight = getTreeHeight(root->left);
    int rightHeight = getTreeHeight(root->right);
    return std::max(leftHeight, rightHeight) + 1;
}

/**
 * 计算节点总数
 * 思路 (递归):
 * 1. 终止条件：空节点，数量为 0。
 * 2. 递推公式：总数 = 左子树节点数 + 右子树节点数 + 1 (自身)。
 */
int countNodes(TreeNode* root) {
    if (root == nullptr) {
        return 0;
    }
    return countNodes(root->left) + countNodes(root->right) + 1;
}

/**
 * 辅助函数：手动构建一个简单的二叉树用于测试
 * 结构如下：
 * 1
 * / \
 * 2   3
 * / \
 * 4   5
 */
void testBinaryTree() {
    // 创建节点
    TreeNode* root = new TreeNode(1);
    TreeNode* n2 = new TreeNode(2);
    TreeNode* n3 = new TreeNode(3);
    TreeNode* n4 = new TreeNode(4);
    TreeNode* n5 = new TreeNode(5);

    // 建立连接关系
    root->left = n2;
    root->right = n3;
    n2->left = n4;
    n2->right = n5;

    // 输出测试结果
    std::cout << "--- 二叉树基础测试 ---" << std::endl;
    std::cout << "根节点值: " << root->val << std::endl;
    
    // 计算属性
    int height = getTreeHeight(root);
    int count = countNodes(root);

    std::cout << "树的高度: " << height << std::endl; // 预期输出: 3
    std::cout << "节点总数: " << count << std::endl;  // 预期输出: 5

    // 内存清理 (在实际工程中通常使用析构函数或智能指针)
    delete n4;
    delete n5;
    delete n2;
    delete n3;
    delete root;
}

int main() {
    testBinaryTree();
    return 0;
}

```

### 学习建议 (For CS Freshman)

1. **手动模拟递归**：初学树的操作时，最难理解的是递归。建议拿纸笔画出栈的调用过程，理解 `getTreeHeight` 是如何一层层返回的。
2. **区分引用与指针**：在后续学习修改树结构（如插入节点）时，注意何时需要传递指针的引用 `TreeNode*&` 或二级指针 `TreeNode**`。
3. **极端情况**：写代码时永远要考虑 `root == nullptr` 的情况，这是面试中常见的扣分点。