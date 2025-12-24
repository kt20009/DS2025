#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <cmath>
#include <queue>
#include <functional>

using namespace std;
using namespace std::chrono;

// 边界框结构体
struct BoundingBox {
    float x1, y1, x2, y2;  // 左上角和右下角坐标
    float score;           // 置信度
    
    BoundingBox(float x1=0, float y1=0, float x2=0, float y2=0, float score=0) 
        : x1(x1), y1(y1), x2(x2), y2(y2), score(score) {}
};

// 计算IOU（交并比）
float calculate_iou(const BoundingBox& box1, const BoundingBox& box2) {
    float inter_x1 = max(box1.x1, box2.x1);
    float inter_y1 = max(box1.y1, box2.y1);
    float inter_x2 = min(box1.x2, box2.x2);
    float inter_y2 = min(box1.y2, box2.y2);
    
    if (inter_x2 < inter_x1 || inter_y2 < inter_y1) {
        return 0.0f;
    }
    
    float inter_area = (inter_x2 - inter_x1) * (inter_y2 - inter_y1);
    float area1 = (box1.x2 - box1.x1) * (box1.y2 - box1.y1);
    float area2 = (box2.x2 - box2.x1) * (box2.y2 - box2.y1);
    float union_area = area1 + area2 - inter_area;
    
    return inter_area / union_area;
}

// 1. 快速排序
void quickSort(vector<int>& indices, vector<float>& scores, int left, int right) {
    if (left >= right) return;
    
    int i = left, j = right;
    float pivot = scores[indices[(left + right) / 2]];
    
    while (i <= j) {
        while (scores[indices[i]] > pivot) i++;
        while (scores[indices[j]] < pivot) j--;
        if (i <= j) {
            swap(indices[i], indices[j]);
            i++;
            j--;
        }
    }
    
    quickSort(indices, scores, left, j);
    quickSort(indices, scores, i, right);
}

// 2. 归并排序
void merge(vector<int>& indices, vector<float>& scores, int left, int mid, int right) {
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    
    while (i <= mid && j <= right) {
        if (scores[indices[i]] >= scores[indices[j]]) {
            temp[k++] = indices[i++];
        } else {
            temp[k++] = indices[j++];
        }
    }
    
    while (i <= mid) temp[k++] = indices[i++];
    while (j <= right) temp[k++] = indices[j++];
    
    for (int p = 0; p < k; p++) {
        indices[left + p] = temp[p];
    }
}

void mergeSort(vector<int>& indices, vector<float>& scores, int left, int right) {
    if (left >= right) return;
    
    int mid = left + (right - left) / 2;
    mergeSort(indices, scores, left, mid);
    mergeSort(indices, scores, mid + 1, right);
    merge(indices, scores, left, mid, right);
}

// 3. 堆排序
void heapify(vector<int>& indices, vector<float>& scores, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < n && scores[indices[left]] > scores[indices[largest]])
        largest = left;
    
    if (right < n && scores[indices[right]] > scores[indices[largest]])
        largest = right;
    
    if (largest != i) {
        swap(indices[i], indices[largest]);
        heapify(indices, scores, n, largest);
    }
}

void heapSort(vector<int>& indices, vector<float>& scores) {
    int n = indices.size();
    
    // 构建最大堆
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(indices, scores, n, i);
    
    // 提取元素
    for (int i = n - 1; i > 0; i--) {
        swap(indices[0], indices[i]);
        heapify(indices, scores, i, 0);
    }
}

// 4. 冒泡排序
void bubbleSort(vector<int>& indices, vector<float>& scores) {
    int n = indices.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (scores[indices[j]] < scores[indices[j + 1]]) {
                swap(indices[j], indices[j + 1]);
            }
        }
    }
}

// NMS算法实现
vector<int> nms(vector<BoundingBox>& boxes, float threshold, 
                function<void(vector<int>&, vector<float>&)> sortFunc) {
    int n = boxes.size();
    vector<float> scores(n);
    vector<int> indices(n);
    
    // 
    for (int i = 0; i < n; i++) {
        scores[i] = boxes[i].score;
        indices[i] = i;
    }
    
    // 使用指定的排序算法
    sortFunc(indices, scores);
    
    vector<int> selected;
    vector<bool> suppressed(n, false);
    
    for (int i = 0; i < n; i++) {
        int idx = indices[i];
        if (suppressed[idx]) continue;
        
        selected.push_back(idx);
        
        for (int j = i + 1; j < n; j++) {
            int next_idx = indices[j];
            if (suppressed[next_idx]) continue;
            
            float iou = calculate_iou(boxes[idx], boxes[next_idx]);
            if (iou > threshold) {
                suppressed[next_idx] = true;
            }
        }
    }
    
    return selected;
}

