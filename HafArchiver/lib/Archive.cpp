#include "Archive.h"
#include "Hamming.h"

Archive::Archive() {
  name_ = "unknown.haf";
}

Archive::Archive(const std::string& path) {
  name_ = path;
}

void Archive::AddFileToList(const std::string &file) {
  files_.push_back(file);
}

void Archive::AddFilesToList(const std::vector<std::string> &files) {
  for (int i = 0; i < files.size(); ++i) {
    AddFileToList(files[i]);
  }
}


void Archive::AddFileToArchive(const std::string& file) {
  std::ofstream archive(name_, std::ios::binary | std::ios::app);

  if (!archive.is_open()) {
    std::cerr << "Unable to open archive: " << name_ << "\n";
    std::exit(EXIT_FAILURE);
  }

  std::ifstream new_file(file, std::ios::binary);
  if (!new_file.is_open()) {
    std::cerr << "Unable to open file: " << file << "\n";
    std::exit(EXIT_FAILURE);
  }

  new_file.seekg(0, std::ios::end);
  size_t file_size = static_cast<size_t>(new_file.tellg());
  new_file.seekg(0, std::ios::beg);

  uint8_t name_size = static_cast<uint8_t>(file.size());
  archive.write(reinterpret_cast<const char*>(&name_size), sizeof(uint8_t));
  archive.write(file.c_str(), file.size());

  size_t remain = file_size;
  file_size += file_size/kBufferSizeInput + 1;
  archive.write(reinterpret_cast<const char*>(&file_size), sizeof(file_size));

  std::vector<char> buffer(kBufferSizeInput);

  while (remain > 0) {
    buffer.resize(std::min(remain, static_cast<size_t>(kBufferSizeInput)));
    new_file.read(reinterpret_cast<char*>(buffer.data()), std::min(remain, static_cast<size_t>(kBufferSizeInput)));
    std::vector<char> ham = EncodeHam(buffer);
    archive.write(reinterpret_cast<const char*>(ham.data()), ham.size());
    remain -= std::min(remain, static_cast<size_t>(kBufferSizeInput));
  }


  new_file.close();
  archive.close();
}

void Archive::Create() {
  std::ofstream archive(name_, std::ios::binary);
  if (!archive.is_open()) {
    std::cerr << "Unable to create file\n";
    std::exit(EXIT_FAILURE);
  }

  for (const auto& file_name : files_) {
    std::ifstream opened_file(file_name, std::ios::binary);
    if (!opened_file.is_open()) {
      std::cerr << "Unable to open file: " << file_name << "\n";
      std::exit(EXIT_FAILURE);
    }

    opened_file.seekg(0, std::ios::end);
    size_t file_size = static_cast<size_t>(opened_file.tellg());
    opened_file.seekg(0, std::ios::beg);

    uint8_t name_size = static_cast<uint8_t>(file_name.size());
    archive.write(reinterpret_cast<const char*>(&name_size), sizeof(uint8_t));
    archive.write(file_name.c_str(), file_name.size());

    size_t remain = file_size;
    file_size += file_size / kBufferSizeInput + 1;
    archive.write(reinterpret_cast<const char*>(&file_size), sizeof(file_size));

    std::vector<char> buffer(kBufferSizeInput);

    while (remain > 0) {
      buffer.resize(std::min(remain, static_cast<size_t>(kBufferSizeInput)));
      opened_file.read(reinterpret_cast<char*>(buffer.data()), std::min(remain, kBufferSizeInput));
      std::vector<char> ham = EncodeHam(buffer);
      archive.write(reinterpret_cast<const char*>(ham.data()), ham.size());
      remain -= std::min(remain, static_cast<size_t>(kBufferSizeInput));
    }

    opened_file.close();
  }
  archive.close();
}

void Archive::WriteFileList() {
  std::ifstream archive(name_, std::ios::binary);

  if (!archive.is_open()) {
    std::cerr << "Unable to open archive\n";
    std::exit(EXIT_FAILURE);
  }

  uint8_t name_size;
  std::string file_name;
  size_t file_size;

  while (archive.read(reinterpret_cast<char*>(&name_size), sizeof(uint8_t)))  {
    file_name.resize(name_size);
    archive.read(&file_name[0], name_size);

    archive.read(reinterpret_cast<char*>(&file_size), sizeof(size_t));

    std::cout << "File Name: " << file_name << " File Size: " << file_size << '\n';
    archive.seekg(file_size, std::ios::cur);
  }
  archive.close();
}

