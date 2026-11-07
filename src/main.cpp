// SPDX-FileCopyrightText: 2026 hooolyshell
// SPDX-License-Identifier: MIT

#include <iostream>
namespace {
auto bigger = [](int a, int b) { return a > b; };
} // namespace
int main(void) {
    std::cout << "Hello, World!" << std::endl;
    std::cout << "call bigger(2,1):" << ::bigger(2, 1) << std::endl;
    std::cout << "Bye!" << std::endl;
    return 0;
}