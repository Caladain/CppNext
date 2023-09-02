#include "lexer.h"

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
    }

    void Lexer::ProcessFilePaths(const std::vector<std::string>& rawCommandLineFilePaths, const cxxopts::ParseResult& commandLineOptions)
    {
        std::filesystem::path prefix{};
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
                LexLine(lineNumber, fileToLex.tokens, line, commandLineOptions);
                lineNumber++;
            }
            file.close();
        }
        
    }

    void Lexer::LexLine(int32_t lineNumber, std::vector<token>& tokenStream, [[maybe_unused]] const std::string& lineToLex, [[maybe_unused]] const cxxopts::ParseResult& commandLineOptions)
    {

        token newToken;
        newToken.lineNumber = lineNumber;
        tokenStream.push_back(newToken);
    }

    std::vector<lexedFile>* Lexer::GetLexedFiles()
    {
        return lexedFiles.get();
    }
}