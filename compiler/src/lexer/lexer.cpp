#include "lexer/lexer.h"
#include "errors/errors.h"

#include <cctype>
#include <fstream>
#include <memory>
#include <filesystem>

#include <magic_enum.hpp>
#include "fmt/core.h"


namespace cppnext
{
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
        if (commandLineOptions.count("lexerdebug") || commandLineOptions.count("lexerdebugtokens"))
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
                ThrowError(1, fmt::format(ErrorTable[1], prefixedPath.string()));
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
            ErrorAndPrintIfOpenCloseMismatch(parenContainer, '(', ')');
            ErrorAndPrintIfOpenCloseMismatch(bracketContainer, '[', ']');
            ErrorAndPrintIfOpenCloseMismatch(braceContainer, '{', '}');
            file.close();
        }
        
    }

    void Lexer::LexLine(int32_t fileIndex, int32_t lineNumber, std::vector<Token>& tokenStream, const std::string& lineToLex, [[maybe_unused]] const cxxopts::ParseResult& commandLineOptions)
    {        
        for (int32_t i = 0; i < lineToLex.size(); i++)
        {
             char characterBeingEvaluated { lineToLex[i] };
                       
            if (isspace(lineToLex[i]))
            {
                continue;
            }
            if (IsCommentStartCharacter(lineToLex[i]) || inMultiLineComment)
            {
                ConsumeComment(characterBeingEvaluated, lineToLex, i);
                continue;
            }
            if (IsValidStartIdentifierCharacter(lineToLex[i]))
            {
                int startOfIdentifierString = i;
                std::string IdentifierString = ConsumeIdentifier(characterBeingEvaluated, lineToLex, i);
                if (tokenRepresentation.count(IdentifierString))
                {
                    tokenStream.push_back(LexToken(fileIndex, lineNumber, startOfIdentifierString, IdentifierString));
                }
                else
                {
                    tokenStream.push_back(LexToken(fileIndex, lineNumber, startOfIdentifierString, tokenType::AlphaNumeric, IdentifierString));
                }
                characterBeingEvaluated = lineToLex[i];
            }
            if (IsValidStartNumericalCharacter(lineToLex[i]))
            {
                int startOfIdentifierString = i;
                std::string IdentifierString = ConsumeNumerical(characterBeingEvaluated, lineToLex, i);
                if (tokenRepresentation.count(IdentifierString))
                {
                    tokenStream.push_back(LexToken(fileIndex, lineNumber, startOfIdentifierString, IdentifierString));
                }
                else
                {
                    tokenStream.push_back(LexToken(fileIndex, lineNumber, startOfIdentifierString, tokenType::NumericAlpha, IdentifierString));
                }
                characterBeingEvaluated = lineToLex[i];
            }
            if (CreateTokenIfReservedSymbol(characterBeingEvaluated, lineToLex, fileIndex, lineNumber, i, tokenStream))
            {
                
            }
        }
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
                break;
            }
        }
        return incompleteWord;
    }

    std::string Lexer::ConsumeStringLiteral(const std::string& lineToLex, int32_t& positionInLine)
    {
        std::string incompleteWord;
        positionInLine++;
        bool foundEndQuote = false;
        for (; positionInLine < lineToLex.size(); positionInLine++)
        {            
            if (lineToLex[positionInLine] == '"')
            {
                break;
            }
            incompleteWord += lineToLex[positionInLine];
        }
        if (!foundEndQuote)
        {
            //throw
        }
        return incompleteWord;
    }

    std::string Lexer::ConsumeCharacterLiteral(const std::string& lineToLex, int32_t& positionInLine)
    {
        std::string incompleteWord;
        positionInLine++;
        bool foundEndQuote = false;
        for (; positionInLine < lineToLex.size(); positionInLine++)
        {            
            if (lineToLex[positionInLine] == '\'')
            {
                break;
            }
            incompleteWord += lineToLex[positionInLine];
        }
        if (!foundEndQuote)
        {
            //throw
        }
        return incompleteWord;
    }

    void Lexer::ConsumeComment(const char characterBeingEvaluated, const std::string& lineToLex, int32_t& positionInLine)
    {
        char nextCharacterBeingEvaluated{};
        if (positionInLine + 1 <= lineToLex.size())
        {
            {
                nextCharacterBeingEvaluated = lineToLex[positionInLine + 1];
            }
        }
        if (characterBeingEvaluated == '/' && nextCharacterBeingEvaluated == '*')
        {
            inMultiLineComment = true;
        }
        if (characterBeingEvaluated == '*' && nextCharacterBeingEvaluated == '/')
        {
            inMultiLineComment = false;
        }
        if ((characterBeingEvaluated == '/' && nextCharacterBeingEvaluated == '/'))
        {
            positionInLine = (int32_t)lineToLex.size();
        }
        else
        {
            positionInLine++;
        }
    }

    std::string Lexer::ConsumeNumerical(const char characterBeingEvaluated, const std::string& lineToLex, int32_t& positionInLine)
    {
        std::string incompleteWord = { characterBeingEvaluated };
        positionInLine++;
        bool firstPeriodSeperatorFound = false;
        bool firstAlphaFound = false;
        for (; positionInLine < lineToLex.size(); positionInLine++)
        {
            if (IsNumericalCharacter(lineToLex[positionInLine]))
            {
                if (lineToLex[positionInLine] == '.')
                {
                    if (firstPeriodSeperatorFound)
                    {
                        //throw
                    }
                    else
                    {
                        firstPeriodSeperatorFound = true;
                    }
                }
                if (isalpha(lineToLex[positionInLine]))
                {
                    if (firstAlphaFound)
                    {
                        //throw
                    }
                    else
                    {
                        firstAlphaFound = true;
                    }
                }
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

    bool Lexer::CreateTokenIfReservedSymbol(const char characterBeingEvaluated, const std::string& lineToLex, int32_t fileIndex, int32_t lineNumber, int32_t& positionInLine, std::vector<Token>& tokenStream)
    {
        char nextCharacterBeingEvaluated{};
        if (positionInLine + 1 <= lineToLex.size())
        {
            {
                nextCharacterBeingEvaluated = lineToLex[positionInLine + 1];
            }
        }
        const auto representationCount = tokenRepresentation.count(std::string{characterBeingEvaluated});
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
            if (characterBeingEvaluated == '"')
            {
                int startOfStringLiteral = positionInLine;
                std::string word = ConsumeStringLiteral(lineToLex, positionInLine);
                tokenStream.push_back(LexToken(fileIndex, lineNumber, startOfStringLiteral, tokenType::StringLiteral, word));
                return true;
            }
            if (characterBeingEvaluated == '\'')
            {
                int startOfStringLiteral = positionInLine;
                std::string word = ConsumeCharacterLiteral(lineToLex, positionInLine);
                tokenStream.push_back(LexToken(fileIndex, lineNumber, startOfStringLiteral, tokenType::CharacterLiteral, word));
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
            auto newToken = LexToken(fileIndex, lineNumber, positionInLine, std::string{ characterBeingEvaluated });
            HandleOpenCloseCounting(parenContainer, parenCount, '(', ')', characterBeingEvaluated, newToken);
            HandleOpenCloseCounting(bracketContainer, bracketCount, '[', ']', characterBeingEvaluated, newToken);
            HandleOpenCloseCounting(braceContainer, braceCount, '{', '}', characterBeingEvaluated, newToken);            
            tokenStream.push_back(newToken);
            return true;
        }
        return false;
    }
    void Lexer::HandleOpenCloseCounting(std::vector<Token>& container, int32_t& counter, const char OpeningSymbol, const char ClosingSymbol, const char currentSymbol, Token newToken)
    {
        if (currentSymbol == OpeningSymbol || currentSymbol == ClosingSymbol)
        {
            container.push_back(newToken);
            if (currentSymbol == OpeningSymbol)
            {
                counter++;
            }
            else
            {
                counter--;
                if (counter == 0)
                {
                    container.clear();
                }
                if (counter < 0)
                {
                    fmt::print(ErrorTable[2], currentSymbol, OpeningSymbol);
                    ThrowError(2, fmt::format("{}", FormatDebugToken(newToken, {})));
                }
            }
        }
    }
    void Lexer::ErrorAndPrintIfOpenCloseMismatch(const std::vector<Token>& container, const char OpeningSymbol, const char ClosingSymbol)
    {
        if (container.size() != 0)
        {
            fmt::print(ErrorTable[3], OpeningSymbol, ClosingSymbol, OpeningSymbol, ClosingSymbol);
            for (const auto& token : container)
            {
                fmt::print("{}", FormatDebugToken(token, {}));
            }
            ThrowError(3, "");
        }
    }

    bool Lexer::IsValidStartIdentifierCharacter(const char character) const
    {
        return isalpha(character) || character == '_';
    }

    bool Lexer::IsValidStartNumericalCharacter(const char character) const
    {
        return isdigit(character);
    }

    bool Lexer::IsCommentStartCharacter(const char character) const
    {
        return character == '/'; // // or /* 
    }

    bool Lexer::IsIdentifierCharacter(const char character) const
    {
        return isalnum(character) || character == '_';
    }

    bool Lexer::IsNumericalCharacter(const char character) const
    {
        return isalnum(character) || character == '.';
    }

    std::vector<lexedFile>* Lexer::GetLexedFiles() const
    {
        return lexedFiles.get();
    }

    Token Lexer::LexToken(int32_t fileIndex, int32_t lineNumber, int32_t linePosition, tokenType type, std::string value)
    {
        return CreateToken(fileIndex, lineNumber, linePosition, type, value);
    }

    Token Lexer::LexToken(int32_t fileIndex, int32_t lineNumber, int32_t linePosition, std::string value)
    { 
        if (tokenRepresentation.count(value))
        {
            return LexToken(fileIndex, lineNumber, linePosition, tokenRepresentation[value], value);
        }
        
        return LexToken(fileIndex, lineNumber, linePosition, tokenType::Unknown, value);
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
        std::string fullErrorLine = fmt::format("{}  :Line({})Pos({})\n", trimmedString, prettyLineNumber, linePosition + 1);
        std::string caretLine = fmt::format("{}^", space);
        return { fullErrorLine, caretLine };
    }


    Token Lexer::CreateToken(int32_t fileIndex, int32_t lineNumber, int32_t linePosition, tokenType type, std::string value)
    {
        Token newToken;
        newToken.fileIndex = fileIndex;
        newToken.lineNumber = lineNumber;
        newToken.linePosition = linePosition;
        newToken.SetValue(type, value);
        return newToken;
    }

    void Lexer::Print(const cxxopts::ParseResult& commandLineOptions) const
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
            if (commandLineOptions.count("lexerdebug"))
            {
                std::filesystem::path outputPath = outputPathPrefix;
                outputPath.append(file.originalPath.filename().string());
                outputPath.replace_extension("debug");
                std::filesystem::create_directories(outputPath.parent_path());
                auto outputFile = fmt::output_file(outputPath.string());
                outputFile.print("Lexed File[{}] {} Begin\n", file.fileIndex, file.originalPath.string());
                for (const auto& token : file.tokens)
                {
                    outputFile.print("{}", FormatDebugToken(token, commandLineOptions));
                }
            }
            if (commandLineOptions.count("lexerdebugtokens"))
            {
                std::filesystem::path outputPath = outputPathPrefix;
                outputPath.append(file.originalPath.filename().string());
                outputPath.replace_extension("tokens");
                std::filesystem::create_directories(outputPath.parent_path());
                auto tokenFile = fmt::output_file(outputPath.string());
                for (const auto& token : file.tokens)
                {
                    tokenFile.print("{}", FormatToken(token, commandLineOptions));
                }
            }
        }
    }
    std::string Lexer::FormatDebugToken(const Token& token, [[maybe_unused]] const cxxopts::ParseResult& commandLineOptions) const
    {
        const auto [ErrorLine, spacedCaret] = PrepareErrorMessageLine(token.fileIndex, token.lineNumber, token.linePosition);
        std::string output = fmt::format("{}", ErrorLine);
        return output + fmt::format("{} {}\n", spacedCaret, token.toString());
    }
    std::string Lexer::FormatToken(const Token& token, [[maybe_unused]] const cxxopts::ParseResult& commandLineOptions) const
    {
        return fmt::format("[{}]\n", token.toString());
    }
}