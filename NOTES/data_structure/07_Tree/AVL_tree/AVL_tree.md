[MindMap](./AVL_tree.png)


# 数据结构笔记：AVL 树 (Adelson-Velsky and Landis Tree)

## 1. 定义 (Definition)

AVL 树是**第一种**被发明的**自平衡二叉搜索树 (Self-balancing Binary Search Tree)**。它在二叉搜索树 (BST) 的基础上增加了一个核心约束：

* **平衡约束**：对于树中的**任意**节点，其左子树和右子树的高度差（绝对值）不能超过 1。
* **有序性**：依然满足 BST 的左小右大规则。

如果任何节点的左右子树高度差超过 1，AVL 树会立即通过**旋转 (Rotation)** 操作来重新恢复平衡。这保证了树的高度始终维持在 `O(log n)` 级别，从而杜绝了普通 BST 退化为链表的可能性。

## 2. 核心术语 (Terminology)

### 2.1 节点高度 (Node Height)

* **定义**：节点到其最远叶子节点的最长路径上的边数（或节点数，取决于具体实现）。
* **约定**：在本笔记的代码中，我们约定**空节点的高度为 0**，**叶子节点的高度为 1**。
* **计算公式**：`Height(node) = max(Height(node->left), Height(node->right)) + 1`

### 2.2 平衡因子 (Balance Factor, BF)

* **定义**：某个节点的左子树高度减去右子树高度。
* **公式**：`BF(node) = Height(node->left) - Height(node->right)`
* **合法范围**：在 AVL 树中，所有节点的 BF 必须属于 `{ -1, 0, 1 }`。
* `BF > 1`：左子树太高（左重，Left-Heavy）。
* `BF < -1`：右子树太高（右重，Right-Heavy）。



## 3. 旋转操作 (Rotation Operations)

当插入或删除节点导致某节点的 `|BF| > 1` 时，需要通过旋转来修复。旋转分为四种情况，取决于破坏平衡的节点即其子树的位置。

| 情况名称 | 失衡节点的`BF` | 子节点的`BF` | 描述 | 解决方法 | 记忆口诀 |
| --- | --- | --- | --- | --- | --- |
| **LL (Left-Left)** | `> 1` | `>= 0` | 在左孩子的左子树插入导致不平衡 | **右旋 (Right Rotation)** | 左左太重，向右旋 |
| **RR (Right-Right)** | `> 1` | `< 0` | 在右孩子的右子树插入导致不平衡 | **左旋 (Left Rotation)** | 右右太重，向左旋 |
| **LR (Left-Right)** | `< -1` | `<= 0` | 在左孩子的右子树插入导致不平衡 | **先左旋后右旋 (Left-Right Rotation)** | 左右折线，先左底旋，再整体右旋 |
| **RL (Right-Left)** | `< -1`| `> 0` | 在右孩子的左子树插入导致不平衡 | **先右旋后左旋 (Right-Left Rotation)** | 右左折线，先右底旋，再整体左旋 |

### 3.1 旋转细节

* **右旋 (Right Rotation)**：
* 处理左重情况。根节点变为左孩子的右孩子，原左孩子的右孩子变为原根节点的左孩子。

```cpp
/* 右旋操作 */
TreeNode *rightRotate(TreeNode *node) {
    TreeNode *child = node->left;
    TreeNode *grandChild = child->right;
    // 以 child 为原点，将 node 向右旋转
    child->right = node;
    node->left = grandChild;
    // 更新节点高度
    updateHeight(node);
    updateHeight(child);
    // 返回旋转后子树的根节点
    return child;
}
```

* **左旋 (Left Rotation)**：
* 处理右重情况。根节点变为右孩子的左孩子，原右孩子的左孩子变为原根节点的右孩子。

