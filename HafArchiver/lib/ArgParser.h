#pragma once
#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <string_view>

namespace ArgumentParser {

class ArgParser {
 private:
  std::string name;
  size_t size = 0;

  template<typename T>
  class Argument {
   public:
    bool is_positional = false;
    bool is_multivalue = false;
    bool is_default = false;
    size_t min_args = 1;
    T def;

    std::vector<std::string> storage_key;
    std::vector<T> unique_storage;

    T* storage_value = nullptr;
    std::vector<T>* storage_values = nullptr;
   public:
    std::string description_arg;

    Argument(std::string long_key = "", std::string short_key = "") : storage_key {long_key, short_key} {};

    Argument& StoreValue(T& value_) {
      storage_value = &value_;
      return *this;
    }

    Argument& StoreValues(std::vector<T>& value_) {
      storage_values = &value_;
      return *this;
    }

    Argument& MultiValue(size_t min_args_ = 1) {
      is_multivalue = true;
      min_args = min_args_;
      return *this;
    }

    Argument& Positional() {
      is_positional = true;
      return *this;
    }

    bool IsPositional() {
      return is_positional;
    }

    void Default(T value) {
      is_default = true;
      def = value;
      if (storage_value != nullptr) {
        *storage_value = def;
      }
    }

    bool IsDefault() {
      return is_default;
    }

    T GetDefault() {
      return def;
    }

    void SetValue(T value) {
      if (storage_value != nullptr) {
        *storage_value = value;
      } else if (storage_values != nullptr) {
        (*storage_values).push_back(value);
      } else {
        unique_storage.push_back(value);
      }
    }

    T GetValue(int index) {
      if (storage_value != nullptr) {
        return *storage_value;
      } else if (storage_values != nullptr) {
        if ((*storage_values).size() - 1 >= index) {
          return (*storage_values)[index];
        } else {
          return def;
        }
      } else {
        if (unique_storage.size() - 1 >= index && !unique_storage.empty()) {
          return unique_storage[index];
        } else {
          return def;
        }
      }
    }

    bool Find(const std::string& key) {
      for (auto element : storage_key) {
        if (element == key) {
          return true;
        }
      }
      return false;
    }

    bool FindAnyValue() {
      if (storage_value != nullptr || storage_values != nullptr || !unique_storage.empty() || is_default) {
        if (is_multivalue && ((*storage_values).size() < min_args && unique_storage.size() < min_args) && !is_default) {
          return false;
        }
        return true;
      }
      return false;
    }

    std::string ArgumentDescription() {
      std::string result;

      if (storage_key.size() >= 2) {
        if (storage_key[0].size() == 1) {
          result += '-' + storage_key[0] + ",  ";
          result += "--" + storage_key[1] + ",  ";
        } else if (storage_key[1].size() == 1) {
          result += '-' + storage_key[1] + ",  ";
          result += "--" + storage_key[0] + ",  ";
        } else {
          result += "     --" + storage_key[0] + ",  ";
        }
      }

      result += description_arg + ' ';
      std::vector<std::string> info;
      if (is_multivalue || is_positional || min_args > 0) {
        if (is_multivalue) {
          info.push_back("repeated");
        }
        if (is_positional) {
          info.push_back("posititonal");
        }
        if (min_args >= 1) {
          info.push_back("min args = " + std::to_string(min_args));
        }
      }
      if (!info.empty()) {
        result += " [";
        for (int i = 0; i < info.size() - 1; ++i) {
          result += info[i] + ", ";
        }
        result += info[info.size()-1] + "]";
      }
      result += '\n';
      return result;
    }
  };

 public:
  std::vector<Argument<std::string>*> StringArguments;
  std::vector<Argument<int>*> IntArguments;
  std::vector<Argument<bool>*> FlagArguments;

  bool is_help = false;
  std::string short_help;
  std::string long_help;
  std::string description_help;

  ArgParser(std::string name_) : name(name_) {};
  ArgParser() : name("Parser") {};

  bool Parse(int argc, char** argv);

  bool Parse(const std::vector<std::string>& input);

  Argument<std::string>& AddStringArgument(const std::string& long_key, const std::string& info = "");

  Argument<std::string>& AddStringArgument(char short_key, const std::string& long_key, const std::string& info = "");

  std::string GetStringValue(const std::string& key, int index = 0);

  Argument<int>& AddIntArgument(const std::string& long_key, const std::string& info = "");

  Argument<int>& AddIntArgument(char short_key, const std::string& long_key, const std::string& info = "");

  int GetIntValue(const std::string& key, int index = 0);

  Argument<bool>& AddFlag(char short_key, const std::string& long_key, const std::string& info = "");

  Argument<bool>& AddFlag(const std::string& long_key, const std::string& info = "");

  bool GetFlag(const std::string& key, int index = 0);

  void AddHelp(char short_key, const std::string& long_key, const std::string& info = "");

  bool Help() const;

  std::string HelpDescription();

  ~ArgParser() {
    for (auto StringArgument : StringArguments) {
      delete StringArgument;
    }

    for (auto IntArgument : IntArguments) {
      delete IntArgument;
    }

    for (auto FlagArgument : FlagArguments) {
      delete FlagArgument;
    }
  }
};

}