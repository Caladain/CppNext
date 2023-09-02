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
        ("f,files", "CppNext Input File(s)", cxxopts::value<std::vector<std::string>>())
        ("o,output", "CppNext Output Folder", cxxopts::value<std::string>())
        ("v,verbose", "Verbose Output", cxxopts::value<bool>()->default_value("false"))
        ;
    options.parse_positional({ "files" });
    auto result = options.parse(argc, argv);
    if (result.count("help"))
    {
        fmt::print("{}\n", options.help());
        return 0;
    }
    auto filesToProcess = result["files"].as<std::vector<std::string>>();

    //Pass the options to the compiler and kick things off
    auto compiler = std::make_unique<cppnext::compiler::cppnCompiler>();
    compiler->ProcessFiles(filesToProcess, result);
    return 0;
}