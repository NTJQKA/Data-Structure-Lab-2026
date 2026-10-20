#include <iostream>
#include <string>

#include "Calculator.h"
#include "Correction.h"

int main() {
    std::cout << "请输入表达式，以 = 结束：" << std::endl;

    std::string expression;
    if (!std::getline(std::cin, expression)) {
        std::cerr << "未读取到表达式。" << std::endl;
        return 1;
    }

    // 先检查原始输入，失败后不进入分词和求值，避免非法输入触发空栈访问。
    if (!check(expression)) {
        return 1;
    }

    make_vector(expression);
    print_vec_a();
    solve();
    return 0;
}
