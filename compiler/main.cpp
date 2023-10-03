#include "compiler/cppnCompiler.h"

#include <cxxopts.hpp>
#include "fmt/core.h"

#include <string>
#include <vector>
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{   
    //Pass the options to the compiler and kick things off
    auto compiler = std::make_unique<cppnext::cppnCompiler>();    
    return compiler->ProcessCommandlineArguments(argc, argv);
}