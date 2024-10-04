#include <lib/InvertIndex.cpp>
#include <lib/ParseFiles.cpp>
#include <lib/Trie.cpp>
#include <gtest/gtest.h>

static std::string KYourTestFolder = "";
static std::string KYourFolderToSaveIndex = "";

TEST(ParseFilesTest, LoadTrieFromFileTest) {
    InvertIndex invertIndex;
    invertIndex.SetFolderPath(KYourTestFolder);
    invertIndex.ParseFiles();
    invertIndex.SaveInvertIndexToFolder(KYourTestFolder);

    Trie trie;
    trie.LoadTrieFromFile(KYourFolderToSaveIndex + "invert_index.txt");
    EXPECT_EQ(trie.getPositions("1452124525212152141").empty(), true);
    EXPECT_EQ(trie.getPositions("push").empty(), false);
}

TEST(ParseFilesTest, ParseFilesTest) {
    ParseFiles parseFiles;
    parseFiles.SetFolderPath(KYourFolderToSaveIndex);
    auto v = parseFiles.GetResult("(vect OR include)");
    EXPECT_EQ(v.empty(), false);
}
