[MindMap](./Graph.png)


# C++ ���ݽṹ�ʼǣ�ͼ (Graph)

## 1. ���ĸ������ (Common Concepts)

ͼ (Graph) ��һ�ַ��������ݽṹ���ɶ��� (Vertex) ���Ϻͱ� (Edge) ������ɡ����� $G = (V, E)$��

### 1.1 ͼ�ķ���

* **����ͼ (Undirected Graph)**: ��û�з��򣬱�ʾ��������֮���˫���ϵ�����磺 ��ͬ�� $(A, B)$ ��ͬ�� $(B, A)$��
* **����ͼ (Directed Graph/Digraph)**: ���з��������ָ���յ㡣���磺$<A, B>$ ��ʾ�� A �� B���������ܴ� B �� A��
* **��Ȩͼ (Weighted Graph)**: ����ıߴ���Ȩֵ (Weight)�������ڱ�ʾ���롢���û�ʱ�����ġ���Ӧ�������Ϊ**�� (Network)**��

### 1.2 ��ͨ�� (Connectivity)

* **��ͨͼ**: ������ͼ�У����������������֮�䶼����·������Ƹ�ͼ����ͨ�ġ�
* **ǿ��ͨͼ**: ������ͼ�У���������������� $v_i$ �� $v_j$�������ڴ� $v_i$ �� $v_j$ �Լ��� $v_j$ �� $v_i$ ��·������Ƹ�ͼ��ǿ��ͨ�ġ�
* **��ͨ����**: ����ͨͼ�еļ�����ͨ��ͼ��

### 1.3 ����·�� (Degree & Path)

* **�ڽ� (Adjacency)**: ��������֮���б������������ǻ�Ϊ�ڽӵ㡣
* **�� (Degree)**: �붥�������ıߵ�������
* ������ͼ��ϸ��Ϊ **��� (In-degree)** (ָ��õ�ı���) �� **���� (Out-degree)** (�Ӹõ㷢���ı���)��
* **���ֶ���**: ���ж���Ķ�֮�͵��ڱ����� 2 ����


* **·�� (Path)**: �������У�����������������֮����ڱߡ�
* **��·��**: �����ж��㲻�ظ����ֵ�·����



## 2. ͼ�ı�ʾ��洢 (Graph Representation)

�� C++ �У�ѡ�����ִ洢�ṹȡ����ͼ��**ϡ��̶�**�Լ�����ҪƵ��ִ�еĲ�����

### 2.1 �ڽӾ��� (Adjacency Matrix)

* **ԭ��**: ʹ��һ����ά���� `matrix[i][j]` ����ʾ���� $i$ �Ͷ��� $j$ ֮��Ĺ�ϵ��
* ��Ȩͼ��1 ��ʾ�бߣ�0 ��ʾ�ޱߡ�
* ��Ȩͼ���洢Ȩֵ���ޱ�ͨ���� $\infty$ (INT_MAX) ��ʾ��


* **�ռ临�Ӷ�**: $O(V^2)$��
* **���ó���**: **����ͼ** (�����ӽ���������ƽ��)��������Ҫ�����ж�������Ƿ��бߵĳ�����

### 2.2 �ڽӱ� (Adjacency List)

* **ԭ��**: ��ÿ�����㽨��һ���������� `std::vector`�����洢������ö���ֱ���������ڽӵ㡣
* **�ռ临�Ӷ�**: $O(V + E)$��
* **���ó���**: **ϡ��ͼ** (����ԶС�ڶ�������ƽ��)����������㷨�����͹���Ӧ�õ���ѡ��

### 2.3 Ч�ʶԱ� (Efficiency Comparison)

| ���� | �ڽӾ��� | �ڽӱ� |
| --- | --- | --- |
| **�ռ�ռ��** | $O(V^2)$ (�˷ѿռ�) | $O(V+E)$ (��ʡ�ռ�) |
| **�ж������Ƿ��б�** | $O(1)$ | $O(Degree(V))$ |
| **�г�ĳ��������ھ�** | $O(V)$ (�������һ��) | $O(Degree(V))$ (ֱ�ӱ�������) |
| **����/ɾ����** | $O(1)$ | $O(1)$ �� $O(Degree(V))$ |

---

## 3. C++ ����ʵ�� (Implementation)

