#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <climits>
#include <algorithm>
#include <set>
using namespace std;

const int INF = INT_MAX;

// 图类
class Graph {
private:
    int V; // 节点数
    vector<vector<int> > adjMatrix; // 邻接矩阵
    vector<vector<pair<int, int>>> adjList; // 邻接表 (节点, 权重)
    vector<char> nodeNames; // 节点名称映射

public:
    // 构造函数
    Graph(int vertices, const vector<char>& names) : V(vertices), nodeNames(names) {
        // 初始化邻接矩阵
        adjMatrix.resize(V, vector<int>(V, INF));
        for (int i = 0; i < V; ++i) {
            adjMatrix[i][i] = 0;
        }
        // 初始化邻接表
        adjList.resize(V);
    }

    // 添加边 (带权)
    void addEdge(int u, int v, int weight) {
        adjMatrix[u][v] = weight;
        adjMatrix[v][u] = weight; // 无向图
        adjList[u].emplace_back(v, weight);
        adjList[v].emplace_back(u, weight);
    }

    // 添加边 (无权)
    void addEdge(int u, int v) {
        addEdge(u, v, 1); // 默认为1
    }

    // 获取节点索引
    int getNodeIndex(char name) {
        auto it = find(nodeNames.begin(), nodeNames.end(), name);
        if (it != nodeNames.end()) {
            return it - nodeNames.begin();
        }
        return -1;
    }

    // 输出邻接矩阵
    void printAdjMatrix() {
        cout << "邻接矩阵:" << endl;
        // 打印列名
        cout << "  ";
        for (char c : nodeNames) {
            cout << c << " ";
        }
        cout << endl;
        // 打印矩阵内容
        for (int i = 0; i < V; ++i) {
            cout << nodeNames[i] << " ";
            for (int j = 0; j < V; ++j) {
                if (adjMatrix[i][j] == INF) {
                    cout << "∞ ";
                } else {
                    cout << adjMatrix[i][j] << " ";
                }
            }
            cout << endl;
        }
    }

