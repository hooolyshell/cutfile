// SPDX-FileCopyrightText: 2026 hooolyshell
// SPDX-License-Identifier: MIT

#include "cutfile.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

int main() {
    // 1. 定义路径
    fs::path input = R"(D:\test.txt)";
    fs::path outDir = R"(D:\cut_output)";

    // 2. 确保输出根目录存在
    std::error_code ec;
    fs::create_directories(outDir, ec);
    if (ec) {
        std::cerr << "Failed to create output directory: " << ec.message()
                  << '\n';
        return 1;
    }

    hooolyshell::CutFile cutter;

    // ==========================================
    // 3. 测试 Cut 功能
    // ==========================================
    std::cout << ">>> Starting Cut operation...\n";
    int cutResult = cutter.Cut(input, 2, outDir); // 每次切 2 字节
    if (cutResult != 0) {
        std::cerr << "Cut failed, error code: " << cutResult << '\n';
        return 1;
    }
    std::cout << "Cut completed successfully!\n\n";

    // ==========================================
    // 4. 测试 Merge 功能
    // ==========================================
    std::cout << ">>> Starting Merge operation...\n";

    // 注意：Merge 需要的路径是包含 merge.json 和分片文件的那个具体目录
    // 根据 Cut 的逻辑，这个目录是 outDir / input.filename()
    fs::path mergeSourceDir = outDir / input.filename();

    int mergeResult = cutter.Merge(mergeSourceDir);
    if (mergeResult != 0) {
        std::cerr << "Merge failed, error code: " << mergeResult << '\n';
        return 1;
    }

    std::cout << "Merge completed successfully!\n";
    std::cout
        << "Merged file saved at: "
        << (mergeSourceDir / "__cutfile_merge_out" / input.filename()).string()
        << '\n';

    return 0;
}