void Archive::Extract(const std::string &file) {
  std::ifstream archive(name_, std::ios::binary);

  if (!archive.is_open()) {
    std::cerr << "Unable to open archive\n";
    std::exit(EXIT_FAILURE);
  }

  uint8_t name_size;
  std::string file_name;
  size_t file_size;

  while (archive.read(reinterpret_cast<char*>(&name_size), sizeof(uint8_t))) {
    file_name.resize(name_size);
    archive.read(&file_name[0], name_size);

    archive.read(reinterpret_cast<char*>(&file_size), sizeof(file_size));

    if (file_name == file) {
      std::ofstream extract_file("extracted_" + file_name, std::ios::binary);
      if (!extract_file.is_open()) {
        std::cerr << "Unable to open extracted file: " << file_name << "\n";
        continue;
      }

      std::vector<char> buffer;
      size_t remain = file_size;
      while (remain > 0) {
        size_t read_size = std::min(remain, kBufferSizeOutput);
        buffer.resize(read_size);
        archive.read(reinterpret_cast<char*>(buffer.data()), read_size);
        std::vector<char> unham = DecodeHam(buffer);

        extract_file.write(reinterpret_cast<char*>(unham.data()), unham.size());
        remain -= read_size;
      }
      extract_file.close();
      break;
    } else {
      archive.seekg(file_size, std::ios::cur);
    }
  }
  archive.close();
}

void Archive::ExtractAll() {
  std::ifstream archive(name_, std::ios::binary);

  if (!archive.is_open()) {
    std::cerr << "Unable to open archive\n";
    std::exit(EXIT_FAILURE);
  }

  uint8_t name_size;
  std::string file_name;
  size_t file_size;

  while (archive.read(reinterpret_cast<char*>(&name_size), sizeof(uint8_t))) {
    file_name.resize(name_size);
    archive.read(&file_name[0], name_size);

    archive.read(reinterpret_cast<char*>(&file_size), sizeof(file_size));

    std::ofstream extract_file("extracted_" + file_name, std::ios::binary);

    if (!extract_file.is_open()) {
      std::cerr << "Unable to open extracted file: " << file_name << "\n";
      continue;
    }

    std::vector<char> buffer;
    size_t remain = file_size;
    while (remain > 0) {
      size_t read_size = std::min(remain, kBufferSizeOutput);
      buffer.resize(read_size);
      archive.read(reinterpret_cast<char*>(buffer.data()), read_size);
      std::vector<char> unham = DecodeHam(buffer);

      extract_file.write(reinterpret_cast<char*>(unham.data()), unham.size());
      remain -= read_size;
    }
    extract_file.close();
    break;

  }
  archive.close();
}

void Archive::DeleteFile(const std::string& file_to_delete) {
  std::string temp_archive_name = "temp_archive.haf";
  std::ofstream temp_archive(temp_archive_name, std::ios::binary);
  if (!temp_archive.is_open()) {
    std::cerr << "Unable to create temp archive\n";
    std::exit(EXIT_FAILURE);
  }

  std::ifstream archive(name_, std::ios::binary);
  if (!archive.is_open()) {
    std::cerr << "Unable to open archive\n";
    std::exit(EXIT_FAILURE);
  }

  uint8_t name_size;
  std::string file_name;
  size_t file_size;

  while (archive.read(reinterpret_cast<char*>(&name_size), sizeof(uint8_t))) {
    file_name.resize(name_size);
    archive.read(&file_name[0], name_size);

    archive.read(reinterpret_cast<char*>(&file_size), sizeof(file_size));

    if (file_name != file_to_delete) {
      temp_archive.write(reinterpret_cast<char*>(&name_size),sizeof(uint8_t));
      temp_archive.write(file_name.c_str(), file_name.size());
      temp_archive.write(reinterpret_cast<char*>(&file_size), sizeof(file_size));

      char buffer[kBufferSizeOutput];
      size_t remain = file_size;
      while (remain > 0) {
        size_t read_size = std::min(remain, kBufferSizeOutput);
        archive.read(buffer, read_size);

        if (archive.fail()) {
          std::cerr << "Unable to read from archive\n";
          break;
        }

        temp_archive.write(buffer, read_size);
        remain -= read_size;
      }
    } else {
      archive.seekg(file_size, std::ios::cur);
    }
  }

  archive.close();
  temp_archive.close();

  std::remove(name_.c_str());
  std::rename(temp_archive_name.c_str(), name_.c_str());
}

void Archive::Concatenate(const std::string& name_arch1, const std::string& name_arch2) {
  std::ifstream arch1(name_arch1, std::ios::binary);
  std::ifstream arch2(name_arch2, std::ios::binary);

  std::ofstream concatenated(name_, std::ios::binary | std::ios::app);
  if (!arch2.is_open() || !arch1.is_open() || !concatenated.is_open()) {
    std::cerr << "Unable to open archive\n";
    std::exit(EXIT_FAILURE);
  }

  const size_t buffer_size = kBufferSizeInput;
  char buffer[buffer_size];

  while (!arch1.eof()) {
    arch1.read(buffer, buffer_size);
    concatenated.write(buffer, arch1.gcount());
  }

  while (!arch2.eof()) {
    arch2.read(buffer, buffer_size);
    concatenated.write(buffer, arch2.gcount());
  }

  arch1.close();
  arch2.close();
  concatenated.close();
}
