#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <sstream>  // 新增：用于字符串拼接

// 复数类定义
class Complex {
private:
    double real;  // 实部
    double imag;  // 虚部

public:
    // 构造函数
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    
    // 获取实部
    double getReal() const { return real; }
    
    // 获取虚部
    double getImag() const { return imag; }
    
    // 计算复数的模
    double modulus() const {
        return sqrt(real * real + imag * imag);
    }
    
    // 重载相等运算符（实部和虚部都相等才认为相等）
    bool operator==(const Complex& other) const {
        return (real == other.real) && (imag == other.imag);
    }
    
    // 重载不等运算符
    bool operator!=(const Complex& other) const {
        return !(*this == other);
    }
    
    // 重载输出运算符
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << "(" << c.real << ", " << c.imag << ")";
        return os;
    }
};

// 生成随机复数向量
std::vector<Complex> generateRandomComplexVector(int size, double minVal, double maxVal) {
    std::vector<Complex> vec;
    for (int i = 0; i < size; ++i) {
        double real = minVal + (maxVal - minVal) * rand() / RAND_MAX;
        double imag = minVal + (maxVal - minVal) * rand() / RAND_MAX;
        vec.push_back(Complex(real, imag));
    }
    return vec;
}

// 置乱向量
void shuffleVector(std::vector<Complex>& vec) {
    for (int i = vec.size() - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        std::swap(vec[i], vec[j]);
    }
}

// 查找元素（实部和虚部均相同）
int findElement(const std::vector<Complex>& vec, const Complex& target) {
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i] == target) {
            return i;  // 找到返回索引
        }
    }
    return -1;  // 未找到返回-1
}

// 插入元素
void insertElement(std::vector<Complex>& vec, const Complex& elem, int position) {
    if (position >= 0 && position <= vec.size()) {
        vec.insert(vec.begin() + position, elem);
    } else {
        std::cout << "插入位置无效！" << std::endl;
    }
}

// 删除元素
void deleteElement(std::vector<Complex>& vec, int position) {
    if (position >= 0 && position < vec.size()) {
        vec.erase(vec.begin() + position);
    } else {
        std::cout << "删除位置无效！" << std::endl;
    }
}

// 向量唯一化（去除重复元素）
void uniqueVector(std::vector<Complex>& vec) {
    std::vector<Complex> temp;
    for (const auto& elem : vec) {
        bool found = false;
        for (const auto& t : temp) {
            if (elem == t) {
                found = true;
                break;
            }
        }
        if (!found) {
            temp.push_back(elem);
        }
    }
    vec = temp;
}

// 比较函数：先按模比较，模相同则按实部比较
bool compareComplex(const Complex& a, const Complex& b) {
    if (a.modulus() != b.modulus()) {
        return a.modulus() < b.modulus();
    }
    return a.getReal() < b.getReal();
}

// 起泡排序
void bubbleSort(std::vector<Complex>& vec) {
    int n = vec.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (!compareComplex(vec[j], vec[j + 1])) {
                std::swap(vec[j], vec[j + 1]);
            }
        }
    }
}

// 归并排序辅助函数：合并两个有序子数组
void merge(std::vector<Complex>& vec, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    std::vector<Complex> L(n1), R(n2);
    
    for (int i = 0; i < n1; ++i)
        L[i] = vec[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = vec[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (compareComplex(L[i], R[j])) {
            vec[k] = L[i];
            i++;
        } else {
            vec[k] = R[j];
            j++;
        }
        k++;
    }
    
    while (i < n1) {
        vec[k] = L[i];
        i++;
        k++;
    }
    
    while (j < n2) {
        vec[k] = R[j];
        j++;
        k++;
    }
}

// 归并排序递归函数
void mergeSort(std::vector<Complex>& vec, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        mergeSort(vec, left, mid);
        mergeSort(vec, mid + 1, right);
        
        merge(vec, left, mid, right);
    }
}

// 归并排序接口
void mergeSort(std::vector<Complex>& vec) {
    if (vec.size() <= 1) return;
    mergeSort(vec, 0, vec.size() - 1);
}

// 区间查找：查找模介于[m1, m2)的所有元素
std::vector<Complex> rangeSearch(const std::vector<Complex>& sortedVec, double m1, double m2) {
    std::vector<Complex> result;
    for (const auto& elem : sortedVec) {
        double mod = elem.modulus();
        if (mod >= m1 && mod < m2) {
            result.push_back(elem);
        } else if (mod >= m2) {
            // 由于向量已排序，一旦模大于等于m2，后续元素都不满足条件
            break;
        }
    }
    return result;
}

// 打印向量
void printVector(const std::vector<Complex>& vec, const std::string& msg = "") {
    if (!msg.empty()) {
        std::cout << msg << std::endl;
    }
    int displayCount = std::min(10, (int)vec.size()); // 最多显示10个元素
    for (int i = 0; i < displayCount; ++i) {
        std::cout << vec[i] << " ";
    }
    if (vec.size() > 10) {
        std::cout << "...";
    }
    std::cout << std::endl << "向量大小: " << vec.size() << std::endl << std::endl;
}

