#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

// 栈数据结构实现
template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };
    Node* top;
    int size;

public:
    // 构造函数
    Stack() : top(nullptr), size(0) {}
    
    // 析构函数
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
    
    // 入栈操作
    void push(T val) {
        Node* newNode = new Node(val);
        newNode->next = top;
        top = newNode;
        size++;
    }
    
    // 出栈操作
    T pop() {
        if (isEmpty()) {
            throw runtime_error("栈为空，无法执行出栈操作");
        }
        Node* temp = top;
        T val = temp->data;
        top = top->next;
        delete temp;
        size--;
        return val;
    }
    
    // 获取栈顶元素
    T peek() const {
        if (isEmpty()) {
            throw runtime_error("栈为空，无法获取栈顶元素");
        }
        return top->data;
    }
    
    // 检查栈是否为空
    bool isEmpty() const {
        return top == nullptr;
    }
    
    // 获取栈的大小
    int getSize() const {
        return size;
    }
};

// 获取运算符优先级
int getPriority(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '(':
            return 0;
        default:
            return -1;
    }
}

// 执行运算
double calculate(double num1, double num2, char op) {
    switch (op) {
        case '+':
            return num1 + num2;
        case '-':
            return num1 - num2;
        case '*':
            return num1 * num2;
        case '/':
            if (num2 == 0) {
                throw runtime_error("除数不能为零");
            }
            return num1 / num2;
        default:
            throw runtime_error("无效的运算符");
    }
}

// 字符串计算器主函数
double stringCalculator(const string& expr) {
    Stack<double> numStack;  // 存储数字的栈
    Stack<char> opStack;     // 存储运算符的栈
    
    int i = 0;
    int n = expr.length();
    
    while (i < n) {
        // 跳过空格
        if (isspace(expr[i])) {
            i++;
            continue;
        }
        
        // 处理数字（包括整数、小数）
        if (isdigit(expr[i]) || expr[i] == '.') {
            double num = 0;
            // 处理整数部分
            while (i < n && isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }
            
            // 处理小数部分
            if (i < n && expr[i] == '.') {
                i++;
                double fraction = 0.1;
                while (i < n && isdigit(expr[i])) {
                    num += (expr[i] - '0') * fraction;
                    fraction *= 0.1;
                    i++;
                }
            }
            
            numStack.push(num);
        }
        // 处理左括号
        else if (expr[i] == '(') {
            opStack.push(expr[i]);
            i++;
        }
        // 处理右括号
        else if (expr[i] == ')') {
            // 计算到对应的左括号
            while (!opStack.isEmpty() && opStack.peek() != '(') {
                char op = opStack.pop();
                double num2 = numStack.pop();
                double num1 = numStack.pop();
                numStack.push(calculate(num1, num2, op));
            }
            
            if (opStack.isEmpty()) {
                throw runtime_error("括号不匹配");
            }
            
            opStack.pop();  // 弹出左括号
            i++;
        }
        // 处理运算符
        else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            // 处理表达式开头的正负号
            if (numStack.isEmpty() && expr[i] == '-') {
                numStack.push(0);
            }
            
            // 当前运算符优先级小于等于栈顶运算符优先级时，先计算栈顶运算符
            while (!opStack.isEmpty() && getPriority(opStack.peek()) >= getPriority(expr[i])) {
                char op = opStack.pop();
                double num2 = numStack.pop();
                double num1 = numStack.pop();
                numStack.push(calculate(num1, num2, op));
            }
            
            opStack.push(expr[i]);
            i++;
        }
        // 无效字符
        else {
            throw runtime_error("无效的字符: " + string(1, expr[i]));
        }
    }
    
    // 处理剩余的运算符
    while (!opStack.isEmpty()) {
        char op = opStack.pop();
        if (op == '(') {
            throw runtime_error("括号不匹配");
        }
        double num2 = numStack.pop();
        double num1 = numStack.pop();
        numStack.push(calculate(num1, num2, op));
    }
    
    if (numStack.getSize() != 1) {
        throw runtime_error("表达式无效");
    }
    
    return numStack.pop();
}

// 测试案例
void testCalculator() {
    struct TestCase {
        string expression;
        bool expectedValid;
        double expectedResult;
    };
    
    TestCase testCases[] = {
        {"3 + 4 * 2 / (1 - 5)", true, 3 + 4*2/(1-5)},
        {"10 + 20 * 30", true, 10 + 20*30},
        {"(10 + 20) * 30", true, (10 + 20)*30},
        {"100 / 25 + 7", true, 100/25 + 7},
        {"5.5 + 3.2 * 2", true, 5.5 + 3.2*2},
        {"10 - (2 + 3) * 2", true, 10 - (2+3)*2},
        {"10 / 0", false, 0},          // 除零错误
        {"(1 + 2", false, 0},          // 括号不匹配
        {"1 + * 2", false, 0},         // 无效表达式
        {"100 - 25 * 2 + 30", true, 100 - 25*2 + 30},
        {"-5 + 10", true, -5 + 10},    // 负数测试
        {"2 * (-3 + 5)", true, 2*(-3 + 5)}
    };
    
    int numTests = sizeof(testCases) / sizeof(TestCase);
    
    for (int i = 0; i < numTests; i++) {
        cout << "测试案例 " << i + 1 << ": " << testCases[i].expression << endl;
        try {
            double result = stringCalculator(testCases[i].expression);
            if (testCases[i].expectedValid) {
                cout << "  计算结果: " << result << endl;
                cout << "  预期结果: " << testCases[i].expectedResult << endl;
                if (abs(result - testCases[i].expectedResult) < 1e-6) {
                    cout << "  测试通过" << endl;
                } else {
                    cout << "  测试失败" << endl;
                }
            } else {
                cout << "  预期无效，但计算出结果: " << result << "，测试失败" << endl;
            }
        } catch (const exception& e) {
            if (testCases[i].expectedValid) {
                cout << "  预期有效，但发生错误: " << e.what() << "，测试失败" << endl;
            } else {
                cout << "  捕获到预期错误: " << e.what() << "，测试通过" << endl;
            }
        }
        cout << endl;
    }
}

int main() {
    cout << "=== 基于栈的字符串计算器 ===" << endl << endl;
    
    // 运行测试案例
    testCalculator();
    
    // 交互式计算
    string expr;
    cout << "请输入表达式(输入q退出): ";
    while (getline(cin, expr)) {
        if (expr == "q" || expr == "Q") {
            break;
        }
        
        try {
            double result = stringCalculator(expr);
            cout << "计算结果: " << result << endl;
        } catch (const exception& e) {
            cout << "错误: " << e.what() << endl;
        }
        
        cout << "请输入表达式(输入q退出): ";
    }
    
    return 0;
}
    