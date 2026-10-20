import math
import subprocess
from pathlib import Path


# 判断字符是否为数字
def is_digit(ch: str) -> bool:
    return "0" <= ch <= "9"


# 输出语法错误
def fail(pos: int, message: str) -> bool:
    print(f"语法检查失败（第 {pos + 1} 个字符）：{message}")
    return False


# 检查表达式是否合法
def check(expression: str) -> bool:
    left_brackets = []  # 保存左括号的位置
    need_operand = True  # 当前是否需要操作数
    allow_minus = True  # 当前是否允许一元负号
    has_content = False  # 是否存在表达式内容
    i = 0
    length = len(expression)

    while i < length:
        ch = expression[i]

        # 1. 跳过空格
        if ch.isspace():
            i += 1
            continue

        # 2. 检查等号
        if ch == "=":
            if not has_content:
                return fail(i, "表达式不能为空")

            if need_operand:
                return fail(i, "等号前缺少操作数")

            if left_brackets:
                return fail(
                    left_brackets[-1],
                    "左括号没有匹配的右括号"
                )

            # 等号后只能有空格
            for j in range(i + 1, length):
                if not expression[j].isspace():
                    return fail(
                        j,
                        "等号只能出现一次，且必须位于表达式末尾"
                    )

            return True

        has_content = True

        # 3. 检查整数和小数
        if is_digit(ch) or ch == ".":
            if not need_operand:
                return fail(
                    i,
                    "两个操作数之间缺少运算符"
                )

            start = i
            has_dot = False

            # 读取完整数字
            while (
                i < length
                and (is_digit(expression[i]) or expression[i] == ".")
            ):
                if expression[i] == ".":
                    if has_dot:
                        return fail(i, "一个数字只能有一个小数点")

                    if (
                        i == start
                        or i + 1 >= length
                        or not is_digit(expression[i + 1])
                    ):
                        return fail(
                            i,
                            "小数点前后必须有数字，例如 0.5"
                        )

                    has_dot = True

                i += 1

            number = expression[start:i]

            # 检查数字能否转换为有效的浮点数
            try:
                value = float(number)

                if not math.isfinite(value):
                    return fail(
                        start,
                        "数字超出可处理范围"
                    )

            except (ValueError, OverflowError):
                return fail(start, "数字格式不合法")

            need_operand = False
            allow_minus = False
            continue

        # 4. 检查左括号
        if ch == "(":
            if not need_operand:
                return fail(
                    i,
                    "左括号前需要运算符，例如 2*(3+4)"
                )

            left_brackets.append(i)
            need_operand = True
            allow_minus = True

            i += 1
            continue

        # 5. 检查右括号
        if ch == ")":
            if not left_brackets:
                return fail(
                    i,
                    "右括号没有匹配的左括号"
                )

            if need_operand:
                return fail(
                    i,
                    "括号内为空或缺少操作数"
                )

            left_brackets.pop()
            need_operand = False
            allow_minus = False

            i += 1
            continue

        # 6. 检查运算符
        if ch in "+-*/%^&":
            if need_operand:

                # 只允许在表达式开头或左括号后使用负号
                if ch == "-" and allow_minus:
                    allow_minus = False
                    i += 1
                    continue

                # 不支持一元正号
                if ch == "+" and allow_minus:
                    return fail(
                        i,
                        "当前版本不支持一元正号"
                    )

                return fail(
                    i,
                    "运算符前缺少操作数，或运算符位置不合法"
                )

            need_operand = True
            allow_minus = False

            i += 1
            continue

        # 7. 非法字符
        return fail(
            i,
            "出现非法字符，请使用数字、括号和支持的运算符"
        )

    # 8. 检查表达式结束状态
    if not has_content:
        return fail(0, "表达式不能为空")

    if left_brackets:
        return fail(
            left_brackets[-1],
            "左括号没有匹配的右括号"
        )

    if need_operand:
        return fail(
            length,
            "表达式末尾缺少操作数"
        )

    return fail(
        length,
        "表达式必须以等号 = 结束"
    )


# 调用 C++ 计算器
def calculate(expression: str) -> None:
    # 查找与当前 Python 文件位于同一目录的 C++ 程序
    cpp_program = Path(__file__).resolve().with_name(
        "calculator.exe"
    )

    if not cpp_program.is_file():
        print("错误：找不到 calculator.exe")
        print("请先编译 C++ 计算器，并将生成的文件放在本目录。")
        return

    try:
        result = subprocess.run(
            [str(cpp_program)],
            input=expression + "\n",
            capture_output=True,
            text=True,
            encoding="utf-8",
            errors="replace",
            cwd=str(cpp_program.parent),
            timeout=10,
            check=False
        )

    except subprocess.TimeoutExpired:
        print("错误：计算程序运行超时")
        return

    except OSError as error:
        print(f"错误：无法启动 C++ 计算器：{error}")
        return

    # 检查 C++ 程序是否正常运行
    if result.returncode != 0:
        print("C++ 计算程序运行失败")

        if result.stderr.strip():
            print(result.stderr.strip())

        if result.stdout.strip():
            print(result.stdout.strip())

        return

    # C++ 可能输出 Token 调试信息，取最后一行
    lines = [
        line.strip()
        for line in result.stdout.splitlines()
        if line.strip()
    ]

    if not lines:
        print("错误：C++ 程序没有输出结果")
        return

    output = lines[-1]

    # 处理现有 C++ 程序的除零错误提示
    if "数学错误" in output or "错误" in output:
        print(output)
    else:
        print(f"计算结果：{output}")


# 主程序
def main():
    print("===== 表达式计算器 =====")
    print("支持的运算符：+ - * / % ^ &")
    print("表达式必须以 = 结束")
    print("输入 exit 退出程序")

    while True:
        expression = input("\n请输入表达式：")

        # 退出程序
        if expression.strip().lower() in {
            "exit", "quit", "q"
        }:
            print("程序已退出")
            break

        # 第一步：检查表达式语法
        if not check(expression):
            continue

        print("语法检查通过")

        # 第二步：调用 C++ 计算器
        calculate(expression)


if __name__ == "__main__":
    main()