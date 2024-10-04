#include "InvertIndex.h"

void InvertIndex::SetFolderPath(const std::string& folderPath_) {
    folderPath = folderPath_;
}

void InvertIndex::ParseFiles() {
    size_t pathId = 1;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        std::ifstream file(entry.path());
        std::string line;
        size_t line_number = 0;
        size_t counter_words = 0;
        std::string word;
        char el;
        while (file.get(el)) {
            if (iswalnum(el)) {
                word += std::tolower(el);
            } else {
                if (!word.empty()) {
                    ++counter_words;
                    trie.insert(word, pathId, line_number);
                    word = "";
                }
                if (el == '\n' || el == '\r') {
                    ++line_number;
                }
            }
        }
        pathIndex[pathId] = {entry.path().string(), counter_words};
        ++pathId;
    }

}

void InvertIndex::SaveInvertIndexToFolder(const std::string& folderPath_) {
    trie.SaveTrieToFile(folderPath_ + KNameIndexFile);
    std::ofstream file(folderPath_ + KNamePathFile);
    for (const auto& [index, name] : pathIndex) {
        file << index << " " << name.first << " " << name.second << std::endl;
    }
    file.close();
}