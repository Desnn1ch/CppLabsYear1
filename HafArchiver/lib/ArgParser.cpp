#include "ArgParser.h"
#include <utility>

using namespace ArgumentParser;

bool ArgParser::Parse(int argc, char **argv) {
  std::vector<std::string> arguments(argv, argv+argc);
  return Parse(arguments);
}

bool ArgParser::Parse(const std::vector<std::string>& arguments) {
  if (arguments.empty()) {
    return false;
  }
  int start = 0;
  if (arguments[0][0] != '-') {
    start = 1;
  }
  std::vector<std::string> positional;
  for (size_t i = start; i < arguments.size(); ++i)   {
    if (arguments[i][0] == '-') {
      int slide = 1;
      if (arguments[i][1] == '-') {
        slide++;
      }
      if (arguments[i].find('=') != std::string::npos) {
        std::string key = arguments[i].substr(slide, arguments[i].find('=') - slide);
        std::string value = arguments[i].substr(arguments[i].find('=') + 1, arguments[i].size());
        for (auto StringArgument : StringArguments) {
          if (StringArgument->Find(key)) {
            StringArgument->SetValue(value);
          }
        }
        for (auto IntArgument : IntArguments) {
          if (IntArgument->Find(key)) {
            IntArgument->SetValue(std::stoi(value));
          }
        }
        /*
        for (auto FlagArgument : FlagArguments) {
          if (FlagArgument->Find(key)) {
            FlagArgument->SetValue(true);
          }
        }*/
      }
      else {
        //check for help argument
        std::string key = arguments[i].substr(slide);
        //std::cout << 1231312 << " " << key << '\n';
        if (key == short_help || key == long_help) {
          is_help = true;
          return true;
        }
        for (int j = 1; j < arguments[i].size(); ++j) {
          key = std::string(1,arguments[i][j]);
          for (auto FlagArgument : FlagArguments) {
            if (FlagArgument->Find(key)) {
              FlagArgument->SetValue(true);
              break;
            }
          }
        }
      }

      for (auto FlagArgument : FlagArguments) {
        std::string key = arguments[i].substr(slide);
        if (FlagArgument->Find(key)) {
          FlagArgument->SetValue(true);
        }
      }
    }
    else {
      positional.push_back(arguments[i]);
    }
  }

  for (auto StringArgument : StringArguments) {
    if (StringArgument->IsPositional()) {
      for (auto element : positional) {
        StringArgument->SetValue(element);
      }
    }
    if (!(StringArgument->FindAnyValue())) {
      return false;
    }
  }
  for (auto IntArgument : IntArguments) {
    if (IntArgument->IsPositional()) {
      for (auto element : positional) {
        IntArgument->SetValue(std::stoi(element));
      }
    }
    if (!(IntArgument->FindAnyValue())) {
      return false;
    }
  }
  for (auto FlagArgument : FlagArguments) {
    if (FlagArgument->IsDefault() && (!FlagArgument->FindAnyValue())) {

      FlagArgument->SetValue(FlagArgument->GetDefault());
    }
    if (!(FlagArgument->FindAnyValue())) {
      return false;
    }
  }
  return true;
}

ArgParser::Argument<std::string>& ArgParser::AddStringArgument(const std::string& long_key, const std::string& info) {
  auto new_arg = new Argument<std::string>(long_key);
  new_arg->description_arg = info;
  StringArguments.push_back(new_arg);
  return *new_arg;
}

ArgParser::Argument<std::string> &ArgParser::AddStringArgument(char short_key, const std::string& long_key, const std::string& info) {
  std::string temp(1, short_key);
  auto new_arg = new Argument<std::string>(long_key, temp);
  new_arg->description_arg = info;
  StringArguments.push_back(new_arg);
  return *new_arg;
}

std::string ArgParser::GetStringValue(const std::string& key, int index) {
  for (auto StringArgument : StringArguments) {
    if (StringArgument->Find(key)) {
      return StringArgument->GetValue(index);
    }
  }
  return "";
}

ArgParser::Argument<int>& ArgParser::AddIntArgument(const std::string &long_key, const std::string& info) {
  auto new_arg = new Argument<int>(long_key);
  new_arg->description_arg = info;
  IntArguments.push_back(new_arg);
  return *new_arg;
}

ArgParser::Argument<int>& ArgParser::AddIntArgument(char short_key, const std::string& long_key, const std::string& info) {
  std::string temp(1, short_key);
  auto new_arg = new Argument<int>(long_key, temp);
  new_arg->description_arg = info;
  IntArguments.push_back(new_arg);
  return *new_arg;
}

int ArgParser::GetIntValue(const std::string &key, int index) {
  for (auto IntArgument : IntArguments) {
    if (IntArgument->Find(key)) {
      return IntArgument->GetValue(index);
    }
  }
  return 0;
}

ArgParser::Argument<bool>& ArgParser::AddFlag(char short_key, const std::string& long_key, const std::string& info) {
  std::string temp(1, short_key);
  auto new_arg = new Argument<bool>(temp, long_key);
  new_arg->description_arg = info;
  FlagArguments.push_back(new_arg);
  return *new_arg;
}

ArgParser::Argument<bool>& ArgParser::AddFlag(const std::string& long_key, const std::string& info) {
  auto new_arg = new Argument<bool>(long_key);
  new_arg->description_arg = info;
  FlagArguments.push_back(new_arg);
  return *new_arg;
}

bool ArgParser::GetFlag(const std::string &key, int index) {
  for (auto FlagArgument : FlagArguments) {
    if (FlagArgument->Find(key)) {
      return FlagArgument->GetValue(0);
    }
  }
  return false;
}

void ArgParser::AddHelp(char short_key, const std::string& long_key, const std::string& info) {
  short_help = std::string(1,short_key);
  long_help = long_key;
  description_help = info;
}

bool ArgParser::Help() const {
  return is_help;
}

std::string ArgParser::HelpDescription() {
  std::string result;
  result += name + '\n';
  result +=  description_help + '\n';
  result += '\n';

  if (!StringArguments.empty()) {
    for (auto StringArgument : StringArguments) {
      result += StringArgument->ArgumentDescription();
    }
  }
  if (!FlagArguments.empty()) {
    for (auto FlagArgument : FlagArguments) {
      result += FlagArgument->ArgumentDescription();
    }
  }
  if (!IntArguments.empty()) {
    for (auto IntArgument : IntArguments) {
      result += IntArgument->ArgumentDescription();
    }
  }

  result += '\n';
  if (!short_help.empty()) {
    result += '-' + short_help + ",  ";
  } else {
    result += "     ";
  }
  result += "--" + long_help + " Display this help and exit\n";
  return result;
}