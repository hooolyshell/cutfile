// SPDX-FileCopyrightText: 2026 hooolyshell
// SPDX-License-Identifier: MIT

#include "cutfile.h"

int hooolyshell::CutFile::ReadSettings(const fs::path &path) {
    if (!fs::exists(path)) {
        cerr << "File not found!";
        return FILE_NOT_FOUND;
    }
    std::ifstream inFile(path);
    std::vector<char> buffer(2000);
    inFile.read(buffer.data(), 2000);
    if (!json::accept(buffer.data())) {
        cerr << "Not is correct json file!";
        return NOT_IS_OK_JSON;
    }
    settings = json::parse(buffer.data());
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
    j["sizePart"] = sizePart;
    outFile << j.dump(4);
    return OK;
}

int hooolyshell::CutFile::Merge(const fs::path &path) {
    if (!fs::exists(path)) {
        cerr << "ERR: Failed to load " << path.string() << "!" << endl;
        return FAILED_LOADING_FILE;
    }
    if (!fs::is_directory(path)) {
        cerr << "ERR: path must be a directory!" << endl;
        return NOT_CORRECT_PATH;
    }
    fs::path jsonPath = path / "merge.json";
    if (!exists(jsonPath)) {
        cerr << "ERR: " << jsonPath.string() << " not found!" << endl;
        return FAILED_LOADING_FILE;
    }
    ifstream readJson(jsonPath);
    if (!readJson.is_open()) {
        cerr << "ERR: " << jsonPath.string() << " can not be opened!" << endl;
        return FAILED_LOADING_FILE;
    }
    json merge;
    try {
        readJson >> merge;
    } catch (const json::parse_error &e) {
        cerr << "ERR: merge.json is invalid JSON!" << endl;
        return NOT_IS_OK_JSON;
    }
    if (!(merge.contains("name") && merge.contains("countPart") &&
          merge.contains("sizePart"))) {
        cerr << "merge.json incorrect!" << endl;
        return NOT_IS_OK_JSON;
    }
    int sP = merge["sizePart"], cP = merge["countPart"];
    fs::path outName = path / "__cutfile_merge_out" / merge["name"];
    std::error_code ec;
    fs::create_directory(path / "__cutfile_merge_out", ec);
    if (ec) {
        cerr << "ERR: Failed to create " << path.string()
             << "/__cutfile_merge_out!" << endl;
        return FAILED_LOADING_FILE;
    }
    if (fs::exists(outName))
        cerr << "WARN: " << outName.string() << " already exists." << endl;
    ofstream out(outName, std::ios_base::binary);
    if (!out.is_open()) {
        cerr << "Failed to open " << outName << "!" << endl;
        return FAILED_LOADING_FILE;
    }
    for (int i = 1; i <= cP; i++) {
        fs::path iName = path / std::to_string(i);
        ifstream in(iName, std::ios_base::binary);
        if (!in.is_open()) {
            cerr << "Failed to open " << iName.string() << "!";
            return FAILED_LOADING_FILE;
        }
        std::vector<char> b(sP);
        in.read(b.data(), sP);
        out.write(b.data(), in.gcount());
    }
    return OK;
}