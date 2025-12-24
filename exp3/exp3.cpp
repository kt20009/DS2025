#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
#include <climits>
#include <map>
#include <set>
#include <string>
using namespace std;

// 图类（邻接矩阵实现）
class Graph {
private:
    vector<vector<int>> adjMatrix;
    vector<string> vertices;
    map<string, int> vertexIndex;
    bool directed;
    int n;

public:
    Graph(int size, bool isDirected = false) : directed(isDirected) {
        n = size;
        adjMatrix.resize(n, vector<int>(n, 0));
    }

    // 设置顶点名称
    void setVertices(const vector<string>& verts) {
        vertices = verts;
        for (int i = 0; i < verts.size(); i++) {
            vertexIndex[verts[i]] = i;
        }
        n = verts.size();
        adjMatrix.resize(n, vector<int>(n, 0));
    }

    // 添加边
    void addEdge(const string& u, const string& v, int weight = 1) {
        int i = vertexIndex[u];
        int j = vertexIndex[v];
        adjMatrix[i][j] = weight;
        if (!directed) {
            adjMatrix[j][i] = weight;
        }
    }

    // 获取邻接矩阵
    const vector<vector<int>>& getAdjMatrix() const {
        return adjMatrix;
    }

    // 获取顶点列表
    const vector<string>& getVertices() const {
        return vertices;
    }

    // 获取顶点索引
    int getIndex(const string& v) const {
        if (vertexIndex.find(v) != vertexIndex.end()) {
            return vertexIndex.at(v);
        }
        return -1;
    }

    // 获取邻居节点
    vector<string> getNeighbors(const string& v) const {
        vector<string> neighbors;
        int idx = getIndex(v);
        if (idx == -1) return neighbors;
        
        for (int i = 0; i < n; i++) {
            if (adjMatrix[idx][i] != 0) {
                neighbors.push_back(vertices[i]);
            }
        }
        return neighbors;
    }

    // 获取边的权值
    int getWeight(const string& u, const string& v) const {
        int i = getIndex(u);
        int j = getIndex(v);
        if (i == -1 || j == -1) return 0;
        return adjMatrix[i][j];
    }

    // 打印邻接矩阵
    void printAdjMatrix() const {
        cout << "邻接矩阵:" << endl;
        cout << "   ";
        for (const auto& v : vertices) {
            cout << v << "  ";
        }
        cout << endl;
        
        for (int i = 0; i < n; i++) {
            cout << vertices[i] << ": ";
            for (int j = 0; j < n; j++) {
                cout << adjMatrix[i][j] << " ";
                if (adjMatrix[i][j] < 10) cout << " ";
            }
            cout << endl;
        }
    }
};

// BFS实现
vector<string> bfs(const Graph& graph, const string& start) {
    vector<string> traversal;
    set<string> visited;
    queue<string> q;
    
    visited.insert(start);
    q.push(start);
    
    while (!q.empty()) {
        string current = q.front();
        q.pop();
        traversal.push_back(current);
        
        vector<string> neighbors = graph.getNeighbors(current);
        for (const auto& neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }
    
    return traversal;
}

// DFS实现（递归）
void dfsRecursive(const Graph& graph, const string& current, 
                  set<string>& visited, vector<string>& traversal) {
    visited.insert(current);
    traversal.push_back(current);
    
    vector<string> neighbors = graph.getNeighbors(current);
    for (const auto& neighbor : neighbors) {
        if (visited.find(neighbor) == visited.end()) {
            dfsRecursive(graph, neighbor, visited, traversal);
        }
    }
}

vector<string> dfs(const Graph& graph, const string& start) {
    vector<string> traversal;
    set<string> visited;
    dfsRecursive(graph, start, visited, traversal);
    return traversal;
}

// Dijkstra最短路径算法
map<string, int> dijkstra(const Graph& graph, const string& start) {
    map<string, int> dist;
    set<string> visited;
    
    // 初始化距离
    for (const auto& v : graph.getVertices()) {
        dist[v] = INT_MAX;
    }
    dist[start] = 0;
    
    // 使用优先队列（这里用vector模拟，实际应用可以用priority_queue）
    vector<string> unvisited = graph.getVertices();
    
    while (!unvisited.empty()) {
        // 找到当前未访问节点中距离最小的
        string current;
        int minDist = INT_MAX;
        for (const auto& v : unvisited) {
            if (dist[v] < minDist) {
                minDist = dist[v];
                current = v;
            }
        }
        
        // 从unvisited中移除当前节点
        unvisited.erase(remove(unvisited.begin(), unvisited.end(), current), unvisited.end());
        visited.insert(current);
        
        // 更新邻居距离
        vector<string> neighbors = graph.getNeighbors(current);
        for (const auto& neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                int weight = graph.getWeight(current, neighbor);
                if (dist[current] != INT_MAX && dist[current] + weight < dist[neighbor]) {
                    dist[neighbor] = dist[current] + weight;
                }
            }
        }
    }
    
    return dist;
}

