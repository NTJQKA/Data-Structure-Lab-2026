"""
需要输入：
B 编码后导出的 b_to_c.tsv。
文件使用 UTF-8 编码，三列用 Tab 分隔，首行为 byte_id、frequency、code。
byte_id 是十进制字节编号，frequency 是出现次数，code 是由 0 和 1 组成的字符串。

对外输出：
f1_code.txt：供 E 读取的码表，格式与输入相同。
终端显示理论压缩比，计算方法为有效编码位数除以原始位数。

调用顺序：
1. B 编码完成后，用 i、stats.frequency[i]、char_code[i] 导出三列数据。
2. 在本文件所在目录运行：
   python -X utf8 module.py --input b_to_c.tsv --output f1_code.txt
3. 退出码为 0 时继续执行 D，否则停止本轮。E 解码时跳过码表首行。

原文只有一种字节时，各模块统一使用码字 "0"。
"""

import argparse
import csv
from pathlib import Path

HEADER = ("byte_id", "frequency", "code")


def export_code_table(records, output_path):
    """接收 (字节编号, 频度, 01码字) 记录，写出码表并返回压缩比小数；空表返回 None。

    示例：export_code_table([(65, 7, "0"), (66, 3, "1")], "example_code.txt")
    比值为有效编码位数除以原始位数，不计码表、文件头、补齐位，也不读取 D 的文件。
    """
    rows = sorted(records)
    seen = set()
    original_bytes = encoded_bits = 0
    for byte_id, frequency, code in rows:
        if not 0 <= byte_id <= 255 or byte_id in seen or frequency <= 0:
            raise ValueError(f"字节编号 {byte_id} 越界、重复或频度不为正")
        if not code or any(bit not in "01" for bit in code):
            raise ValueError(f"字节 {byte_id} 的码字须为非空的 01 字符串")
        seen.add(byte_id)
        original_bytes += frequency
        encoded_bits += frequency * len(code)

    with open(output_path, "w", encoding="utf-8", newline="") as target:
        writer = csv.writer(target, delimiter="\t", lineterminator="\n")
        writer.writerow(HEADER)
        writer.writerows(rows)
    return encoded_bits / (8 * original_bytes) if original_bytes else None


def _read_b_table(path):
    with path.open(encoding="utf-8-sig", newline="") as source:
        reader = csv.reader(source, delimiter="\t")
        if next(reader, None) != list(HEADER):
            raise ValueError("表头须为 byte_id、frequency、code，列之间用 Tab 分隔")
        for row in reader:
            if row:
                byte_id, frequency, code = row
                yield int(byte_id), int(frequency), code


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="输出哈夫曼码表并计算理论压缩比")
    parser.add_argument("--input", type=Path, required=True, help="B 导出的三列 TSV")
    parser.add_argument("--output", type=Path, required=True, help="输出给 E 的三列 TXT")
    args = parser.parse_args()
    try:
        if args.input.resolve() == args.output.resolve():
            raise ValueError("输入与输出不能使用同一路径")
        ratio = export_code_table(_read_b_table(args.input), args.output)
    except (OSError, ValueError, csv.Error) as error:
        parser.exit(1, f"C 模块失败：{error}\n")
    print("原文为空，压缩比无定义" if ratio is None else f"理论压缩比：{ratio:.4%}")
