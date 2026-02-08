[MindMap](./Binary_search_tree.png)


# 数据结构笔记：二叉搜索树 (Binary Search Tree, BST)

## 1. 定义 (Definition)

二叉搜索树（又称二叉查找树、二叉排序树）是一种特殊的二叉树，它不仅满足二叉树的结构特征，还满足以下严格的**有序性性质**：

1. **节点约束**：对于树中的任意节点：
* 其**左子树**中所有节点的值都**小于**该节点的值。
* 其**右子树**中所有节点的值都**大于**该节点的值。


2. **递归定义**：任意节点的左、右子树本身也必须分别是二叉搜索树。
3. **无重复值**：通常情况下，BST不包含重复值的节点（具体的实现可以约定左子树包含等于根节点的值，但标准定义一般假设键值唯一）。

> **核心特性**：正是由于这种有序性，对二叉搜索树进行**中序遍历 (In-order Traversal)**，可以得到一个递增的有序序列。

## 2. 核心操作 (Operations)

### 2.1 查找元素 (Search)

* **逻辑**：从根节点开始。如果目标值等于当前节点值，查找成功；如果目标值小于当前节点值，向左子树递归；如果目标值大于当前节点值，向右子树递归。
* **类比**：类似于数组的二分查找 (Binary Search)。

### 2.2 插入元素 (Insert)

* **逻辑**：新节点总是作为叶子节点插入。
* **过程**：从根节点开始搜索，找到新节点应该存在的位置（左孩子为空或右孩子为空），将其链接到该位置。不允许插入已存在的键值。

### 2.3 遍历 (Traversal)

* **中序遍历 (In-order)**：左 -> 根 -> 右。结果是**有序数组**。这是BST最重要的遍历方式。
* **前序遍历/后序遍历**：用于序列化树或释放内存。

### 2.4 删除元素 (Delete) - *难点*

思维导图中虽未详细展开，但这是考试和面试的重点。删除操作分三种情况：

1. **删除叶子节点**：直接删除，将其父节点的对应指针置为 `nullptr`。
2. **删除只有一个子节点的节点**：将其子节点提升，替换被删除节点的位置。
3. **删除有两个子节点的节点**：
* 找到该节点**右子树中的最小值**（后继节点）或者**左子树中的最大值**（前驱节点）。
* 用该值替换被删除节点的值。
* 递归地删除那个后继或前驱节点（此时问题转化为情况1或2）。



## 3. 效率分析 (Efficiency)

### 3.1 时间复杂度

BST 的效率取决于树的**高度 (Height)**，记为 。

| 操作 | 平均情况 (随机插入) | 最坏情况 (有序插入 - 退化为链表) |
| --- | --- | --- |
| **查找** | `O(log n)` | `O(n)` |
| **插入** | `O(log n)` | `O(n)` |
| **删除** | `O(log n)` | `O(n)` |

* **对比数组表示的二叉树**：
* 普通有序数组：查找 `O(log n)`，但插入和删除需要移动大量元素，为 `O(n)`。
* 链表：插入删除 `O(1)` (前提是已知位置)，但查找是 `O(n)`。
* BST 平衡了查找和增删的效率。



### 3.2 空间复杂度

* `O(n)`，用于存储  个节点。

## 4. 常见应用 (Applications)

1. **多级索引**：在操作系统或文件系统中，利用BST（或其变种如B树、B+树）实现高效的文件查找、插入和删除。
2. **底层数据结构**：
* 许多高级搜索算法的基础。
* 在C++ STL中，`std::set` 和 `std::map` 通常是用 **红黑树 (Red-Black Tree)** 实现的，而红黑树就是一种自平衡的二叉搜索树。


3. **数据流管理**：用于动态维护一个有序序列，随时可以输出有序数据或查找特定排名的元素。

---

## 5. C++ 代码实现 (Implementation)

以下是一个标准的 C++ 实现，包含了插入、查找、中序遍历和销毁操作。使用了 `struct` 定义节点，`class` 封装操作。