int main() {
    srand(time(0)); // 初始化随机数生成器
    
    // 1. 测试无序向量的各种操作
    std::cout << "=== 测试无序向量的操作 ===" << std::endl;
    std::vector<Complex> complexVec = generateRandomComplexVector(10, -10, 10);
    printVector(complexVec, "生成的随机复数向量:");
    
    // 测试置乱
    shuffleVector(complexVec);
    printVector(complexVec, "置乱后的向量:");
    
    // 测试查找
    if (!complexVec.empty()) {
        Complex target = complexVec[3];
        int index = findElement(complexVec, target);
        if (index != -1) {
            // 使用stringstream拼接字符串和对象
            std::stringstream ss;
            ss << "查找元素 " << target << "，在索引 " << index << " 处找到";
            std::cout << ss.str() << std::endl << std::endl;
        } else {
            std::stringstream ss;
            ss << "未找到元素 " << target;
            std::cout << ss.str() << std::endl << std::endl;
        }
    }
    
    // 测试插入
    Complex insertElem(100, 200);
    insertElement(complexVec, insertElem, 2);
    // 修复：使用stringstream拼接插入信息
    std::stringstream insertMsg;
    insertMsg << "插入元素 " << insertElem << " 后的向量:";
    printVector(complexVec, insertMsg.str());
    
    // 测试删除
    if (complexVec.size() > 3) {
        Complex deleteElem = complexVec[3];
        deleteElement(complexVec, 3);
        // 使用stringstream拼接删除信息
        std::stringstream deleteMsg;
        deleteMsg << "删除索引3处的元素 " << deleteElem << " 后的向量:";
        printVector(complexVec, deleteMsg.str());
    }
    
    // 测试唯一化
    uniqueVector(complexVec);
    printVector(complexVec, "唯一化后的向量:");
    
    // 2. 测试排序效率
    std::cout << "=== 测试排序效率 ===" << std::endl;
    int testSize = 5000;  // 测试向量大小
    std::vector<Complex> testVec = generateRandomComplexVector(testSize, -100, 100);
    
    // 测试顺序情况
    std::vector<Complex> sortedVec = testVec;
    mergeSort(sortedVec);  // 先排序使其成为顺序
    
    // 测试逆序情况
    std::vector<Complex> reversedVec = sortedVec;
    std::reverse(reversedVec.begin(), reversedVec.end());
    
    // 测试乱序情况
    std::vector<Complex> shuffledVec = sortedVec;
    shuffleVector(shuffledVec);
    
    // 测试起泡排序在三种情况下的效率
    clock_t start, end;
    
    // 顺序情况的起泡排序
    std::vector<Complex> bubbleSorted1 = sortedVec;
    start = clock();
    bubbleSort(bubbleSorted1);
    end = clock();
    double bubbleTime1 = double(end - start) / CLOCKS_PER_SEC;
    
    // 乱序情况的起泡排序
    std::vector<Complex> bubbleSorted2 = shuffledVec;
    start = clock();
    bubbleSort(bubbleSorted2);
    end = clock();
    double bubbleTime2 = double(end - start) / CLOCKS_PER_SEC;
    
    // 逆序情况的起泡排序
    std::vector<Complex> bubbleSorted3 = reversedVec;
    start = clock();
    bubbleSort(bubbleSorted3);
    end = clock();
    double bubbleTime3 = double(end - start) / CLOCKS_PER_SEC;
    
    // 测试归并排序在三种情况下的效率
    // 顺序情况的归并排序
    std::vector<Complex> mergeSorted1 = sortedVec;
    start = clock();
    mergeSort(mergeSorted1);
    end = clock();
    double mergeTime1 = double(end - start) / CLOCKS_PER_SEC;
    
    // 乱序情况的归并排序
    std::vector<Complex> mergeSorted2 = shuffledVec;
    start = clock();
    mergeSort(mergeSorted2);
    end = clock();
    double mergeTime2 = double(end - start) / CLOCKS_PER_SEC;
    
    // 逆序情况的归并排序
    std::vector<Complex> mergeSorted3 = reversedVec;
    start = clock();
    mergeSort(mergeSorted3);
    end = clock();
    double mergeTime3 = double(end - start) / CLOCKS_PER_SEC;
    
    // 输出排序时间比较
    std::cout << "排序效率比较 (向量大小: " << testSize << "):" << std::endl;
    std::cout << "顺序情况:" << std::endl;
    std::cout << "  起泡排序: " << bubbleTime1 << " 秒" << std::endl;
    std::cout << "  归并排序: " << mergeTime1 << " 秒" << std::endl << std::endl;
    
    std::cout << "乱序情况:" << std::endl;
    std::cout << "  起泡排序: " << bubbleTime2 << " 秒" << std::endl;
    std::cout << "  归并排序: " << mergeTime2 << " 秒" << std::endl << std::endl;
    
    std::cout << "逆序情况:" << std::endl;
    std::cout << "  起泡排序: " << bubbleTime3 << " 秒" << std::endl;
    std::cout << "  归并排序: " << mergeTime3 << " 秒" << std::endl << std::endl;
    
    // 3. 测试区间查找
    std::cout << "=== 测试区间查找 ===" << std::endl;
    // 使用前面已排序的向量进行测试
    double m1 = 5.0, m2 = 15.0;
    // 使用stringstream拼接区间信息
    std::stringstream rangeMsg;
    rangeMsg << "模介于[" << m1 << ", " << m2 << ")的元素:";
    std::vector<Complex> rangeResult = rangeSearch(sortedVec, m1, m2);
    printVector(rangeResult, rangeMsg.str());
    
    return 0;
}
