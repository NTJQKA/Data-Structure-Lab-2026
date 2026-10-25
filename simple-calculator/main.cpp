#include <iostream>
#include <string>
#include <windows.h>

#include "cli.h"

int main() {
    SetConsoleOutputCP(CP_UTF8); // 中文输出使用 UTF-8
    SetConsoleCP(CP_UTF8);       // 控制台输入使用 UTF-8

    run_cli();
    return 0;
}
