#include "lexer/lexer.h"

#include <cctype>
#include <fstream>

#include <magic_enum.hpp>
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
            int32_t lineNumber{ 0 };
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
            const std::string characterBeingEvaluated { lineToLex[i] };
            const auto representationCount = cppnext::token::tokenRepresentation.count(characterBeingEvaluated);
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
                    continue;
                }
            }
            if (representationCount > 0)
            {
                if (processingIncompleteWord)
                {
                    std::string maybeWord = incompleteWord + characterBeingEvaluated;
                    if (cppnext::token::tokenRepresentation.count(maybeWord))
                    {
                        tokenStream.push_back(LexToken(fileIndex, lineNumber, startOfIncompleteWord, maybeWord));
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
                if (characterBeingEvaluated == ":")
                {
                    startOfIncompleteWord = i;
                    processingIncompleteWord = true;
                    incompleteWord = characterBeingEvaluated;
                    continue;
                }
                else
                {
                    tokenStream.push_back(LexToken(fileIndex, lineNumber, i, characterBeingEvaluated));
                    continue;
                }
            }
            if (processingIncompleteWord)
            {
                incompleteWord += characterBeingEvaluated;
                continue;
            }
            else
            {
                startOfIncompleteWord = i;
                processingIncompleteWord = true;
                incompleteWord = characterBeingEvaluated;
            }
        }        
    }

    std::vector<lexedFile>* Lexer::GetLexedFiles() const
    {
        return lexedFiles.get();
    }

    cppnext::token::Token Lexer::LexToken(int32_t fileIndex, int32_t lineNumber, int32_t linePosition, std::string value)
    {
        if (cppnext::token::tokenRepresentation.count(value))
        {
            return CreateToken(fileIndex, lineNumber, linePosition, cppnext::token::tokenRepresentation[value], value);
        }
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
                PrintToken(token, commandLineOptions);
            }
        }
    }
    void Lexer::PrintToken(const cppnext::token::Token& token, [[maybe_unused]] const cxxopts::ParseResult& commandLineOptions) const
    {
        const auto& lexedFile = (*lexedFiles)[token.fileIndex];
        auto prettyLineNumber = token.lineNumber + 1; //Line numbers are zero index, but humans like 1 index for error messages.

        std::string rawLine = lexedFile.originalLines[token.lineNumber];
        auto firstPosition = rawLine.find_first_not_of(" \t");
        auto lastPosition = rawLine.find_last_not_of(" \t");
        std::string_view trimmedString = std::string_view(rawLine).substr(firstPosition, lastPosition+1);
        auto caretPosition = token.linePosition - firstPosition;
        auto space = std::string(caretPosition, ' ');
        fmt::print("{}  :({})({})\n", trimmedString, prettyLineNumber, token.linePosition);
        fmt::print("{}^ {} {}\n", space, magic_enum::enum_name(token.type), token.value);
    }
}