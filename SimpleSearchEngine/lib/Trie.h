#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

class TrieNode {
 public:
    std::unordered_map<char, TrieNode> children;
    std::unordered_map<uint32_t, std::vector<size_t>> positions;
    TrieNode() = default;
};

class Trie {
 private:
    TrieNode root_;
    size_t max_deep_word_;

    void SaveTrieNodeToFile(const TrieNode& node, std::ofstream& file) const;
    void LoadTrieNodeFromFile(TrieNode& node, std::ifstream& file);
    void LoadTrieNodeFromFile(TrieNode& node, std::ifstream& file, const std::vector<std::string>& words, int16_t index);

 public:
    Trie();

    void SetMaxDeep(size_t new_deep);
    void insert(const std::string& word, size_t pathId, size_t position);
    std::unordered_map<uint32_t, std::vector<size_t>> getPositions(const std::string& word) const;
    void SaveTrieToFile(const std::string& filename) const;
    void LoadTrieFromFile(const std::string& filename);
    void LoadTrieFromFile(const std::string& filename, const std::vector<std::string>& words);
};
