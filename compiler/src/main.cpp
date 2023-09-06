#include "compiler/cppnCompiler.h"

#include <cxxopts.hpp>
#include "fmt/core.h"

#include <string>
#include <vector>
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    //Lets do the command line song and dance, then kick things off to the compiler.
    cxxopts::Options options("CppNext Compiler", "Experimental Compiler for the CppNext project.");
    options.add_options()
        ("h,help", "Print out all command line options")
        ("s,sourceFolder", "Set the source folder for relative paths in the Input Files", cxxopts::value<std::string>())
        ("r,recursive", "Ignore -f if -s exists, recursively dive down SourceFolder for all .cppn files.")
        ("f,files", "CppNext Input File(s)", cxxopts::value<std::vector<std::string>>())
        ("o,output", "CppNext Output Folder", cxxopts::value<std::string>())
        ("v,verbose", "Verbose Output", cxxopts::value<bool>()->default_value("false"))
        ("lexerdebug", "Output Lexer Debugging information")
        ("lexerdebugtokens", "Output Lexer Tokens for Debugging")
        ("parserdebug", "Output Parser Debugging information")
        ;
    options.parse_positional({ "files" });
    auto result = options.parse(argc, argv);
    if (result.count("help"))
    {
        fmt::print("{}\n", options.help());
        return 0;
    }
    
    //Pass the options to the compiler and kick things off
    auto compiler = std::make_unique<cppnext::cppnCompiler>();
    compiler->ProcessFiles(result);
    return 0;
}