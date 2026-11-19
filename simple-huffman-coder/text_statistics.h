#ifndef TEXT_STATISTICS_H
#define TEXT_STATISTICS_H

#include <array>
#include <cstdint>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

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