```cpp
/* 左旋操作 */
TreeNode *leftRotate(TreeNode *node) {
    TreeNode *child = node->right;
    TreeNode *grandChild = child->left;
    // 以 child 为原点，将 node 向左旋转
    child->left = node;
    node->right = grandChild;
    // 更新节点高度
    updateHeight(node);
    updateHeight(child);
    // 返回旋转后子树的根节点
    return child;
}
```



## 4. 核心操作 (Operations)

### 4.1 查找 (Search)

与普通 BST 完全一致。时间复杂度稳定为 `O(log n)`。

### 4.2 插入 (Insert)

1. 执行标准的 BST 插入。
2. **回溯更新高度**：从插入点向上回溯，更新沿途每个节点的高度。
3. **检查平衡**：计算当前节点的平衡因子。
4. **执行旋转**：如果 `|BF| > 1`，根据 LL/RR/LR/RL 四种情况进行旋转。

```cpp
/* 插入节点 */
void insert(int val) {
    root = insertHelper(root, val);
}

/* 递归插入节点（辅助方法） */
TreeNode *insertHelper(TreeNode *node, int val) {
    if (node == nullptr)
        return new TreeNode(val);
    /* 1. 查找插入位置并插入节点 */
    if (val < node->val)
        node->left = insertHelper(node->left, val);
    else if (val > node->val)
        node->right = insertHelper(node->right, val);
    else
        return node;    // 重复节点不插入，直接返回
    updateHeight(node); // 更新节点高度
    /* 2. 执行旋转操作，使该子树重新恢复平衡 */
    node = rotate(node);
    // 返回子树的根节点
    return node;
}
```

### 4.3 删除 (Delete)

1. 执行标准的 BST 删除。
2. **回溯更新高度**：从删除点（或替换节点的位置）向上回溯。
3. **检查平衡并旋转**：与插入类似，但删除操作可能导致由于高度变化引起的多层不平衡，需要一路回溯到根节点进行修复。

---

```cpp
/* 删除节点 */
void remove(int val) {
    root = removeHelper(root, val);
}

/* 递归删除节点（辅助方法） */
TreeNode *removeHelper(TreeNode *node, int val) {
    if (node == nullptr)
        return nullptr;
    /* 1. 查找节点并删除 */
    if (val < node->val)
        node->left = removeHelper(node->left, val);
    else if (val > node->val)
        node->right = removeHelper(node->right, val);
    else {
        if (node->left == nullptr || node->right == nullptr) {
            TreeNode *child = node->left != nullptr ? node->left : node->right;
            // 子节点数量 = 0 ，直接删除 node 并返回
            if (child == nullptr) {
                delete node;
                return nullptr;
            }
            // 子节点数量 = 1 ，直接删除 node
            else {
                delete node;
                node = child;
            }
        } else {
            // 子节点数量 = 2 ，则将中序遍历的下个节点删除，并用该节点替换当前节点
            TreeNode *temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            int tempVal = temp->val;
            node->right = removeHelper(node->right, temp->val);
            node->val = tempVal;
        }
    }
    updateHeight(node); // 更新节点高度
    /* 2. 执行旋转操作，使该子树重新恢复平衡 */
    node = rotate(node);
    // 返回子树的根节点
    return node;
}
```

## 5. C++ 代码实现 (Implementation)

这是一个包含完整插入、删除和自动平衡功能的 AVL 树实现。

