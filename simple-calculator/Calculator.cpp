// by zhuruikang
#include "Calculator.h"
#include <cmath>
#include <stack>
#include <set>
#include <map>
#include <iomanip>// 加入setprecision
#include "Tokenizer.h"
using namespace std;
const double eps = 1e-9;

bool math_error = false;
vector<Token> a;
stack<double> sta_x;
stack<char> sta_op;
// set<char> binary_op_set = {'+', '-', '*', '/', '%', '^', '&'};
// 二元运算符集合未使用，先注释掉，备用
map<char, int> lev_in = {
    {'=', 0},
    {'+', 3},
    {'-', 3},
    {'*', 5}, 
    {'/', 5}, 
    {'%', 5},
    {'(', 1},
    {')', 8},
    {'^', 7}, 
    {'&', 7}
}, lev_out = {
    {'=', 0},
    {'+', 2},
    {'-', 2},
    {'*', 4},
    {'/', 4},
    {'%', 4},
    {'(', 8},
    {')', 1},
    {'^', 6}, 
    {'&', 6}
};
void make_vector(string &s) {
    math_error = false;                       // 清除上一轮错误
    while (!sta_x.empty()) sta_x.pop();       // 清空操作数栈
    while (!sta_op.empty()) sta_op.pop();     // 清空运算符栈

    a = tokenizer(s);
	tool(a);
}
void print_vec_a() {
    for (auto &[op, x, tp] : a) {
        if (tp == 0) cout << x << " ";
        else cout << op << " ";
    } cout << endl;
}
double calculate(double x, char op, double y) {
    double ret = 0;
    if (op == '+') ret = x + y;
    else if (op == '-') ret = x - y;
    else if (op == '*') ret = x * y;
    else if (op == '/') {
        if (fabs(y) < eps) {// y == 0的规避浮点数精度问题写法
            math_error = true;
        }
        else ret = x / y;
    }
    else if (op == '%') {
        if (fabs(y) < eps) {// y == 0的规避浮点数精度问题写法
            math_error = true;
        }
        else ret = (int)x % (int)y;
    }
    else if (op == '^') ret = pow(x, y);
    else if (op == '&') ret = pow(x, 1 / y);
    // 这里不用额外错误检测了，经测试，pow函数可以自动处理错误情形（例如-2^2.5），并返回nan，即not a number
    else {
        cerr << "滚木" << endl;
    }
    return ret;
}
void solve() {
    for (auto &[op, x, tp] : a) {
        if (tp == 0) sta_x.push(x);// 数字栈压入数
        else {// 符号栈进行具体判断
            while (!sta_op.empty() && lev_in[sta_op.top()] > lev_out[op]) {// 先判断负号栈非空才能进行负号优先级比较，防止空指针错误
                // 无需写 if (binary_op_set.count(sta_op.top()))
                // 因为符号栈顶必然是一个二元运算符，否则在人为优先级规定下，必然不满足 lev_in[sta_op.top()] > lev_out[op]
                // （这里面没有判断数字栈里面是否有两个数，因为如果前面排除干净了语法错误，里面保证一定有2个数的，我认为）
                double top[2];
                for (int i = 1; i >= 0; --i) {// 注意要倒序，因为栈是倒着弹出，但是要还原正序的输入顺序
                    top[i] = sta_x.top();
                    sta_x.pop();
                }
                double calculated_ans = calculate(top[0], sta_op.top(), top[1]);
                if (math_error) {
                    cout << "数学错误：表达式中含有0作为除数或者模数，程序自动结束" << endl;
                    return;
                }
                sta_x.push(calculated_ans);
                sta_op.pop();
            }
            if (!sta_op.empty() && lev_in[sta_op.top()] == lev_out[op]) {// 左右括号相遇
                sta_op.pop();// 弹出左括号，右括号忽略
            }
            else {// 并非左右括号相遇，正常的变成正确优先级顺序，或者空负号栈
                sta_op.push(op);
            }
        }
    }
    // 这样处理完之后，sta_x会剩一个答案，sta_op剩一个等号。
    // 如果表达式里面不写等于号，会导致差一步算不出正确结果。请语法错误检查者务必认真检查
    //cout << sta_x.top() << endl;// 输出计算结果
	cout << setprecision(15) << sta_x.top() << endl;// 输出计算结果，精度15位
}
