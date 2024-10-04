#pragma once

#include "Trie.h"

#include <unordered_set>
#include <stack>
#include <string>
#include <algorithm>
#include <cmath>
#include <iostream>

static void toLowercase(std::string& word) {
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
}

class ParseFiles {
private:
    inline static std::string KNameIndexFile = "invert_index.txt";
    inline static std::string KNamePathFile = "path_index.txt";

    Trie trie_;
    std::string folder_path_;
    std::unordered_map<size_t, std::pair<std::string, size_t>> path_index_;
    size_t number_of_documents_ = 0;
    size_t avgdl_ = 0;

    std::unordered_set<size_t> ParseWord(const std::string& word);

    void Operation(std::unordered_set<size_t>& lhs, const std::unordered_set<size_t>& rhs, const char& op);

    void PrintError() const;

public:
    void LoadAllTrie(const std::string& path);

    void LoadTrie(const std::string& path, std::vector<std::string>& words);

    void SetFolderPath(const std::string& folder_path__);

    std::unordered_set<size_t> ParseRequest(const std::vector<std::string>& commands);

    std::vector<std::string> ParseCommand(const std::string& command);

    std::vector<std::string> GetResult(const std::string& command);
};

