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
    void Lexer::Lex(const cxxopts::ParseResult& commandLineOptions)
    {
        ProcessFilePaths(commandLineOptions);
        for (auto& file : *lexedFiles)
        {
            LexFile(file, commandLineOptions);
        }
        if (commandLineOptions.count("lexerdebug"))
        {
            Print(commandLineOptions);
        }
    }

    void Lexer::ProcessFilePaths(const cxxopts::ParseResult& commandLineOptions)
    {
        std::filesystem::path prefix{};
        int32_t fileCount{ 0 };
        std::vector<std::string> rawFilePaths;
        bool prefixUsed{ false };
        if (commandLineOptions.count("recursive") && commandLineOptions.count("sourceFolder"))
        {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(commandLineOptions["sourceFolder"].as<std::string>()))
            {
                if (entry.is_regular_file() && !entry.is_symlink() && entry.path().extension() == ".cppn")
                {
                    rawFilePaths.push_back(entry.path().string());
                    prefix = "";
                    prefixUsed = true;
                }
            }
        }
        else
        {            
            if (commandLineOptions.count("sourceFolder"))
            {
                prefix = commandLineOptions["sourceFolder"].as<std::string>();
                prefixUsed = true;
            }
            rawFilePaths = commandLineOptions["files"].as<std::vector<std::string>>();
        }
        
        for (const auto& file : rawFilePaths)
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
        for (int i = 0; i < lineToLex.size(); i++)
        {
             char characterBeingEvaluated { lineToLex[i] };
                       
            if (isspace(lineToLex[i]))
            {
                continue;
            }
            if (IsIdentifierCharacter(lineToLex[i]))
            {
                int startOfIdentifierString = i;
                std::string IdentifierString = ConsumeIdentifier(characterBeingEvaluated, lineToLex, i);
                if (cppnext::token::tokenRepresentation.count(IdentifierString))
                {
                    tokenStream.push_back(LexToken(fileIndex, lineNumber, startOfIdentifierString, IdentifierString));
                }
                else
                {
                    tokenStream.push_back(LexToken(fileIndex, lineNumber, startOfIdentifierString, cppnext::token::tokenType::AlphaNumeric, IdentifierString));
                }
                characterBeingEvaluated = lineToLex[i];
                /*startOfIncompleteWord = i;
                incompleteWord = characterBeingEvaluated;
                i++;
                for (; i < lineToLex.size(); i++)
                {
                    if (IsIdentifierCharacter(lineToLex[i]))
                    {
                        incompleteWord += lineToLex[i];
                    }
                    else
                    {
                        tokenStream.push_back(LexToken(fileIndex, lineNumber, startOfIncompleteWord, incompleteWord));
                        CreateTokenIfReservedSymbol(lineToLex[i], lineToLex, fileIndex, lineNumber, i, tokenStream);
                        break;                        
                    }
                }                
                continue;*/
            }
            if (CreateTokenIfReservedSymbol(characterBeingEvaluated, lineToLex, fileIndex, lineNumber, i, tokenStream))
            {
                
            }
        }
        /*if (processingIncompleteWord)
        {
            tokenStream.push_back(LexToken(fileIndex, lineNumber, startOfIncompleteWord, incompleteWord));
        }*/
    }

    std::string Lexer::ConsumeIdentifier(const char characterBeingEvaluated, const std::string& lineToLex, int32_t& positionInLine)
    {        
        std::string incompleteWord = { characterBeingEvaluated };
        positionInLine++;
        for (; positionInLine < lineToLex.size(); positionInLine++)
        {
            if (IsIdentifierCharacter(lineToLex[positionInLine]))
            {
                incompleteWord += lineToLex[positionInLine];
            }
            else
            {
                //positionInLine--;
                break;
            }
        }
        return incompleteWord;
    }

    bool Lexer::CreateTokenIfReservedSymbol(const char characterBeingEvaluated, const std::string& lineToLex, int32_t fileIndex, int32_t lineNumber, int32_t& positionInLine, std::vector<cppnext::token::Token>& tokenStream)
    {
        char nextCharacterBeingEvaluated{};
        if (positionInLine + 1 <= lineToLex.size())
        {
            {
                nextCharacterBeingEvaluated = lineToLex[positionInLine + 1];
            }
        }
        const auto representationCount = cppnext::token::tokenRepresentation.count(std::string{characterBeingEvaluated});
        if (representationCount)
        {
            if (characterBeingEvaluated == ':' && nextCharacterBeingEvaluated == ':')
            {
                tokenStream.push_back(LexToken(fileIndex, lineNumber, positionInLine, std::string{characterBeingEvaluated} + std::string{nextCharacterBeingEvaluated}));
                positionInLine++;
                return true;
            }
            if (characterBeingEvaluated == '-' && nextCharacterBeingEvaluated == '>')
            {
                tokenStream.push_back(LexToken(fileIndex, lineNumber, positionInLine, std::string{characterBeingEvaluated} + std::string{nextCharacterBeingEvaluated}));
                positionInLine++;
                return true;
            }
            if (characterBeingEvaluated == '#')
            {
                std::string word{};
                int startOfIdentifierString = positionInLine;
                word += ConsumeIdentifier(characterBeingEvaluated, lineToLex, positionInLine);                
                tokenStream.push_back(LexToken(fileIndex, lineNumber, startOfIdentifierString, word));
                return true;
            }
            tokenStream.push_back(LexToken(fileIndex, lineNumber, positionInLine, std::string{characterBeingEvaluated}));
            return true;
        }
        return false;
    }

    bool Lexer::IsIdentifierCharacter(const char character) const
    {
        return isalnum(character) || character == '_';
    }

    std::vector<lexedFile>* Lexer::GetLexedFiles() const
    {
        return lexedFiles.get();
    }

    cppnext::token::Token Lexer::LexToken(int32_t fileIndex, int32_t lineNumber, int32_t linePosition, cppnext::token::tokenType type, std::string value)
    {
        return CreateToken(fileIndex, lineNumber, linePosition, type, value);
    }

    cppnext::token::Token Lexer::LexToken(int32_t fileIndex, int32_t lineNumber, int32_t linePosition, std::string value)
    { 
        if (cppnext::token::tokenRepresentation.count(value))
        {
            return LexToken(fileIndex, lineNumber, linePosition, cppnext::token::tokenRepresentation[value], value);
        }
        
        return LexToken(fileIndex, lineNumber, linePosition, cppnext::token::Unknown, value);
    }

    std::tuple<std::string, std::string> Lexer::PrepareErrorMessageLine(const int32_t& fileIndex, const int32_t& lineNumber, const int32_t& linePosition) const
    {
        const auto& lexedFile = (*lexedFiles)[fileIndex];
        auto prettyLineNumber = lineNumber + 1; //Line numbers are zero index, but humans like 1 index for error messages.

        std::string rawLine = lexedFile.originalLines[lineNumber];
        auto firstPosition = rawLine.find_first_not_of(" \t");
        auto lastPosition = rawLine.find_last_not_of(" \t");
        std::string_view trimmedString = std::string_view(rawLine).substr(firstPosition, lastPosition + 1);
        auto caretPosition = linePosition - firstPosition;
        auto space = std::string(caretPosition, ' ');
        std::string fullErrorLine = fmt::format("{}  :({})({})\n", trimmedString, prettyLineNumber, linePosition);
        std::string caretLine = fmt::format("{}^", space);
        return { fullErrorLine, caretLine };
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
        std::filesystem::path outputPathPrefix = ".";
        if (commandLineOptions.count("output"))
        {
            outputPathPrefix = commandLineOptions["output"].as<std::string>();
        }
        outputPathPrefix.append("compilerDiagnostics");
        outputPathPrefix.append("lexer");
        for (const auto& file : *lexedFiles)
        {
            std::filesystem::path outputPath = outputPathPrefix;
            outputPath.append(file.originalPath.filename().string());
            outputPath.replace_extension("txt");
            std::filesystem::create_directories(outputPath.parent_path());
            auto outputFile = fmt::output_file(outputPath.string());
            outputFile.print("Lexed File[{}] {} Begin\n", file.fileIndex, file.originalPath.string());
            for (const auto& token : file.tokens)
            {
                PrintToken(token, commandLineOptions, outputFile);
            }
        }
    }
    void Lexer::PrintToken(const cppnext::token::Token& token, [[maybe_unused]] const cxxopts::ParseResult& commandLineOptions, fmt::ostream& outputFile) const
    {
        const auto [ErrorLine, spacedCaret] = PrepareErrorMessageLine(token.fileIndex, token.lineNumber, token.linePosition);
        outputFile.print("{}", ErrorLine);
        outputFile.print("{} {} {}\n", spacedCaret, magic_enum::enum_name(token.type), token.value);
    }
}