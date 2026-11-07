// SPDX-FileCopyrightText: 2026 hooolyshell
// SPDX-License-Identifier: MIT

#include "cutfile.h"

int hooolyshell::CutFile::ReadSettings(const fs::path &path) {
    if (!fs::exists(path)) {
        cerr << "File not found!";
        return FILE_NOT_FOUND;
    }
    std::ifstream inFile(path);
    string inStr;
    inFile >> inStr;
    if (!json::accept(inStr)) {
        cerr << "Not is correct json file!";
        return NOT_IS_OK_JSON;
    }
    settings = json::parse(inStr);
    inFile.close();
    return OK;
}

int hooolyshell::CutFile::Cut(const fs::path &file, const int sizePart,
                              fs::path &outDir) {
    if (!fs::exists(file)) {
        cerr << "ERR: File not found!" << endl;
        return FILE_NOT_FOUND;
    }
    if (fs::is_directory(file)) {
        cerr << "ERR: Can not be a directory!" << endl;
        return NOT_CORRECT_PATH;
    }
    if (!fs::is_directory(outDir)) {
        cerr << "ERR: outDir is not a directory!" << endl;
        return NOT_CORRECT_PATH;
    }
    if (sizePart <= 1) {
        cerr << "ERR: Size of each Part is not allowed(<=1)!" << endl;
        return NOT_ALLOWED_N;
    }

    ifstream inFile(file, std::ios_base::binary);
    if (!inFile.is_open()) {
        cerr << "ERR: Failed to open " << file.filename() << "!" << endl;
        return FAILED_LOADING_FILE;
    }

    fs::create_directory(outDir / file.filename());
    ofstream outFile(outDir / file.filename() / "merge.json");
    if (!outFile.is_open()) {
        cerr << "ERR: Failed to open merge.json!" << endl;
        return FAILED_LOADING_FILE;
    }

    std::vector<char> buffer(sizePart);
    int cntName = 1;

    while (inFile.read(buffer.data(), sizePart) || inFile.gcount() > 0) {
        if (inFile.gcount() == 0)
            break;
        ofstream o(outDir / file.filename() / std::to_string(cntName),
                   std::ios_base::binary);
        if (!o.is_open()) {
            cerr << "ERR: Failed to create each part!" << endl;
            return FAILED_LOADING_FILE;
        }
        o.write(buffer.data(), inFile.gcount());
        o.close();
        ++cntName;
    }

    json j;
    j["name"] = file.filename().string();
    j["countPart"] = cntName - 1;
    outFile << j.dump(4);
    return OK;
}