#pragma once
#include <cxxopts.hpp>
#include "lexer/lexer.h"
#include "token/token.h"
#include <filesystem>
#include <vector>
#include <string>
#include <memory>
#include <tuple>
#include "parser/node.h"

#include "fmt/os.h"
//-s N:\Projects\CppNext\compiler\tests\parser -r -o  N:\Projects\CppNext\build --parserdebug
namespace cppnext
{
    class Parser {
    public:
        Parser(Lexer* lexer);
        void Parse(const cxxopts::ParseResult& commandLineOptions);
         std::unique_ptr<Node> ParseTokens(const std::vector<Token>& tokens, int32_t& position);
    private:
        Lexer* lexer{ nullptr };
        void ParseFile(const lexedFile& lexedFile, const cxxopts::ParseResult& commandLineOptions);
        void DebugParseOfFile(const lexedFile& lexedFile, const cxxopts::ParseResult& commandLineOptions);
        std::vector<Node> abstractSyntaxTree;
    };
}