// Prim最小生成树算法
vector<tuple<string, string, int>> prim(const Graph& graph, const string& start) {
    vector<tuple<string, string, int>> mstEdges;
    set<string> inMST;
    
    inMST.insert(start);
    
    while (inMST.size() < graph.getVertices().size()) {
        string u, v;
        int minWeight = INT_MAX;
        
        // 找到连接MST和非MST的最小权值边
        for (const auto& vertex : inMST) {
            vector<string> neighbors = graph.getNeighbors(vertex);
            for (const auto& neighbor : neighbors) {
                if (inMST.find(neighbor) == inMST.end()) {
                    int weight = graph.getWeight(vertex, neighbor);
                    if (weight < minWeight) {
                        minWeight = weight;
                        u = vertex;
                        v = neighbor;
                    }
                }
            }
        }
        
        if (minWeight != INT_MAX) {
            mstEdges.push_back(make_tuple(u, v, minWeight));
            inMST.insert(v);
        } else {
            break; // 图不连通
        }
    }
    
    return mstEdges;
}

// Tarjan算法求关节点
class ArticulationFinder {
private:
    const Graph& graph;
    map<string, int> discovery;
    map<string, int> low;
    map<string, string> parent;
    set<string> articulationPoints;
    int time;
    
    void dfs(const string& u) {
        discovery[u] = low[u] = ++time;
        int children = 0;
        
        vector<string> neighbors = graph.getNeighbors(u);
        for (const auto& v : neighbors) {
            if (discovery.find(v) == discovery.end()) {
                children++;
                parent[v] = u;
                dfs(v);
                
                low[u] = min(low[u], low[v]);
                
                // 判断是否为关节点
                if (parent.find(u) == parent.end() && children > 1) {
                    articulationPoints.insert(u);
                }
                if (parent.find(u) != parent.end() && low[v] >= discovery[u]) {
                    articulationPoints.insert(u);
                }
            } else if (v != parent[u]) {
                low[u] = min(low[u], discovery[v]);
            }
        }
    }
    
public:
    ArticulationFinder(const Graph& g) : graph(g), time(0) {}
    
    set<string> findArticulationPoints(const string& start) {
        discovery.clear();
        low.clear();
        parent.clear();
        articulationPoints.clear();
        time = 0;
        
        // 从起点开始DFS
        dfs(start);
        
        // 检查是否所有顶点都被访问（图可能不连通）
        for (const auto& v : graph.getVertices()) {
            if (discovery.find(v) == discovery.end()) {
                dfs(v);
            }
        }
        
        return articulationPoints;
    }
};

// 构建图1
Graph buildGraph1() {
    Graph g(8, false); // 无向图，8个顶点
    
    // 设置顶点
    vector<string> vertices = {"A", "B", "C", "D", "E", "F", "G", "H"};
    g.setVertices(vertices);
    
    // 添加边
    g.addEdge("A", "B", 4);
    g.addEdge("A", "D", 6);
    g.addEdge("A", "G", 7);
    g.addEdge("B", "C", 12);
    g.addEdge("C", "F", 2);
    g.addEdge("C", "H", 10);
    g.addEdge("C", "E", 1);
    g.addEdge("C", "D", 9);
    g.addEdge("F", "H", 3);
    g.addEdge("F", "E", 5);
    g.addEdge("H", "E", 8);
    g.addEdge("H", "G", 14);
    g.addEdge("G", "E", 11);
    g.addEdge("G", "D", 2);
    g.addEdge("D", "E", 13);
    
    return g;
}

