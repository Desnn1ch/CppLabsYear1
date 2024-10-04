#include "lib\InvertIndex.h"
#include "lib\ParseFiles.h"
inline static std::string KYourTestFolder = "D:\\ITMOcourse\\itmo_last_lab\\for_test4";
inline static std::string KYourFolderToSaveIndex = "D:\\ITMOcourse\\itmo_last_lab\\";

int main() {
    /*
    InvertIndex invertIndex;
    invertIndex.SetFolderPath(KYourTestFolder);
    invertIndex.ParseFiles();
    invertIndex.SaveInvertIndexToFolder(KYourTestFolder);

    Trie trie;
    std::vector<std::string> words{"mo", "ma"};
    trie.LoadTrieFromFile(KYourFolderToSaveIndex + "invert_index.txt", words);
    auto t = trie.getPositions("1452124525212152141");
    std::cout << t.empty();
    t = trie.getPositions("push");
    std::cout << t.empty();
    */
    InvertIndex invertIndex;
    invertIndex.SetFolderPath(KYourTestFolder);
    //invertIndex.SetFolderPath("D:\\ITMOcourse\\itmo_last_lab\\for_test4");
    //invertIndex.SetFolderPath("...\\...\\for_test4");
    invertIndex.ParseFiles();
    //invertIndex.SaveInvertIndexToFolder("D:\\ITMOcourse\\itmo_last_lab\\");
    invertIndex.SaveInvertIndexToFolder(KYourFolderToSaveIndex);

    ParseFiles parseFiles;
    parseFiles.SetFolderPath(KYourFolderToSaveIndex);
    parseFiles.GetResult("(ma OR include)");

    return 0;
}