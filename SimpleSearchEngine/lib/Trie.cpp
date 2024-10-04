#include "Trie.h"
#include <stdexcept>

Trie::Trie() : max_deep_word_(100) {}

void Trie::SetMaxDeep(size_t new_deep) {
    max_deep_word_ = new_deep;
}

void Trie::insert(const std::string& word, size_t pathId, size_t position) {
    if (word.size() > max_deep_word_) {
        return;
    }

    TrieNode* node = &root_;
    for (const auto& symbol : word) {
        node = &node->children[symbol];
    }
    node->positions[pathId].push_back(position);
}

std::unordered_map<uint32_t, std::vector<size_t>> Trie::getPositions(const std::string& word) const {
    const TrieNode* node = &root_;
    for (const auto& ch : word) {
        if (node->children.find(ch) == node->children.end()) {
            return {};
        }
        node = &node->children.at(ch);
    }
    return node->positions;
}

void Trie::SaveTrieToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for writing");
    }
    SaveTrieNodeToFile(root_, file);
}

void Trie::SaveTrieNodeToFile(const TrieNode& node, std::ofstream& file) const {
    file << node.children.size() << " " << node.positions.size() << " ";
    for (const auto& [fileName, positions] : node.positions) {
        file << fileName << " " << positions.size() << " ";
        for (size_t position : positions) {
            file << position << " ";
        }
    }
    file << std::endl;
    for (const auto& [symbol, child] : node.children) {
        file << symbol << " ";
        SaveTrieNodeToFile(child, file);
    }
}

void Trie::LoadTrieFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for reading");
    }
    LoadTrieNodeFromFile(root_, file);
}

void Trie::LoadTrieNodeFromFile(TrieNode& node, std::ifstream& file) {
    size_t num_child, count_files;
    file >> num_child >> count_files;

    for (size_t i = 0; i < count_files; ++i) {
        uint32_t file_name;
        size_t count_positions;
        file >> file_name >> count_positions;
        std::vector<size_t> positions(count_positions);
        for (size_t j = 0; j < count_positions; ++j) {
            file >> positions[j];
        }
        node.positions[file_name] = std::move(positions);
    }

    for (size_t i = 0; i < num_child; ++i) {
        char symbol;
        file >> symbol;
        LoadTrieNodeFromFile(node.children[symbol], file);
    }
}

void Trie::LoadTrieFromFile(const std::string& filename, const std::vector<std::string>& words) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file for reading");
    }
    LoadTrieNodeFromFile(root_, file, words, 0);
}

void Trie::LoadTrieNodeFromFile(TrieNode& node, std::ifstream& file, const std::vector<std::string>& words, int16_t index) {
    size_t num_child, count_files;
    file >> num_child >> count_files;

    for (size_t i = 0; i < count_files; ++i) {
        uint32_t file_name;
        size_t count_positions;
        file >> file_name >> count_positions;
        std::vector<size_t> positions(count_positions);
        for (size_t j = 0; j < count_positions; ++j) {
            file >> positions[j];
        }
        node.positions[file_name] = std::move(positions);
    }

    for (size_t i = 0; i < num_child; ++i) {
        char symbol;
        file >> symbol;
        bool is_skip = true;
        std::vector<std::string> new_words;

        for (const auto& word : words) {
            if (word.size() > index && symbol == word[index]) {
                is_skip = false;
                new_words.push_back(word);
            }
        }

        if (is_skip) {
            LoadTrieNodeFromFile(node.children[symbol], file, {}, index + 1);
        } else {
            LoadTrieNodeFromFile(node.children[symbol], file, new_words, index + 1);
        }
    }
}