```cpp
#include <iostream>
#include <algorithm> // 用于 std::max

// 定义 AVL 树节点
struct Node {
    int key;
    Node* left;
    Node* right;
    int height; // 缓存高度，避免重复计算

    Node(int k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    Node* root;

    // 辅助函数：获取节点高度
    int getHeight(Node* node) {
        if (node == nullptr) return 0;
        return node->height;
    }

    // 辅助函数：获取平衡因子
    int getBalanceFactor(Node* node) {
        if (node == nullptr) return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    // 辅助函数：更新节点高度
    void updateHeight(Node* node) {
        if (node != nullptr) {
            node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
        }
    }

    // 核心操作：右旋 (Right Rotation)
    // 示意图:
    //       y (root)           x
    //      / \               /   \
    //     x   T3    ==>     T1    y
    //    / \                     / \
    //   T1  T2                  T2  T3
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        // 执行旋转
        x->right = y;
        y->left = T2;

        // 更新高度 (必须先更新子节点 y，再更新父节点 x)
        updateHeight(y);
        updateHeight(x);

        // 返回新的根节点
        return x;
    }

    // 核心操作：左旋 (Left Rotation)
    // 示意图:
    //       x (root)           y
    //      / \               /   \
    //     T1  y     ==>     x    T3
    //        / \           / \
    //       T2  T3        T1  T2
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        // 执行旋转
        y->left = x;
        x->right = T2;

        // 更新高度
        updateHeight(x);
        updateHeight(y);

        // 返回新的根节点
        return y;
    }

    // 核心递归：插入节点
    Node* insertRec(Node* node, int key) {
        // 1. 标准 BST 插入
        if (node == nullptr) return new Node(key);

        if (key < node->key)
            node->left = insertRec(node->left, key);
        else if (key > node->key)
            node->right = insertRec(node->right, key);
        else
            return node; // 不允许重复键值

        // 2. 更新当前节点高度
        updateHeight(node);

        // 3. 获取平衡因子，判断是否失衡
        int balance = getBalanceFactor(node);

        // 4. 如果失衡，有 4 种情况

        // 情况 1: LL (左左) -> 右旋
        if (balance > 1 && key < node->left->key) {
            return rightRotate(node);
        }

        // 情况 2: RR (右右) -> 左旋
        if (balance < -1 && key > node->right->key) {
            return leftRotate(node);
        }

        // 情况 3: LR (左右) -> 先左旋后右旋
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // 情况 4: RL (右左) -> 先右旋后左旋
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node; // 如果未失衡，返回原节点指针
    }

    // 辅助函数：查找最小值节点（用于删除）
    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    // 核心递归：删除节点
    Node* deleteRec(Node* root, int key) {
        // 1. 标准 BST 删除
        if (root == nullptr) return root;

        if (key < root->key) {
            root->left = deleteRec(root->left, key);
        } else if (key > root->key) {
            root->right = deleteRec(root->right, key);
        } else {
            // 找到节点，执行删除
            if ((root->left == nullptr) || (root->right == nullptr)) {
                Node* temp = root->left ? root->left : root->right;
                if (temp == nullptr) { // 无子节点
                    temp = root;
                    root = nullptr;
                } else { // 一个子节点
                    *root = *temp; // 复制内容
                }
                delete temp;
            } else {
                // 有两个子节点：获取右子树最小值
                Node* temp = minValueNode(root->right);
                root->key = temp->key;
                root->right = deleteRec(root->right, temp->key);
            }
        }

        if (root == nullptr) return root;

        // 2. 更新高度
        updateHeight(root);

        // 3. 获取平衡因子
        int balance = getBalanceFactor(root);

        // 4. 平衡修复 (注意这里判断条件的微小差异)
        
        // LL Case
        if (balance > 1 && getBalanceFactor(root->left) >= 0)
            return rightRotate(root);

        // LR Case
        if (balance > 1 && getBalanceFactor(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        // RR Case
        if (balance < -1 && getBalanceFactor(root->right) <= 0)
            return leftRotate(root);

        // RL Case
        if (balance < -1 && getBalanceFactor(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    // 中序遍历 (验证有序性)
    void inorderRec(Node* root) {
        if (root != nullptr) {
            inorderRec(root->left);
            std::cout << root->key << " ";
            inorderRec(root->right);
        }
    }

    // 销毁树
    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { destroyTree(root); }

    void insert(int key) {
        root = insertRec(root, key);
    }

    void remove(int key) {
        root = deleteRec(root, key);
    }

    void display() {
        std::cout << "In-order Traversal: ";
        inorderRec(root);
        std::cout << std::endl;
        if (root) std::cout << "Root is: " << root->key << std::endl;
    }
};

int main() {
    AVLTree tree;

    // 测试数据：如果这是普通BST，将会退化成链表
    std::cout << "Inserting 10, 20, 30, 40, 50, 25..." << std::endl;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30); // 触发 RR 旋转
    tree.insert(40);
    tree.insert(50); // 触发 RR 旋转
    tree.insert(25); // 触发 RL 旋转

    /* 此时树的形状应该是平衡的：
            30
           /  \
         20    40
        /  \     \
       10  25    50
    */

    tree.display();

    std::cout << "Deleting 30..." << std::endl;
    tree.remove(30); // 删除根节点，触发重新平衡
    tree.display();

    return 0;
}

```

