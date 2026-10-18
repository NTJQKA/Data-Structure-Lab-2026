#ifndef SYNTAX_CHECKER_H
#define SYNTAX_CHECKER_H

#include <cctype>
#include <cmath>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
 
inline bool check(const std::string& s) {
    const auto isDigit = [](char ch) -> bool {
        return ch >= '0' && ch <= '9';
    };

    const auto fail = [](std::size_t pos, const char* message) -> bool {
        std::cout << "语法检查未通过（第 " << pos + 1
                  << " 个字符）：" << message << std::endl;
        return false;
    };

    // 保存尚未匹配的左括号位置。
    std::stack<std::size_t> leftBrackets;

    bool needOperand = true; // 当前需要数字或左括号。
    bool allowMinus = true;  // 仅开头或左括号后允许一元负号。
    bool hasContent = false;
    std::size_t i = 0;

    while (i < s.size()) {
        const char ch = s[i];

        // 允许词元之间出现空白，但不会将两个数字合并。
        if (std::isspace(static_cast<unsigned char>(ch))) {
            ++i;
            continue;
        }

        // 等号必须位于完整表达式末尾，且只能出现一次。
        if (ch == '=') {
            if (!hasContent) {
                return fail(i, "表达式不能为空。");
            }
            if (needOperand) {
                return fail(i, "等号前缺少操作数。");
            }
            if (!leftBrackets.empty()) {
                return fail(leftBrackets.top(),
                            "这个左括号没有匹配的右括号。");
            }

            for (std::size_t j = i + 1; j < s.size(); ++j) {
                if (!std::isspace(static_cast<unsigned char>(s[j]))) {
                    return fail(j,
                                "等号只能出现一次，且后面只能有空白。");
                }
            }
            return true;
        }

        hasContent = true;

        // 检查整数、小数。
        if (isDigit(ch) || ch == '.') {
            if (!needOperand) {
                return fail(i,
                            "两个操作数之间缺少运算符，不能用空格拆开数字。");
            }

            const std::size_t start = i;
            bool hasDot = false;

            while (i < s.size() && (isDigit(s[i]) || s[i] == '.')) {
                if (s[i] == '.') {
                    if (hasDot) {
                        return fail(i, "一个数字只能有一个小数点。");
                    }
                    if (i == start || i + 1 >= s.size()
                        || !isDigit(s[i + 1])) {
                        return fail(i,
                                    "小数点前后必须有数字，例如 0.5。");
                    }
                    hasDot = true;
                }
                ++i;
            }

            // 提前检查转换范围，避免 tokenizer 的 stod 抛出异常。
            const std::string number = s.substr(start, i - start);

            try {
                std::size_t used = 0;
                const double value = std::stod(number, &used);

                if (used != number.size() || !std::isfinite(value)) {
                    return fail(start, "数字格式或数值范围不合法。");
                }
            } catch (const std::invalid_argument&) {
                return fail(start, "数字格式不合法。");
            } catch (const std::out_of_range&) {
                return fail(start, "数字超出了当前程序可转换的范围。");
            }

            needOperand = false;
            allowMinus = false;
            continue;
        }

        // 左括号前不能紧接数字或右括号。
        if (ch == '(') {
            if (!needOperand) {
                return fail(i,
                            "左括号前缺少运算符，例如应写 2*(3+4)。");
            }

            leftBrackets.push(i);
            needOperand = true;
            allowMinus = true;
            ++i;
            continue;
        }

        // 检查右括号匹配，以及括号内是否有完整表达式。
        if (ch == ')') {
            if (leftBrackets.empty()) {
                return fail(i, "这个右括号没有匹配的左括号。");
            }
            if (needOperand) {
                return fail(i,
                            "括号内为空，或右括号前缺少操作数。");
            }

            leftBrackets.pop();
            needOperand = false;
            allowMinus = false;
            ++i;
            continue;
        }

        // 检查运算符的位置。
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/'
            || ch == '%' || ch == '^' || ch == '&') {

            if (needOperand) {
                // 一元负号只允许出现在开头或左括号后。
                if (ch == '-' && allowMinus) {
                    allowMinus = false; // 防止 --2 等连续负号。
                    ++i;
                    continue;
                }

                // 当前队友的 tool() 只处理一元负号。
                if (ch == '+' && allowMinus) {
                    return fail(i,
                                "当前组版本不支持一元正号，请去掉这个 +。");
                }

                return fail(i,
                            "运算符前缺少操作数；运算符后的负数请加括号。");
            }

            needOperand = true;
            allowMinus = false;
            ++i;
            continue;
        }

        return fail(i,
                    "出现非法字符，请使用数字和英文半角运算符。");
    }

    if (!hasContent) {
        return fail(0, "表达式不能为空。");
    }
    if (!leftBrackets.empty()) {
        return fail(leftBrackets.top(),
                    "这个左括号没有匹配的右括号。");
    }
    if (needOperand) {
        return fail(s.size(), "表达式末尾缺少操作数。");
    }

    return fail(s.size(), "表达式必须以 = 结束。");
}

#endif
