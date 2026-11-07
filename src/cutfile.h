// SPDX-FileCopyrightText: 2026 hooolyshell
// SPDX-License-Identifier: MIT

#pragma once

#include "json.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

namespace hooolyshell {
// using & namespace
using std::cerr;
using std::cout;
using json = nlohmann::json;
using std::endl;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::string;

namespace fs = std::filesystem;

// class CutFile
class CutFile {
  private:
    // const values
    const int FILE_NOT_FOUND = -1;
    const int NOT_IS_OK_JSON = -2;
    const int NOT_CORRECT_PATH = -3;
    const int NOT_ALLOWED_N = -4;
    const int FAILED_LOADING_FILE = -5;
    const int OK = 0;

    json settings;

  public:
    int Cut(fs::path &path, const int partSize, fs::path &outDir);
    int Merge(fs::path &path);
    int ReadSettings(fs::path &path);
};
} // namespace hooolyshell