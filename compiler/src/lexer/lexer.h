#include <cxxopts.hpp>
#include "token/token.h"
#include <filesystem>
#include <vector>
#include <string>
#include <memory>

namespace cppnext::lexer {

    
    struct lexedFile
    {
        std::filesystem::path originalPath;
        std::vector<std::string> originalLines;
        std::vector<cppnext::token::Token> tokens;
    };
        
    class Lexer {
    public:
        Lexer();
        ~Lexer();
        Lexer(const Lexer&) = delete;
        Lexer& operator= (const Lexer&) = delete;
        void Lex(const std::vector<std::string>& rawCommandLineFilePaths, const cxxopts::ParseResult& commandLineOptions);
        std::vector<lexedFile>* GetLexedFiles();
    private:
        void ProcessFilePaths(const std::vector<std::string>& rawCommandLineFilePaths, const cxxopts::ParseResult& commandLineOptions);
        void LexFile(lexedFile& fileToLex, const cxxopts::ParseResult& commandLineOptions);
        void LexLine(int32_t lineNumber, std::vector<cppnext::token::Token>& tokenStream, const std::string& lineToLex, const cxxopts::ParseResult& commandLineOptions);
        std::unique_ptr<std::vector<lexedFile>> lexedFiles =  nullptr;
    };
}