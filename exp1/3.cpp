#include <iostream>
#include <stack>
#include <cstdlib>
#include <ctime>

// 不使用using namespace std，避免可能的命名冲突
// 所有标准库成员都显式使用std::前缀

// 计算最大矩形面积的函数
int largestRectangleArea(int* heights, int n) {
    if (n <= 0) return 0;
    
    std::stack<int> stk;  // 存储索引的栈
    int maxArea = 0;
    
    for (int i = 0; i < n; ++i) {
        // 当前高度小于栈顶高度时，计算面积
        while (!stk.empty() && heights[i] < heights[stk.top()]) {
            int height = heights[stk.top()];
            stk.pop();
            
            // 计算宽度
            int width;
            if (stk.empty()) {
                width = i;
            } else {
                width = i - stk.top() - 1;
            }
            
            // 更新最大面积
            int area = height * width;
            if (area > maxArea) {
                maxArea = area;
            }
        }
        stk.push(i);
    }
    
    // 处理栈中剩余元素
    while (!stk.empty()) {
        int height = heights[stk.top()];
        stk.pop();
        
        int width;
        if (stk.empty()) {
            width = n;
        } else {
            width = n - stk.top() - 1;
        }
        
        int area = height * width;
        if (area > maxArea) {
            maxArea = area;
        }
    }
    
    return maxArea;
}

// 生成随机高度数组
int* generateHeights(int size) {
    int* heights = (int*)malloc(sizeof(int) * size);
    for (int i = 0; i < size; ++i) {
        heights[i] = rand() % 10001;  // 0-10000之间的随机数
    }
    return heights;
}

// 打印数组
void printHeights(int* heights, int size) {
    std::cout << "[";
    int show = (size < 20) ? size : 20;
    for (int i = 0; i < show; ++i) {
        std::cout << heights[i];
        if (i < show - 1) {
            std::cout << ", ";
        }
    }
    if (size > 20) {
        std::cout << ", ...";
    }
    std::cout << "] (长度: " << size << ")" << std::endl;
}

int main() {
    // 示例测试 - 手动初始化，不使用任何C++11特性
    int exampleSize = 6;
    int* example = (int*)malloc(sizeof(int) * exampleSize);
    example[0] = 2;
    example[1] = 1;
    example[2] = 5;
    example[3] = 6;
    example[4] = 2;
    example[5] = 3;
    
    std::cout << "=== 示例测试 ===" << std::endl;
    std::cout << "输入: ";
    printHeights(example, exampleSize);
    std::cout << "最大矩形面积: " << largestRectangleArea(example, exampleSize) << std::endl;
    std::cout << "预期输出: 10" << std::endl << std::endl;
    free(example);  // 释放内存
    
    // 10组测试数据
    std::cout << "=== 随机测试数据 ===" << std::endl;
    srand((unsigned int)time(0));  // 初始化随机数生成器
    
    int testSizes[10] = {1, 10, 50, 100, 500, 1000, 10000, 30000, 50000, 100000};
    
    for (int i = 0; i < 10; ++i) {
        std::cout << "测试 " << (i + 1) << ":" << std::endl;
        int size = testSizes[i];
        int* heights = generateHeights(size);
        
        printHeights(heights, size);
        
        // 计算并计时
        clock_t start = clock();
        int area = largestRectangleArea(heights, size);
        clock_t end = clock();
        
        double time = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
        
        std::cout << "最大面积: " << area << std::endl;
        std::cout << "耗时: " << time << " 毫秒" << std::endl << std::endl;
        
        free(heights);  // 释放内存
    }
    
    return 0;
}
