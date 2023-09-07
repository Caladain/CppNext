#include "parser/parser.h"

#include <magic_enum.hpp>

namespace cppnext
{
    Parser::Parser(Lexer* lexerToUse)
    {
        lexer = lexerToUse;
        if (lexer == nullptr)
        {
            //throw
        }
    }

    void Parser::Parse(const cxxopts::ParseResult& commandLineOptions)
    {
        auto lexedFilesVector = lexer->GetLexedFiles();
        for (const auto& file : *lexedFilesVector)
        {
            ParseFile(file, commandLineOptions);
        }
    }

    void Parser::ParseFile(const auto& lexedFile, const cxxopts::ParseResult& commandLineOptions)
    {
        if (commandLineOptions.count("parserdebug"))
        {
            DebugParseOfFile(lexedFile, commandLineOptions);
        }
    }

    void Parser::DebugParseOfFile(const auto& lexedFile, const cxxopts::ParseResult& commandLineOptions)
    {
        std::filesystem::path outputPathPrefix = ".";
        if (commandLineOptions.count("output"))
        {
            outputPathPrefix = commandLineOptions["output"].as<std::string>();
        }
        outputPathPrefix.append("compilerDiagnostics");
        outputPathPrefix.append("parser");
        if (commandLineOptions.count("parserdebug"))
        {
            std::filesystem::path outputPath = outputPathPrefix;
            outputPath.append(lexedFile.originalPath.filename().string());
            outputPath.replace_extension("debug");
            std::filesystem::create_directories(outputPath.parent_path());
            auto outputFile = fmt::output_file(outputPath.string());
            outputFile.print("Parse File[{}] {} Begin\n", lexedFile.fileIndex, lexedFile.originalPath.string());
            for (const auto& token : lexedFile.tokens)
            {
                outputFile.print("[{}:{}]\n", magic_enum::enum_name(token.type), token.value);
            }
        }
    }
}