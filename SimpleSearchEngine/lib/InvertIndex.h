#pragma once

#include "Trie.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

class InvertIndex {
private:
    inline static std::string KNameIndexFile = "invert_index.txt";
    inline static std::string KNamePathFile = "path_index.txt";
    // folder path
    std::string folderPath;
    // trie
    Trie trie;
    // index : path, int
    std::unordered_map<size_t, std::pair<std::string, size_t>> pathIndex;

public:
    void SetFolderPath(const std::string& folderPath_);

    void ParseFiles();

    void SaveInvertIndexToFolder(const std::string& folderPath_);
};
