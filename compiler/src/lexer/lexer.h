#pragma once
#include <cxxopts.hpp>
#include "token/token.h"
#include <filesystem>
#include <vector>
#include <string>
#include <memory>
#include <tuple>

#include "fmt/os.h"

namespace cppnext {

    
    struct lexedFile
    {
        std::filesystem::path originalPath;
        std::vector<std::string> originalLines;
        int32_t fileIndex{ 0 };
        std::vector<Token> tokens;
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
        void LexLine(int32_t fileIndex, int32_t lineNumber, std::vector<Token>& tokenStream, const std::string& lineToLex, const cxxopts::ParseResult& commandLineOptions);
        std::string FormatDebugToken(const Token& token, const cxxopts::ParseResult& commandLineOptions) const;
        std::string FormatToken(const Token& token, const cxxopts::ParseResult& commandLineOptions) const;
        bool IsIdentifierCharacter(const char character) const;
        bool IsNumericalCharacter(const char character) const;
        bool IsValidStartIdentifierCharacter(const char character) const;
        bool IsValidStartNumericalCharacter(const char character) const;
        bool IsCommentStartCharacter(const char character) const;
        void ConsumeComment(const char characterBeingEvaluated, const std::string& lineToLex, int32_t& positionInLine);
        std::string ConsumeNumerical(const char characterBeingEvaluated, const std::string& lineToLex, int32_t& positionInLine);
        std::string ConsumeIdentifier(const char characterBeingEvaluated, const std::string& lineToLex, int32_t& positionInLine);
        std::string ConsumeStringLiteral(const std::string& lineToLex, int32_t& positionInLine);
        std::string ConsumeCharacterLiteral(const std::string& lineToLex, int32_t& positionInLine);
        bool CreateTokenIfReservedSymbol(const char characterBeingEvaluated, const std::string& lineToLex, int32_t fileIndex, int32_t lineNumber, int32_t& positionInLine, std::vector<Token>& tokenStream);
        std::tuple<std::string, std::string> PrepareErrorMessageLine(const int32_t& fileIndex, const int32_t& lineNumber, const int32_t& linePosition) const;
        Token LexToken(int32_t fileIndex, int32_t lineNumber, int32_t linePosition, std::string value);
        Token LexToken(int32_t fileIndex, int32_t lineNumber, int32_t linePosition, tokenType type, std::string value);
        Token CreateToken(int32_t fileIndex, int32_t lineNumber, int32_t linePosition, tokenType type, std::string value);
        void HandleOpenCloseCounting(std::vector<Token>& container, int32_t& counter, const char OpeningSymbol, const char ClosingSymbol, const char currentSymbol, Token newToken);
        void ErrorAndPrintIfOpenCloseMismatch(const std::vector<Token>& container, const char OpeningSymbol, const char ClosingSymbol);
        std::unique_ptr<std::vector<lexedFile>> lexedFiles =  nullptr;
        std::vector<Token> parenContainer;
        int32_t parenCount{ 0 };
        std::vector<Token> braceContainer;
        int32_t braceCount{ 0 };
        std::vector<Token> bracketContainer;
        int32_t bracketCount{ 0 };
        bool inMultiLineComment = false;
    };
}