��Ϊ�����רҵѧ���������������ջ��� STL ��ʵ�ַ�ʽ��Ϊ�˱��������ֱ��ʹ�ã����´����װΪ Class ��ʽ�������˳�ʼ������ɾ���㡢��ɾ�ߵȲ�����

### 3.1 �����ڽӾ����ʵ�� (Adjacency Matrix Implementation)

����ʵ���ʺ϶��������̶��ҽ�С��ͨ�� $V \le 1000$���������Ϊ����ʾ����̬��ɾ���㡱������ʹ���� `vector<vector<int>>`�������㷨������ͨ��ֱ�ӿ������� `int g[1005][1005]`��

```cpp
/**
 * Adjacency Matrix Implementation
 * �ʺϳ���ͼ�����ұ��Ƿ����ֻ�� O(1)
 */
#include <iostream>
#include <vector>
#include <iomanip>

// ʹ��һ��������������󣬱�ʾû�б�
const int INF = 1e9;

class GraphMatrix {
private:
    std::vector<std::vector<int>> matrix;
    int numVertices;
    bool isDirected;

public:
    // ��ʼ�������캯��
    GraphMatrix(int vertices, bool directed = false) {
        numVertices = vertices;
        isDirected = directed;
        // ��ʼ������INF ��ʾ�ޱߣ�0 ��ʾ�Լ����Լ�
        matrix.resize(vertices, std::vector<int>(vertices, INF));
        for (int i = 0; i < vertices; ++i) {
            matrix[i][i] = 0;
        }
    }

    // ���Ӷ��� (��̬���ݣ������ϴ� O(V))
    void addVertex() {
        numVertices++;
        // Ϊ���е�ÿһ������һ��
        for (auto& row : matrix) {
            row.push_back(INF);
        }
        // �����µ�һ��
        std::vector<int> newRow(numVertices, INF);
        newRow[numVertices - 1] = 0;
        matrix.push_back(newRow);
        std::cout << "Added vertex " << numVertices - 1 << std::endl;
    }

    // ɾ������ (��Ҫ�ƶ�����Ԫ�أ��������� O(V^2))
    // ע�⣺ɾ������ v ��ԭ���� v+1 ��Ϊ v���Դ�����
    void removeVertex(int v) {
        if (v < 0 || v >= numVertices) return;

        // 1. ɾ���� v ��
        matrix.erase(matrix.begin() + v);

        // 2. ɾ��ÿһ���еĵ� v ��
        for (auto& row : matrix) {
            row.erase(row.begin() + v);
        }

        numVertices--;
        std::cout << "Removed vertex " << v << ". Indices shifted." << std::endl;
    }

    // ���ӱ� (��Ȩֵ)
    void addEdge(int from, int to, int weight) {
        if (from >= numVertices || to >= numVertices) return;
        
        matrix[from][to] = weight;
        if (!isDirected) {
            matrix[to][from] = weight;
        }
    }

    // ɾ����
    void removeEdge(int from, int to) {
        if (from >= numVertices || to >= numVertices) return;

        matrix[from][to] = INF;
        if (!isDirected) {
            matrix[to][from] = INF;
        }
    }

    // ��ӡ����
    void print() {
        std::cout << "\n--- Adjacency Matrix ---\n";
        for (int i = 0; i < numVertices; ++i) {
            for (int j = 0; j < numVertices; ++j) {
                if (matrix[i][j] == INF) 
                    std::cout << std::setw(4) << "INF";
                else 
                    std::cout << std::setw(4) << matrix[i][j];
            }
            std::cout << std::endl;
        }
    }
};

```

### 3.2 �����ڽӱ���ʵ�� (Adjacency List Implementation)

������ͨ�õ�ʵ�ַ�ʽ��ʹ�� `std::vector` Ƕ�� `std::pair` ���洢�ھӺ�Ȩֵ��

