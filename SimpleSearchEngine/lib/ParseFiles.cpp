#include "ParseFiles.h"

std::unordered_set<size_t> ParseFiles::ParseWord(const std::string& word) {
    auto mp = trie_.getPositions(word);
    std::unordered_set<size_t> result;
    for (const auto& [file_name, positions] : mp) {
        result.insert(file_name);
    }
    return result;
}

void ParseFiles::Operation(std::unordered_set<size_t>& lhs, const std::unordered_set<size_t>& rhs, const char& op) {
    if (op == '+') {
        lhs.insert(rhs.begin(), rhs.end());
    } else if (op == '*') {
        std::unordered_set<size_t> intersection;
        for (const auto& element : lhs) {
            if (rhs.find(element) != rhs.end()) {
                intersection.insert(element);
            }
        }
        lhs = std::move(intersection);
    }
}

void ParseFiles::PrintError() const{
    throw std::runtime_error("Invalid input format");
}

void ParseFiles::LoadAllTrie(const std::string& path) {
    trie_.LoadTrieFromFile(path + KNameIndexFile);

    std::ifstream file(path + KNamePathFile);
    size_t index, count_words;
    std::string file_name;

    while (file >> index >> file_name >> count_words) {
        path_index_[index] = {file_name, count_words};
        number_of_documents_ = std::max(number_of_documents_, index);
        avgdl_ += count_words;
    }

    avgdl_ = avgdl_ / number_of_documents_;
    file.close();
}

void ParseFiles::LoadTrie(const std::string& path, std::vector<std::string>& words) {
    trie_.LoadTrieFromFile(path + "invert_index.txt", words);


    std::ifstream file(path + "path_index.txt");
    size_t index, count_words;
    std::string file_name;
    while (file >> index >> file_name >> count_words) {
        path_index_[index] = {file_name, count_words};
        number_of_documents_ = std::max(number_of_documents_, index);
        avgdl_ += count_words;
    }
    avgdl_ = avgdl_ / number_of_documents_;
    file.close();
}

void ParseFiles::SetFolderPath(const std::string& folder_path__) {
    folder_path_ = folder_path__;
}

/*
* OR = +
* AND = *
*/
std::unordered_set<size_t> ParseFiles::ParseRequest(const std::vector<std::string>& commands) {
    std::stack<std::unordered_set<size_t>> file_names;
    std::stack<char> operators;

    for (const auto& command : commands) {
        if (command == "(") {
            operators.push('(');
        } else if (command == ")") {
            while (!operators.empty() && operators.top() != '(') {
                auto rhs = file_names.top();
                file_names.pop();
                auto lhs = file_names.top();
                file_names.pop();
                Operation(lhs, rhs, operators.top());
                operators.pop();
                file_names.push(lhs);
            }
            operators.pop();
        } else if (command == "AND") {
            operators.push('*');
        } else if (command == "OR") {
            while (!operators.empty() && operators.top() == '*') {
                auto rhs = file_names.top();
                file_names.pop();
                auto lhs = file_names.top();
                file_names.pop();
                Operation(lhs, rhs, operators.top());
                operators.pop();
                file_names.push(lhs);
            }
            operators.push('+');
        } else {
            file_names.push(ParseWord(command));
        }
    }

    while (!operators.empty()) {
        auto rhs = file_names.top();
        file_names.pop();
        auto lhs = file_names.top();
        file_names.pop();
        Operation(lhs, rhs, operators.top());
        operators.pop();
        file_names.push(lhs);
    }

    return file_names.top();
}

std::vector<std::string> ParseFiles::ParseCommand(const std::string& command) {
    std::istringstream iss(command);
    std::vector<std::string> request;
    std::string s;
    int64_t brackets_counter = 0;
    bool is_prev_operator = true;
    while (iss >> s) {
        if (s == "OR" || s == "AND") {
            request.push_back(s);
            if (is_prev_operator) {
                PrintError();
            }
            is_prev_operator = true;
            continue;
        }

        while (s.front() == '(') {
            request.push_back("(");
            s.erase(s.begin());
            ++brackets_counter;
        }

        size_t counter = 0;
        while (s.back() == ')') {
            s.pop_back();
            ++counter;
            --brackets_counter;
        }

        if (!is_prev_operator) {
            PrintError();
        }

        is_prev_operator = false;
        toLowercase(s);
        request.push_back(s);

        while (counter > 0) {
            request.push_back(")");
            --counter;
        }
    }

    if (brackets_counter != 0) {
        PrintError();
    }

    return request;
}

std::vector<std::string> ParseFiles::GetResult(const std::string& command) {
    auto request = ParseCommand(command);
    std::vector<std::string> words;
    for (auto word : request) {
        if (word == "AND" || word == "OR" || word == "(" || word == ")") {
            continue;
        }
        words.push_back(word);
    }

    LoadTrie(folder_path_, words);

    auto files = ParseRequest(request);
    if (files.size() == 0) {
        std::cout << "Nothing was found matching your request" << '\n';
        return {};
    }

    std::vector<std::pair<double, size_t>> result;
    for (const auto& file_name : files) {
        result.emplace_back(0, file_name);
    }
    std::string s;
    for (const auto& word : words) {
        auto mp = trie_.getPositions(word);
        for (auto& [value, file] : result) {
            if (mp.find(file) != mp.end()) {
                value += ((double) mp[file].size() / ((double) mp[file].size() + (1 - (double) path_index_[file].second / (double) avgdl_))) *
                         log(((double) number_of_documents_ - (double) mp.size() + 0.5) / ((double) mp.size() + 0.5));
            }
        }
    }

    std::sort(result.begin(), result.end(), std::greater<std::pair<double, size_t>>());
    std::vector<std::string> suitable_files;
    for (const auto& [value, file_name] : result) {
        suitable_files.push_back(path_index_[file_name].first);
    }

    //"Word: file: pos1, pos2 ... ; file: pos1, pos2 ...; ..."э

    for (const auto& word : words) {
        auto table = trie_.getPositions(word);
        if (table.empty()) {
            continue;
        }
        std::cout << word << ":" << std::endl;
        for (const auto& [value, file_name] : result) {
            if (table[file_name].empty()) {
                continue;
            }
            std::cout << path_index_[file_name].first << ": ";
            for (size_t position : table[file_name]) {
                std::cout << position << " ";
            }
            std::cout << std::endl;
        }
    }
    return suitable_files;
}
