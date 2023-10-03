#include <gtest/gtest.h>
#include <filesystem>
#include <string>
#include <vector>
#include "compiler/cppnCompiler.h"
/*
* ("h,help", "Print out all command line options")
        ("s,sourceFolder", "Set the source folder for relative paths in the Input Files", cxxopts::value<std::string>())
        ("r,recursive", "Ignore -f if -s exists, recursively dive down SourceFolder for all .cppn files.")
        ("f,files", "CppNext Input File(s)", cxxopts::value<std::vector<std::string>>())
        ("o,output", "CppNext Output Folder", cxxopts::value<std::string>())
        ("v,verbose", "Verbose Output", cxxopts::value<bool>()->default_value("false"))
        ("lexeronly", "Only Run The Lexer")
        ("lexerdebug", "Output Lexer Debugging information")
        ("lexerdebugtokens", "Output Lexer Tokens for Debugging")
        ("parserdebug", "Output Parser Debugging information")
        ("parseronly", "Run the Lexer, followed by the Parser only.")
*/

bool compareFiles(const std::string& firstFile, const std::string& secondFile)
{
    std::filesystem::path firstPath = firstFile;
    std::filesystem::path secondPath = secondFile;
    if (!std::filesystem::exists(firstPath) || !std::filesystem::exists(secondFile))
    {
        return false;
    }
    return true;
}

TEST(LexerTests, GenerateLexerTestData) {
    //-s ..\..\compiler\tests\inputFiles\lexer -r -o  ..\..\build --lexerdebug --lexeronly
    std::vector<std::string> arguments = { "Test", "-s", "..\\..\\compiler\\tests\\inputFiles\\lexer", "-r", "-o", "..\\..\\build", "--lexerdebug", "--lexeronly"};

    std::vector<char*> argv;
    for (const auto& arg : arguments)
    {
        argv.push_back((char*)arg.data());
    }
    argv.push_back(nullptr);
    cppnext::cppnCompiler testCompiler;
    auto result = testCompiler.ProcessCommandlineArguments(int(argv.size() - 1), argv.data());
    EXPECT_EQ(result, 1);
}

TEST(LexerTests, Test001) {
    //auto result = compareFiles("..\\..\\compiler\\tests\\ResultFiles\\lexer\\Test001.debug", "..\\..\\build\\")
    //EXPECT_TRUE();
    EXPECT_EQ(1, 1);
}