// 构建图2
Graph buildGraph2() {
    Graph g(12, false); // 无向图，12个顶点
    
    // 设置顶点
    vector<string> vertices = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"};
    g.setVertices(vertices);
    
    // 添加边
    // ABEF四个节点连接
    g.addEdge("A", "B", 1);
    g.addEdge("B", "E", 1);
    g.addEdge("E", "F", 1);
    g.addEdge("A", "F", 1);
    
    // E再连接I和F，I、F中间也相连
    g.addEdge("E", "I", 1);
    g.addEdge("I", "F", 1);
    
    // F再连接J、C、K、G
    g.addEdge("F", "J", 1);
    g.addEdge("F", "C", 1);
    g.addEdge("F", "K", 1);
    g.addEdge("F", "G", 1);
    
    // J、K相连
    g.addEdge("J", "K", 1);
    
    // G、K相连
    g.addEdge("G", "K", 1);
    
    // K再连接L
    g.addEdge("K", "L", 1);
    
    // C再连接D和H
    g.addEdge("C", "D", 1);
    g.addEdge("C", "H", 1);
    
    // D、H也相连
    g.addEdge("D", "H", 1);
    
    return g;
}

// 打印遍历结果
void printTraversal(const string& name, const vector<string>& traversal) {
    cout << name << "遍历顺序: ";
    for (const auto& v : traversal) {
        cout << v << " ";
    }
    cout << endl;
}

// 打印最短路径结果
void printShortestPaths(const string& start, const map<string, int>& dist) {
    cout << "从" << start << "出发的最短路径:" << endl;
    for (const auto& pair : dist) {
        if (pair.second == INT_MAX) {
            cout << "  " << pair.first << ": 不可达" << endl;
        } else {
            cout << "  " << pair.first << ": " << pair.second << endl;
        }
    }
}

// 打印最小生成树
void printMST(const vector<tuple<string, string, int>>& mst) {
    cout << "最小生成树边:" << endl;
    int totalWeight = 0;
    for (const auto& edge : mst) {
        cout << "  " << get<0>(edge) << " - " << get<1>(edge) << " (" << get<2>(edge) << ")" << endl;
        totalWeight += get<2>(edge);
    }
    cout << "总权值: " << totalWeight << endl;
}

// 打印关节点
void printArticulationPoints(const set<string>& points) {
    if (points.empty()) {
        cout << "没有关节点" << endl;
    } else {
        cout << "关节点: ";
        for (const auto& point : points) {
            cout << point << " ";
        }
        cout << endl;
    }
}

int main() {
    cout << "========== 图1 ==========" << endl;
    Graph g1 = buildGraph1();
    
    // (1) 输出邻接矩阵
    cout << "\n(1) 图1的邻接矩阵:" << endl;
    g1.printAdjMatrix();
    
    // (2) BFS和DFS
    cout << "\n(2) 从A点出发:" << endl;
    vector<string> bfsResult = bfs(g1, "A");
    printTraversal("BFS", bfsResult);
    
    vector<string> dfsResult = dfs(g1, "A");
    printTraversal("DFS", dfsResult);
    
    // (3) 最短路径和最小支撑树
    cout << "\n(3) 从A点出发:" << endl;
    map<string, int> shortestPaths = dijkstra(g1, "A");
    printShortestPaths("A", shortestPaths);
    
    vector<tuple<string, string, int>> mst = prim(g1, "A");
    printMST(mst);
    
    cout << "\n========== 图2 ==========" << endl;
    Graph g2 = buildGraph2();
    
    // (4) 双连通分量和关节点
    cout << "\n(4) 计算关节点:" << endl;
    
    // 选取不同起点
    vector<string> startPoints = {"A", "F", "K"};
    set<set<string>> allPoints; // 存储所有起点得到的关节点集合
    
    for (const auto& start : startPoints) {
        cout << "\n起点: " << start << endl;
        ArticulationFinder finder(g2);
        set<string> points = finder.findArticulationPoints(start);
        printArticulationPoints(points);
        allPoints.insert(points);
    }
    
    // 检查结果是否一致
    if (allPoints.size() == 1) {
        cout << "\n所有起点计算得到的关节点结果一致" << endl;
    } else {
        cout << "\n警告：不同起点计算得到的关节点结果不一致" << endl;
    }
    
    return 0;
}