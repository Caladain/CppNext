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
#include "parser/generalParser.h"

#include "fmt/os.h"
//-s N:\Projects\CppNext\compiler\tests\parser -r -o  N:\Projects\CppNext\build --parserdebug
//-s N:\Projects\CppNext\compiler\tests\parser -f SimpleNamespaceVariablesOnly.cppn -o  N:\Projects\CppNext\build --parserdebug --lexerdebug --lexerdebugtokens
namespace cppnext
{
    namespace parser
    {
        struct Node;
    }
    class Parser {
    public:
        Parser(Lexer* lexer);
        void Parse(const cxxopts::ParseResult& commandLineOptions);
         std::unique_ptr<parser::Node> ParseTokens(const std::vector<Token>& tokens, int32_t& position);
    private:
        Lexer* lexer{ nullptr };
        void ParseFile(const lexedFile& lexedFile, const cxxopts::ParseResult& commandLineOptions);        
        void DebugParseOfFile(const lexedFile& lexedFile, const cxxopts::ParseResult& commandLineOptions);
        std::vector<parser::Node> abstractSyntaxTree;
        GeneralParser generalParser;
    };
}