    // BFS遍历
    void BFS(char startName) {
        int start = getNodeIndex(startName);
        if (start == -1) {
            cout << "起始节点不存在" << endl;
            return;
        }

        vector<bool> visited(V, false);
        queue<int> q;
        q.push(start);
        visited[start] = true;

        cout << "BFS遍历结果: ";
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            cout << nodeNames[u] << " ";

            for (auto& edge : adjList[u]) {
                int v = edge.first;
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        cout << endl;
    }

    // DFS遍历 (递归)
    void DFSUtil(int u, vector<bool>& visited) {
        visited[u] = true;
        cout << nodeNames[u] << " ";

        for (auto& edge : adjList[u]) {
            int v = edge.first;
            if (!visited[v]) {
                DFSUtil(v, visited);
            }
        }
    }

    void DFS(char startName) {
        int start = getNodeIndex(startName);
        if (start == -1) {
            cout << "起始节点不存在" << endl;
            return;
        }

        vector<bool> visited(V, false);
        cout << "DFS遍历结果: ";
        DFSUtil(start, visited);
        cout << endl;
    }

    // Dijkstra最短路径算法
    void dijkstra(char startName) {
        int start = getNodeIndex(startName);
        if (start == -1) {
            cout << "起始节点不存在" << endl;
            return;
        }

        vector<int> dist(V, INF);
        vector<int> prev(V, -1);
        dist[start] = 0;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (auto& edge : adjList[u]) {
                int v = edge.first;
                int weight = edge.second;
                if (dist[v] > dist[u] + weight) {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        cout << "Dijkstra最短路径 (从" << startName << "出发):" << endl;
        for (int i = 0; i < V; ++i) {
            if (i == start) continue;
            cout << startName << "到" << nodeNames[i] << ": ";
            if (dist[i] == INF) {
                cout << "不可达";
            } else {
                cout << dist[i] << ", 路径: ";
                // 回溯路径
                stack<int> path;
                int curr = i;
                while (curr != -1) {
                    path.push(curr);
                    curr = prev[curr];
                }
                while (!path.empty()) {
                    cout << nodeNames[path.top()];
                    path.pop();
                    if (!path.empty()) cout << "->";
                }
            }
            cout << endl;
        }
    }

    // Prim最小生成树算法
    void prim(char startName) {
        int start = getNodeIndex(startName);
        if (start == -1) {
            cout << "起始节点不存在" << endl;
            return;
        }

        vector<int> key(V, INF);
        vector<int> parent(V, -1);
        vector<bool> inMST(V, false);

        key[start] = 0;

        for (int count = 0; count < V - 1; ++count) {
            // 找到key最小的节点
            int minKey = INF, u;
            for (int v = 0; v < V; ++v) {
                if (!inMST[v] && key[v] < minKey) {
                    minKey = key[v];
                    u = v;
                }
            }

            inMST[u] = true;

            // 更新邻接节点的key值
            for (auto& edge : adjList[u]) {
                int v = edge.first;
                int weight = edge.second;
                if (!inMST[v] && weight < key[v]) {
                    key[v] = weight;
                    parent[v] = u;
                }
            }
        }

        cout << "Prim最小生成树 (从" << startName << "出发):" << endl;
        int totalWeight = 0;
        for (int i = 0; i < V; ++i) {
            if (i != start) {
                cout << nodeNames[parent[i]] << "-" << nodeNames[i] << " (" << key[i] << ")" << endl;
                totalWeight += key[i];
            }
        }
        cout << "总权重: " << totalWeight << endl;
    }

    // Tarjan算法找双连通分量和关节点
    void tarjanUtil(int u, vector<int>& disc, vector<int>& low, vector<int>& parent, 
                   vector<bool>& ap, stack<pair<int, int>>& edgeStack, 
                   vector<vector<pair<int, int>>>& components, int& time) {
        int children = 0;
        disc[u] = low[u] = ++time;

        for (auto& edge : adjList[u]) {
            int v = edge.first;
            if (disc[v] == -1) { // 未访问
                children++;
                parent[v] = u;
                edgeStack.push({u, v});
                tarjanUtil(v, disc, low, parent, ap, edgeStack, components, time);

                low[u] = min(low[u], low[v]);

                // 检查u是否为关节点
                if ((parent[u] == -1 && children > 1) || (parent[u] != -1 && low[v] >= disc[u])) {
                    ap[u] = true;
                    // 提取双连通分量
                    vector<pair<int, int>> component;
                    while (edgeStack.top() != make_pair(u, v)) {
                        component.push_back(edgeStack.top());
                        edgeStack.pop();
                    }
                    component.push_back(edgeStack.top());
                    edgeStack.pop();
                    components.push_back(component);
                }
            } else if (v != parent[u] && disc[v] < disc[u]) { // 回边且不是父节点
                low[u] = min(low[u], disc[v]);
                edgeStack.push({u, v});
            }
        }
    }

    // 找双连通分量和关节点
    void findBiconnectedComponentsAndAPs() {
        vector<int> disc(V, -1);
        vector<int> low(V, -1);
        vector<int> parent(V, -1);
        vector<bool> ap(V, false);
        stack<pair<int, int>> edgeStack;
        vector<vector<pair<int, int>>> components;
        int time = 0;

        for (int i = 0; i < V; ++i) {
            if (disc[i] == -1) {
                tarjanUtil(i, disc, low, parent, ap, edgeStack, components, time);
                // 处理剩余边
                if (!edgeStack.empty()) {
                    vector<pair<int, int>> component;
                    while (!edgeStack.empty()) {
                        component.push_back(edgeStack.top());
                        edgeStack.pop();
                    }
                    components.push_back(component);
                }
            }
        }

        // 输出关节点
        cout << "关节点: ";
        for (int i = 0; i < V; ++i) {
            if (ap[i]) {
                cout << nodeNames[i] << " ";
            }
        }
        cout << endl;

        // 输出双连通分量
        cout << "双连通分量数量: " << components.size() << endl;
        for (int i = 0; i < components.size(); ++i) {
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                }
        }
    };

int main() {
    // 图1初始化
    vector<char> nodes1 = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    Graph g1(8, nodes1);
    g1.addEdge(g1.getNodeIndex('A'), g1.getNodeIndex('B'), 4);
    g1.addEdge(g1.getNodeIndex('A'), g1.getNodeIndex('D'), 6);
    g1.addEdge(g1.getNodeIndex('A'), g1.getNodeIndex('G'), 7);
    g1.addEdge(g1.getNodeIndex('B'), g1.getNodeIndex('C'), 12);
    g1.addEdge(g1.getNodeIndex('C'), g1.getNodeIndex('E'), 1); // 取最小权重1
    g1.addEdge(g1.getNodeIndex('C'), g1.getNodeIndex('F'), 2);
    g1.addEdge(g1.getNodeIndex('C'), g1.getNodeIndex('H'), 10);
    g1.addEdge(g1.getNodeIndex('D'), g1.getNodeIndex('G'), 2);
    g1.addEdge(g1.getNodeIndex('D'), g1.getNodeIndex('E'), 13);
    g1.addEdge(g1.getNodeIndex('E'), g1.getNodeIndex('F'), 5);
    g1.addEdge(g1.getNodeIndex('E'), g1.getNodeIndex('G'), 11);
    g1.addEdge(g1.getNodeIndex('E'), g1.getNodeIndex('H'), 8);
    g1.addEdge(g1.getNodeIndex('F'), g1.getNodeIndex('H'), 3);
    g1.addEdge(g1.getNodeIndex('G'), g1.getNodeIndex('H'), 14);

    // 任务1: 输出图1邻接矩阵
    cout << "=== 任务1: 图1邻接矩阵 ===" << endl;
    g1.printAdjMatrix();
    cout << endl;

    // 任务2: BFS和DFS
    cout << "=== 任务2: BFS和DFS遍历 ===" << endl;
    g1.BFS('A');
    g1.DFS('A');
    cout << endl;

    // 任务3: 最短路径和最小生成树
    cout << "=== 任务3: 最短路径和最小生成树 ===" << endl;
    g1.dijkstra('A');
    cout << endl;
    g1.prim('A');
    cout << endl;

    // 图2初始化
    vector<char> nodes2 = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'};
    Graph g2(12, nodes2);
    g2.addEdge(g2.getNodeIndex('A'), g2.getNodeIndex('B'));
    g2.addEdge(g2.getNodeIndex('B'), g2.getNodeIndex('F'));
    g2.addEdge(g2.getNodeIndex('E'), g2.getNodeIndex('F'));
    g2.addEdge(g2.getNodeIndex('E'), g2.getNodeIndex('I'));
    g2.addEdge(g2.getNodeIndex('F'), g2.getNodeIndex('G'));
    g2.addEdge(g2.getNodeIndex('F'), g2.getNodeIndex('J'));
    g2.addEdge(g2.getNodeIndex('F'), g2.getNodeIndex('K'));
    g2.addEdge(g2.getNodeIndex('C'), g2.getNodeIndex('F'));
    g2.addEdge(g2.getNodeIndex('C'), g2.getNodeIndex('G'));
    g2.addEdge(g2.getNodeIndex('C'), g2.getNodeIndex('D'));

    // 任务4: 双连通分量和关节点
    cout << "=== 任务4: 双连通分量和关节点 ===" << endl;
    // 测试不同起点
    vector<char> starts = {'A', 'F', 'C'};
    for (char start : starts) {
        cout << "以" << start << "为起点:" << endl;
        g2.findBiconnectedComponentsAndAPs();
        cout << endl;
    }

    return 0;
}