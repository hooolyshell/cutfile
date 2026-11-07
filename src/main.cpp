// SPDX-FileCopyrightText: 2026 hooolyshell
// SPDX-License-Identifier: MIT

#include "cutfile.h"
#include <filesystem>
#include <iostream>

int main() {
    std::filesystem::path input = R"(D:\test.txt)";
    std::filesystem::path outDir = R"(D:\cut_output)";

    std::error_code ec;
    std::filesystem::create_directories(outDir, ec);
    if (ec) {
        std::cerr << "Failed to create output directory: " << ec.message()
                  << '\n';
        return 1;
    }

    hooolyshell::CutFile cutter;
    int result = cutter.Cut(input, 2, outDir);
    if (result != 0) {
        std::cerr << "Cut failed, error code: " << result << '\n';
        return 1;
    }

    std::cout << "Cut completed.\n";
    return 0;
}