```cpp
#include <iostream>

// 定义树节点结构
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    TreeNode* root;

    // 递归插入辅助函数
    TreeNode* insertRec(TreeNode* node, int key) {
        // 如果树为空，返回新节点
        if (node == nullptr) {
            return new TreeNode(key);
        }

        // 递归向下遍历树
        if (key < node->val) {
            node->left = insertRec(node->left, key);
        } else if (key > node->val) {
            node->right = insertRec(node->right, key);
        }
        // 如果 key == node->val，不进行插入（假设不允许重复）

        return node;
    }

    // 递归中序遍历辅助函数
    void inorderRec(TreeNode* node) {
        if (node != nullptr) {
            inorderRec(node->left);
            std::cout << node->val << " ";
            inorderRec(node->right);
        }
    }

    // 递归查找辅助函数
    bool searchRec(TreeNode* node, int key) {
        if (node == nullptr) return false;
        if (node->val == key) return true;

        if (key < node->val)
            return searchRec(node->left, key);
        else
            return searchRec(node->right, key);
    }

    // 辅助函数：删除整棵树，释放内存
    void destroyTree(TreeNode* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    // 辅助函数：查找最小值节点（用于删除操作）
    TreeNode* minValueNode(TreeNode* node) {
        TreeNode* current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }

    // 递归删除辅助函数
    TreeNode* deleteRec(TreeNode* root, int key) {
        if (root == nullptr) return root;

        // 1. 寻找节点
        if (key < root->val) {
            root->left = deleteRec(root->left, key);
        } else if (key > root->val) {
            root->right = deleteRec(root->right, key);
        } else {
            // 2. 找到节点，执行删除
            
            // 情况 A: 只有一个子节点或无子节点
            if (root->left == nullptr) {
                TreeNode* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                TreeNode* temp = root->left;
                delete root;
                return temp;
            }

            // 情况 B: 有两个子节点
            // 获取右子树中的最小值（后继节点）
            TreeNode* temp = minValueNode(root->right);

            // 将后继节点的值复制到当前节点
            root->val = temp->val;

            // 删除那个后继节点
            root->right = deleteRec(root->right, temp->val);
        }
        return root;
    }

public:
    BinarySearchTree() : root(nullptr) {}

    // 析构函数
    ~BinarySearchTree() {
        destroyTree(root);
    }

    // 插入接口
    void insert(int key) {
        root = insertRec(root, key);
    }

    // 查找接口
    bool search(int key) {
        return searchRec(root, key);
    }

    // 删除接口
    void remove(int key) {
        root = deleteRec(root, key);
    }

    // 中序遍历接口
    void inorder() {
        std::cout << "In-order Traversal: ";
        inorderRec(root);
        std::cout << std::endl;
    }
};

int main() {
    BinarySearchTree bst;

    // 模拟数据流插入
    /* 构建的树结构:
            50
           /  \
         30    70
        /  \   /  \
       20  40 60  80
    */
    bst.insert(50);
    bst.insert(30);
    bst.insert(20);
    bst.insert(40);
    bst.insert(70);
    bst.insert(60);
    bst.insert(80);

    // 验证有序性
    bst.inorder(); // 输出应为: 20 30 40 50 60 70 80

    // 验证查找
    std::cout << "Search 40: " << (bst.search(40) ? "Found" : "Not Found") << std::endl;
    std::cout << "Search 90: " << (bst.search(90) ? "Found" : "Not Found") << std::endl;

    // 验证删除
    std::cout << "Delete 20 (Leaf)..." << std::endl;
    bst.remove(20);
    bst.inorder();

    std::cout << "Delete 30 (One Child)..." << std::endl;
    bst.remove(30);
    bst.inorder();
    
    std::cout << "Delete 50 (Two Children - Root)..." << std::endl;
    bst.remove(50);
    bst.inorder();

    return 0;
}

```