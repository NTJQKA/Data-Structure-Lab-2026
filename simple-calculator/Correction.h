#pragma once

#include <string>

// 检查表达式语法；失败时输出错误位置和原因，不修改输入。
bool check(const std::string& expression);