```cpp
/**
 * Adjacency List Implementation
 * �ʺ�ϡ��ͼ����ʡ�ռ䣬�����ھ�Ч�ʸ�
 */
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

// �߽ṹ�壬�洢Ŀ�궥���Ȩֵ
struct Edge {
    int to;
    int weight;
};

class GraphAdjList {
private:
    // adj[u] �洢�� u ���������б�
    std::vector<std::vector<Edge>> adj;
    int numVertices;
    bool isDirected;

public:
    // ��ʼ��
    GraphAdjList(int vertices, bool directed = false) {
        numVertices = vertices;
        isDirected = directed;
        adj.resize(vertices);
    }

    // ���Ӷ���
    void addVertex() {
        numVertices++;
        adj.resize(numVertices); // Vector ���ݱȾ����ö�
        std::cout << "Added vertex " << numVertices - 1 << std::endl;
    }

    // ���ӱ�
    void addEdge(int from, int to, int weight) {
        if (from >= numVertices || to >= numVertices) return;

        adj[from].push_back({to, weight});
        if (!isDirected) {
            adj[to].push_back({from, weight});
        }
    }

    // ɾ���� (��Ҫ������������)
    void removeEdge(int from, int to) {
        if (from >= numVertices || to >= numVertices) return;

        // Lambda ����ʽ���ڲ��Ҳ�ɾ��Ŀ���
        auto removeFunc = [to](const Edge& e) { return e.to == to; };
        
        // �� from ���б���ɾ��
        auto& listFrom = adj[from];
        listFrom.erase(std::remove_if(listFrom.begin(), listFrom.end(), removeFunc), listFrom.end());

        if (!isDirected) {
            auto removeFuncReverse = [from](const Edge& e) { return e.to == from; };
            auto& listTo = adj[to];
            listTo.erase(std::remove_if(listTo.begin(), listTo.end(), removeFuncReverse), listTo.end());
        }
    }

    /**
     * ɾ������ (�߼�����)
     * ע�⣺���ڽӱ�������ɾ��һ������ᵼ�º�����������������仯
     * ����Ҫ��������ͼɾ��ָ��ö���ıߡ�
     * ���ӶȽϸߣ�ͨ�������в��� "���ɾ��" (Lazy Deletion) ��ʹ�� Map ӳ�� ID��
     * ������ʾ��׼������ɾ���߼���
     */
    void removeVertex(int v) {
        if (v < 0 || v >= numVertices) return;

        // 1. ɾ���ö�����ڽӱ�
        adj.erase(adj.begin() + v);

        // 2. ����ʣ������ж��㣬ɾ��ָ�� v �ıߣ�������ָ�� v ���涥�������
        for (auto& edges : adj) {
            for (auto it = edges.begin(); it != edges.end(); ) {
                if (it->to == v) {
                    it = edges.erase(it); // ɾ��ָ�� v �ı�
                } else {
                    if (it->to > v) {
                        it->to--; // �������������� v �Ķ����ż� 1
                    }
                    ++it;
                }
            }
        }
        numVertices--;
        std::cout << "Removed vertex " << v << ". Indices shifted." << std::endl;
    }

    // ��ӡ�ڽӱ�
    void print() {
        std::cout << "\n--- Adjacency List ---\n";
        for (int i = 0; i < numVertices; ++i) {
            std::cout << "Vertex " << i << ":";
            for (const auto& edge : adj[i]) {
                std::cout << " -> (" << edge.to << ", w:" << edge.weight << ")";
            }
            std::cout << std::endl;
        }
    }
};

```

### 3.3 ���Դ��� (Main)

```cpp
int main() {
    // 1. �����ڽӾ��� (����ͼ)
    std::cout << "=== Testing Adjacency Matrix ===" << std::endl;
    GraphMatrix gMat(3, false); // 3�����㣬����
    gMat.addEdge(0, 1, 10);
    gMat.addEdge(1, 2, 20);
    gMat.print();
    
    gMat.addVertex(); // �������� 3
    gMat.addEdge(3, 0, 5);
    gMat.print();

    gMat.removeVertex(1); // ɾ������ 1��ԭ���� 2 ��� 1��ԭ 3 ��� 2
    gMat.print();

    // 2. �����ڽӱ� (����ͼ)
    std::cout << "\n=== Testing Adjacency List ===" << std::endl;
    GraphAdjList gList(3, true); // 3�����㣬����
    gList.addEdge(0, 1, 5);
    gList.addEdge(0, 2, 3);
    gList.addEdge(2, 1, 1);
    gList.print();

    gList.removeEdge(0, 2);
    gList.print();
    
    gList.removeVertex(1); // ɾ������ 1
    gList.print();

    return 0;
}

```