#include "lexer/lexer.h"

#include <cctype>
#include <fstream>

#include "fmt/core.h"

namespace cppnext::lexer {
    Lexer::Lexer()
    {
        lexedFiles = std::make_unique<std::vector<lexedFile>>();
    }

    Lexer::~Lexer()
    {
    }
    void Lexer::Lex(const std::vector<std::string>& rawCommandLineFilePaths, [[maybe_unused]] const cxxopts::ParseResult& commandLineOptions)
    {
        ProcessFilePaths(rawCommandLineFilePaths, commandLineOptions);
        for (auto& file : *lexedFiles)
        {
            LexFile(file, commandLineOptions);
        }
        Print(commandLineOptions);
    }

    void Lexer::ProcessFilePaths(const std::vector<std::string>& rawCommandLineFilePaths, const cxxopts::ParseResult& commandLineOptions)
    {
        std::filesystem::path prefix{};
        int32_t fileCount{ 0 };
        bool prefixUsed{ false };
        if (commandLineOptions.count("sourceFolder"))
        {
            prefix = commandLineOptions["sourceFolder"].as<std::string>();
            prefixUsed = true;
        }
        for (const auto& file : rawCommandLineFilePaths)
        {
            auto path = std::filesystem::path(file);
            std::filesystem::path prefixedPath = prefix / path;
            if (std::filesystem::exists(prefixedPath))
            {
                lexedFile fileToProcess;
                fileToProcess.originalPath = prefixedPath;
                fileToProcess.fileIndex = fileCount;
                fileCount++;
                lexedFiles->push_back(fileToProcess);
            }
            else
            {
                if (prefixUsed)
                {
                    fmt::print("Prefix: {}\nRawPath: {}\n", prefix.string(), path.string());
                }
                fmt::print("Error 0001: File {} Does not exist.\n", prefixedPath.string());
                throw 0001;
            }
        }
    }

    void Lexer::LexFile(lexedFile& fileToLex, const cxxopts::ParseResult& commandLineOptions)
    {
        std::fstream file;
        file.open(fileToLex.originalPath.string(), std::ios::in);
        if (file.is_open())
        {
            int32_t lineNumber{ 1 };
            std::string line;
            while (std::getline(file, line))
            {
                fileToLex.originalLines.push_back(line);
                LexLine(fileToLex.fileIndex, lineNumber, fileToLex.tokens, line, commandLineOptions);
                lineNumber++;
            }
            file.close();
        }
        
    }

    void Lexer::LexLine(int32_t fileIndex, int32_t lineNumber, std::vector<cppnext::token::Token>& tokenStream, [[maybe_unused]] const std::string& lineToLex, [[maybe_unused]] const cxxopts::ParseResult& commandLineOptions)
    {
        int startOfIncompleteWord=0;
        bool processingIncompleteWord = false;
        std::string incompleteWord{};
        for (int i = 0; i < lineToLex.size(); i++)
        {
            //If the next character is a space, we have a token.
            if (std::isspace(lineToLex[i]))
            {
                //Or we might be a block of spaces
                if (!processingIncompleteWord)
                {
                    continue;
                }
                else
                {
                    tokenStream.push_back(LexToken(fileIndex, lineNumber, startOfIncompleteWord, incompleteWord));
                    startOfIncompleteWord = 0;
                    processingIncompleteWord = false;
                    incompleteWord.clear();
                }
            }
        }        
    }

    std::vector<lexedFile>* Lexer::GetLexedFiles() const
    {
        return lexedFiles.get();
    }

    cppnext::token::Token Lexer::LexToken(int32_t fileIndex, int32_t lineNumber, int32_t linePosition, std::string value)
    {
        return CreateToken(fileIndex, lineNumber, linePosition, cppnext::token::Unknown, value);
    }


    cppnext::token::Token Lexer::CreateToken(int32_t fileIndex, int32_t lineNumber, int32_t linePosition, cppnext::token::tokenType type, std::string value)
    {
        cppnext::token::Token newToken;
        newToken.fileIndex = fileIndex;
        newToken.lineNumber = lineNumber;
        newToken.linePosition = linePosition;
        newToken.type = type;
        newToken.value = value;
        return newToken;
    }

    void Lexer::Print([[maybe_unused]] const cxxopts::ParseResult& commandLineOptions) const
    {
        for (const auto& file : *lexedFiles)
        {
            fmt::print("Lexed File[{}] {} Begin\n", file.fileIndex, file.originalPath.string());
            for (const auto& token : file.tokens)
            {
                fmt::print("Token File Index[{}] Line Number[{}] Line Position[{}] Value[{}]", token.fileIndex, token.lineNumber, token.linePosition, token.value);
            }
        }
    }
}