## 5. 另一版 (Another Implementation)
```cpp
#include<iostream>
#include<algorithm>


class AVLTree
{
private:
	struct TreeNode
	{
		int val;
		int height;
		TreeNode* left;
		TreeNode* right;
		TreeNode(int v = 0, int h = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
			:val(v), height(h), left(l), right(r)
		{};
	};
	TreeNode* root;
	//释放内存
	void destroy(TreeNode* node)
	{
		if (node == nullptr) return;
		destroy(node->left);
		destroy(node->right);
		delete node;
	}
	// 中序遍历辅助方法
	void inOrderHelper(TreeNode* node)
	{
		if (node == nullptr) return;
		inOrderHelper(node->left);
		std::cout << node->val << " ";
		inOrderHelper(node->right);
	}
	// 验证是否平衡辅助方法
	bool isBalancedHelper(TreeNode* node)
	{
		if (node == nullptr)
			return true;
		int balanceFactor = getbalanceFactor(node);
		if (balanceFactor > 1 || balanceFactor < -1)
			return false;
		return isBalancedHelper(node->left) && isBalancedHelper(node->right);
	}
public:
	AVLTree(TreeNode* r = nullptr) :root(r) {};
	~AVLTree()
	{
		destroy(root);
	}
	//获取节点高度
	int getHeight(TreeNode* node)
	{
		if (node == nullptr)
			return -1;
		return node->height;
	}
	//更新节点高度
	void updateHeight(TreeNode* node)
	{
		node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
	}
	//获取节点平衡因子
	int getbalanceFactor(TreeNode* node)
	{
		if (node == nullptr)
			return 0;
		return getHeight(node->left) - getHeight(node->right);
	}
	//右旋
	TreeNode* rightRotate(TreeNode* node)
	{
		TreeNode* child = node->left;
		TreeNode* grandChild = child->right;
		//右旋操作
		child->right = node;
		node->left = grandChild;
		//更新节点高度
		updateHeight(node);
		updateHeight(child);
		return child;
	}
	//左旋
	TreeNode* leftRotate(TreeNode* node)
	{
		TreeNode* child = node->right;
		TreeNode* grandChild = child->left;
		//左旋操作
		child->left = node;
		node->right = grandChild;
		//更新节点高度
		updateHeight(node);
		updateHeight(child);
		return child;
	}
	//旋转
	TreeNode* rotate(TreeNode* node)
	{
		int balanceFactor = getbalanceFactor(node);
		//左偏树
		if (balanceFactor > 1)
		{
			//直接右旋
			if (getbalanceFactor(node->left) >= 0)
			{
				return rightRotate(node);
			}
			//先左旋再右旋
			else
			{
				node->left = leftRotate(node->left);
				return rightRotate(node);
			}
		}
		//右偏树
		else if (balanceFactor < -1)
		{
			//直接左旋
			if (getbalanceFactor(node->right) <= 0)
			{
				return leftRotate(node);
			}
			//先右旋再左旋
			else
			{
				node->right = rightRotate(node->right);
				return leftRotate(node);
			}
		}
		return node;
	}
	//插入元素
	void insert(int num)
	{
		root = insertHelper(root, num);
	}
	//插入元素的辅助方法
	TreeNode* insertHelper(TreeNode* node, int num)
	{
		//若超出叶
		if (node == nullptr)
		{
			node = new TreeNode(num);
			return node;
		}
		//寻找插入位置并插入
		if (num < node->val)
			node->left = insertHelper(node->left, num);
		else if (num > node->val)
			node->right = insertHelper(node->right, num);
		else
			return node;
		//更新节点高度
		updateHeight(node);
		//旋转使得平衡
		node = rotate(node);
		return node;
	}
	//删除元素
	void remove(int target)
	{
		root = removeHelper(root, target);
	}
	//删除元素的辅助方法
	TreeNode* removeHelper(TreeNode* node, int target)
	{
		if (node == nullptr)
			return node;
		if (target < node->val)
			node->left = removeHelper(node->left, target);
		else if (target > node->val)
			node->right = removeHelper(node->right, target);
		else
		{
			if (node->left == nullptr || node->right == nullptr)
			{
				TreeNode* child = (node->left == nullptr ? node->right : node->left);
				if (child == nullptr)
				{
					delete node;
					return nullptr;
				}
				else
				{
					TreeNode* temp = node;
					node = child;
					delete temp;
					return node;
				}
			}
			else
			{
				TreeNode* temp = node->right;
				while (temp->left)
				{
					temp = temp->left;
				}
				int tempVal = temp->val;
				node->right = removeHelper(node->right, tempVal);
				node->val = tempVal;
			}
		}
		updateHeight(node);
		node = rotate(node);
		return node;
	}
	//查找元素
	bool search(int target)
	{
		TreeNode* cur = root;
		while (cur)
		{
			if (target < cur->val)
				cur = cur->left;
			else if (target > cur->val)
				cur = cur->right;
			else return true;
		}
		return false;
	}
	//外部打印接口
	void printInOrder()
	{
		std::cout << "inOrder : ";
		inOrderHelper(root);
		std::cout << std::endl;
	}
	//外部验证接口
	bool isBalanced()
	{
		return isBalancedHelper(root);
	}
	// 获取根节点的值
	int getRootVal()
	{
		if (root) return root->val;
		return -9999;
	}
};

int main()
{
	AVLTree tree;

	std::cout << "1.测试插入与自动平衡" << std::endl;

	tree.insert(10);
	tree.insert(20);
	tree.insert(30);

	std::cout << "插入 10, 20, 30 后:" << std::endl;
	tree.printInOrder();
	std::cout << "当前根节点应为 20: " << tree.getRootVal() << std::endl;

	tree.insert(40);
	tree.insert(50); // 触发 RR
	tree.insert(25); // 触发 RL

	std::cout << "插入完整数据后 (10,20,30,40,50,25):" << std::endl;
	tree.printInOrder();
	std::cout << "树是否平衡: " << (tree.isBalanced() ? "是" : "否") << std::endl;
	std::cout << "当前根节点应为 30: " << tree.getRootVal() << std::endl;

	std::cout << "\n2.测试查找" << std::endl;
	std::cout << "查找 25 (存在): " << (tree.search(25) ? "Found" : "Not Found") << std::endl;
	std::cout << "查找 99 (不存在): " << (tree.search(99) ? "Found" : "Not Found") << std::endl;

	std::cout << "\n3.测试删除" << std::endl;

	// 删除叶子节点
	std::cout << "删除 10 (叶子)" << std::endl;
	tree.remove(10);
	tree.printInOrder();

	// 删除带两个子节点的节点
	std::cout << "删除 30 (根节点，有两个子节点)" << std::endl;
	tree.remove(30);

	tree.printInOrder();
	std::cout << "树是否平衡: " << (tree.isBalanced() ? "是" : "否") << std::endl;
	std::cout << "删除后的根节点: " << tree.getRootVal() << std::endl;

	return 0;
}
```