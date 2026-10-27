//by yuzeming
#include <iostream>
#include <string>
#include "Calculator.h"
#include "Correction.h"

void run_cli() {
    std::cout << "**************************************** 欢迎使用 calculator1.0****************************************" << std::endl;
    std::cout << "使用说明：" << std::endl;
    std::cout << "表达式可包含：" << std::endl;
    std::cout << "1. 加（+）、减（-）、乘（*）、除（/）、求模（%）、开方（&）和乘方（^） 运算符" << std::endl;
    std::cout << "2. 括号" << std::endl;
    std::cout << std::endl;

    std::string expr;
    char choice = 'Y';

    while (choice == 'Y' || choice == 'y') {
        std::cout << "请输入表达式<以等号“=”结束>：" << std::endl;

        std::getline(std::cin, expr);

        if (check(expr)) {
            make_vector(expr);
             solve();
            }

        std::cout << "是否继续<Y/N>?";
        std::cin >> choice;

        std::cin.ignore(10000, '\n');
        std::cout << std::endl;
    }
}