// 3. 数据生成器
class DataGenerator {
private:
    mt19937 gen;
    uniform_real_distribution<float> dist;
    
public:
    DataGenerator() : gen(random_device{}()), dist(0.0f, 1.0f) {}
    
    // 随机分布生成
    vector<BoundingBox> generateRandom(int num_boxes) {
        vector<BoundingBox> boxes;
        boxes.reserve(num_boxes);
        
        for (int i = 0; i < num_boxes; i++) {
            float x = dist(gen) * 800;
            float y = dist(gen) * 600;
            float w = 20 + dist(gen) * 60;
            float h = 20 + dist(gen) * 60;
            float score = dist(gen);
            
            boxes.emplace_back(x, y, x + w, y + h, score);
        }
        
        return boxes;
    }
    
    // 聚集分布生成
    vector<BoundingBox> generateClustered(int num_boxes) {
        vector<BoundingBox> boxes;
        boxes.reserve(num_boxes);
        
        // 生成3个中心点
        vector<pair<float, float>> centers = {
            {200, 150}, {400, 300}, {600, 450}
        };
        
        normal_distribution<float> normal_dist(0.0f, 50.0f);
        
        for (int i = 0; i < num_boxes; i++) {
            int center_idx = i % centers.size();
            float center_x = centers[center_idx].first;
            float center_y = centers[center_idx].second;
            
            float x = max(0.0f, center_x + normal_dist(gen));
            float y = max(0.0f, center_y + normal_dist(gen));
            float w = 20 + dist(gen) * 40;
            float h = 20 + dist(gen) * 40;
            float score = dist(gen);
            
            x = min(x, 800.0f - w);
            y = min(y, 600.0f - h);
            
            boxes.emplace_back(x, y, x + w, y + h, score);
        }
        
        return boxes;
    }
};

// 4. 性能测试函数
void testPerformance(int num_boxes, const string& distribution) {
    DataGenerator generator;
    vector<BoundingBox> boxes;
    
    if (distribution == "random") {
        boxes = generator.generateRandom(num_boxes);
    } else {
        boxes = generator.generateClustered(num_boxes);
    }
    
    // 定义排序函数
    vector<pair<string, function<void(vector<int>&, vector<float>&)>>> sortAlgorithms = {
        {"快速排序", [](vector<int>& indices, vector<float>& scores) {
            quickSort(indices, scores, 0, indices.size() - 1);
        }},
        {"归并排序", [](vector<int>& indices, vector<float>& scores) {
            mergeSort(indices, scores, 0, indices.size() - 1);
        }},
        {"堆排序", [](vector<int>& indices, vector<float>& scores) {
            heapSort(indices, scores);
        }},
        {"冒泡排序", [](vector<int>& indices, vector<float>& scores) {
            bubbleSort(indices, scores);
        }}
    };
    
    cout << "\n数据规模: " << num_boxes << ", 分布: " << distribution << endl;
    cout << "==========================================" << endl;
    
    for (auto& algorithm : sortAlgorithms) {
        // 复制数据以保证公平比较
        vector<BoundingBox> test_boxes = boxes;
        
        auto start = high_resolution_clock::now();
        
        // 运行NMS
        vector<int> selected = nms(test_boxes, 0.5, algorithm.second);
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);
        
        cout << algorithm.first << ": " 
             << duration.count() / 1000.0 << " ms, "
             << "保留框数: " << selected.size() << endl;
    }
}

// MAIN 函数 - 程序的入口点
int main() {
    cout << "排序算法在NMS中的性能测试" << endl;
    cout << "==========================" << endl;
    
    vector<int> test_sizes = {100, 500, 1000, 5000, 10000};
    vector<string> distributions = {"random", "clustered"};
    
    for (const auto& size : test_sizes) {
        for (const auto& dist : distributions) {
            testPerformance(size, dist);
        }
    }
    
    return 0;
}
