#ifndef TEXT_STATISTICS_H
#define TEXT_STATISTICS_H

#include <array>
#include <cstdint>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

/*
文本读取与字符频度统计模块说明

需要输入：
const std::string& file_path：待统计文件的路径

输出：
huffman::TextStats& statistics：由函数填写的统计结果
statistics.bytes：std::vector<unsigned char>，按原顺序保存原文字节
statistics.frequency：std::array<std::uint64_t, 256>
statistics.frequency[i]：字节i的出现次数，未出现的字节为0
std::string& error_message：由函数填写的错误信息

调用方式：
huffman::read_and_count(file_path, statistics, error_message);

返回结果：
true：读取成功，error_message为空
false：打开或读取失败，statistics清空，error_message保存失败原因

补充：
按字节统计，空格、回车、换行均保留并计数
空文件也返回true，bytes为空，全部频度为0
每次调用重新统计，不累加之前的结果
*/

namespace huffman {

// 保存原文字节及各字节出现次数。
struct TextStats {

    std::vector<unsigned char> bytes;

    std::array<std::uint64_t, 256> frequency{};
};

// 成功返回 true；失败清空统计并设置错误信息。空文件也可成功读取。
inline bool read_and_count(const std::string& file_path,
                           TextStats& statistics,
                           std::string& error_message) {

    statistics = TextStats{};
    error_message.clear();

    // 保留原始字节，包括空格与换行。
    std::ifstream input_file(file_path, std::ios::binary);
    if (!input_file) {
        error_message = "无法打开输入文件：" + file_path;
        return false;
    }

    // 完整读取成功后才交付临时结果。
    TextStats temp;
    char cur_char;

    while (input_file.get(cur_char)) {

        // 使用无符号字节作为数组下标。
        const unsigned char byte_value =
            static_cast<unsigned char>(cur_char);

        temp.bytes.push_back(byte_value);
        ++temp.frequency[byte_value];
    }

    if (input_file.bad() || (!input_file.eof() && input_file.fail())) {
        error_message = "读取输入文件失败：" + file_path;
        return false;
    }

    statistics = std::move(temp);
    return true;

}

}

#endif
