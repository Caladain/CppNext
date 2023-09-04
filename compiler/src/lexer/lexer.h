#pragma once
#include <cxxopts.hpp>
#include "token/token.h"
#include <filesystem>
#include <vector>
#include <string>
#include <memory>
#include <tuple>

#include "fmt/os.h"

namespace cppnext::lexer {

    
    struct lexedFile
    {
        std::filesystem::path originalPath;
        std::vector<std::string> originalLines;
        int32_t fileIndex{ 0 };
        std::vector<cppnext::token::Token> tokens;
    };
        
    class Lexer {
    public:
        Lexer();
        ~Lexer();
        Lexer(const Lexer&) = delete;
        Lexer& operator= (const Lexer&) = delete;
        void Lex(const cxxopts::ParseResult& commandLineOptions);
        std::vector<lexedFile>* GetLexedFiles() const;
        void Print(const cxxopts::ParseResult& commandLineOptions) const;
    private:
        void ProcessFilePaths(const cxxopts::ParseResult& commandLineOptions);
        void LexFile(lexedFile& fileToLex, const cxxopts::ParseResult& commandLineOptions);
        void LexLine(int32_t fileIndex, int32_t lineNumber, std::vector<cppnext::token::Token>& tokenStream, const std::string& lineToLex, const cxxopts::ParseResult& commandLineOptions);
        void PrintToken(const cppnext::token::Token& token, const cxxopts::ParseResult& commandLineOptions, fmt::ostream& outputFile) const;
        bool IsIdentifierCharacter(const char character) const;
        bool IsNumericalCharacter(const char character) const;
        bool IsValidStartIdentifierCharacter(const char character) const;
        bool IsValidStartNumericalCharacter(const char character) const;
        std::string ConsumeNumerical(const char characterBeingEvaluated, const std::string& lineToLex, int32_t& positionInLine);
        std::string ConsumeIdentifier(const char characterBeingEvaluated, const std::string& lineToLex, int32_t& positionInLine);
        bool CreateTokenIfReservedSymbol(const char characterBeingEvaluated, const std::string& lineToLex, int32_t fileIndex, int32_t lineNumber, int32_t& positionInLine, std::vector<cppnext::token::Token>& tokenStream);
        std::tuple<std::string, std::string> PrepareErrorMessageLine(const int32_t& fileIndex, const int32_t& lineNumber, const int32_t& linePosition) const;
        cppnext::token::Token LexToken(int32_t fileIndex, int32_t lineNumber, int32_t linePosition, std::string value);
        cppnext::token::Token LexToken(int32_t fileIndex, int32_t lineNumber, int32_t linePosition, cppnext::token::tokenType type, std::string value);
        cppnext::token::Token CreateToken(int32_t fileIndex, int32_t lineNumber, int32_t linePosition, cppnext::token::tokenType type, std::string value);
        std::unique_ptr<std::vector<lexedFile>> lexedFiles =  nullptr;
    };
}