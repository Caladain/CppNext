#pragma once
#include <cxxopts.hpp>
#include "lexer/lexer.h"
#include "token/token.h"
#include <filesystem>
#include <vector>
#include <string>
#include <memory>
#include <tuple>

#include "fmt/os.h"
//-s N:\Projects\CppNext\compiler\tests\parser -r -o  N:\Projects\CppNext\build --parserdebug --lexerdebugtokens
namespace cppnext
{
    class Parser {
    public:
        Parser(Lexer* lexer);
        void Parse();
    private:
        Lexer* lexer{ nullptr };
    };
}