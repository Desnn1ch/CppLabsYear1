#pragma once;

#include "string"
#include "vector"
#include "fstream"
#include "iostream"
#include "cstdint"

const size_t kBufferSizeInput = 30;
const size_t kBufferSizeOutput = 31;


class Archive {
 public:
  Archive();
  Archive(const std::string& path);
  void AddFileToList(const std::string& file);
  void AddFilesToList(const std::vector<std::string>& files);
  void AddFileToArchive(const std::string& file);
  void Create();
  void WriteFileList();
  void Extract(const std::string& file);
  void ExtractAll();
  void DeleteFile(const std::string& file_name);
  void Concatenate(const std::string& name_arch1, const std::string& name_arch2);


 private:
  std::string name_;
  std::vector<std::string> files_;
};
