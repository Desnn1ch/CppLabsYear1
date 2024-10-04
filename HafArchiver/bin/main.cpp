#include "lib/ArgParser.h"
#include "lib/Archive.h"

#include <iostream>

struct Options {
  bool create;
  bool list;
  bool extract;
  bool append;
  bool del;
  bool concatenate;

  std::string file;
  std::vector<std::string> files;
};

/* header archive - ....
 * header file
 * 1 байт - размер имени файла, n байтов - имя файла, 8 байт - размер файла
 * contain - ...
 */

int main(int argc, char** argv) {

  Options options;
  ArgumentParser::ArgParser parser("parser");
  parser.AddFlag('c', "create").StoreValue(options.create).Default(false);
  parser.AddFlag('a', "append").StoreValue(options.append).Default(false);
  parser.AddFlag('d', "delete").StoreValue(options.del).Default(false);
  parser.AddFlag('A', "concatenate").StoreValue(options.concatenate).Default(false);
  parser.AddFlag('x', "extract").StoreValue(options.extract).Default(false);
  parser.AddFlag('l', "list").StoreValue(options.list).Default(false);
  parser.AddStringArgument("files").StoreValues(options.files).MultiValue().Positional();
  parser.AddStringArgument('f', "file").StoreValue(options.file);

  parser.Parse(argc, argv);
  //parser.Parse({"hamarc", "--list", "-f=ARCHIVE","FILE1", "FILE2", "FILE3"});

  if (!options.file.empty() &&
  (options.list || options.extract || options.append || options.del || options.concatenate)) {
    Archive archive(options.file);
    if (options.list) {
      archive.WriteFileList();
    }
    if (options.extract) {
      if (options.files.empty()) {
        archive.ExtractAll();
      } else {
        for (int i = 0; i < options.files.size(); ++i) {
          archive.Extract(options.files[i]);
        }
      }
    }
    if (options.append) {
      for (int i = 0; i < options.files.size(); ++i) {
        archive.Extract(options.files[i]);
      }
    }
    if (options.del) {
      for (int i = 0; i < options.files.size(); ++i) {
        archive.DeleteFile(options.files[i]);
      }
    }
    if (options.concatenate) {
      archive.Concatenate(options.files[0], options.files[1]);
    }
  }

  if (options.create) {
    std::cout << options.files.size();
    Archive archive(options.file);
    for (const auto& name : options.files) {
      archive.AddFileToList(name);

    }
    archive.Create();
  }


  return 0;
}