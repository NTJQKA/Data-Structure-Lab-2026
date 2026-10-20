#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../Correction.h"

struct TestCase {
    std::string expression;
    bool expected;
};

int main() {
    const std::vector<TestCase> cases = {
        {"8-5=", true},
        {"-2*3^5+23/(45+67)-17%3/8&2=", true},
        {"  -(3.15 + 5.55) = \t", true},
        {"2*(-3)=", true},
        {"(-(-2))=", true},
        {"1 + 0.5=", true},
        {"((2+3)*4)=", true},
        {"8&3=", true},
        {"1/0=", true}, // 数学错误由求值阶段处理，语法本身正确。
        {"", false},
        {" \t ", false},
        {"=", false},
        {"1+2", false},
        {"1+2==", false},
        {"1=2", false},
        {"1+=", false},
        {"1+", false},
        {"()=", false},
        {"(1+)=", false},
        {"(1+2=", false},
        {"(1+2", false},
        {"1+2)=", false},
        {"1++2=", false},
        {"2*-3=", false},
        {"--2=", false},
        {"+2=", false},
        {"(+2)=", false},
        {"2 3=", false},
        {"2(3+4)=", false},
        {"(2)3=", false},
        {"(2)(3)=", false},
        {".5=", false},
        {"1.=", false},
        {"1.2.3=", false},
        {"abc=", false},
        {"1,2=", false},
        {std::string(400, '9') + "=", false},
    };

    std::size_t failures = 0;
    for (const auto& test : cases) {
        std::ostringstream diagnostic;
        auto* previous = std::cout.rdbuf(diagnostic.rdbuf());
        const bool actual = check(test.expression);
        std::cout.rdbuf(previous);

        const bool hasDiagnostic = !diagnostic.str().empty();
        if (actual != test.expected
            || (test.expected && hasDiagnostic)
            || (!test.expected && !hasDiagnostic)) {
            std::cerr << "FAIL: " << test.expression << '\n';
            ++failures;
        }
    }

    if (failures != 0) {
        std::cerr << failures << " syntax tests failed.\n";
        return 1;
    }
    std::cout << cases.size() << " syntax tests passed.\n";
    return 0;
}
