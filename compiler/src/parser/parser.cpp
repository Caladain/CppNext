#include "parser/parser.h"

#include "parser/importNode.h"


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
            //RKR: Testing only, remove
            abstractSyntaxTree.clear();
            //RKR
            ParseFile(file, commandLineOptions);
        }
        
    }

    void Parser::ParseFile(const lexedFile& LexedFile, const cxxopts::ParseResult& commandLineOptions)
    {        
        std::size_t lengthOfTokens = LexedFile.tokens.size();
        const auto& tokenVector = LexedFile.tokens;
        for (int32_t i = 0; i < lengthOfTokens; i++)
        {
            auto result = generalParser.ConsumeParse(tokenVector, i);
            if (result)
            {
                auto value = result.value();
                abstractSyntaxTree.emplace_back(value);
            }
            else
            {
                //throw;
            }
        }
        if (commandLineOptions.count("parserdebug"))
        {
            DebugParseOfFile(LexedFile, commandLineOptions);
        }
    }
    
    std::unique_ptr<parser::Node> Parser::ParseTokens([[maybe_unused]]const std::vector<Token>& tokens, [[maybe_unused]] int32_t& position)
    {
        return nullptr;
    }

    void Parser::DebugParseOfFile(const lexedFile& LexedFile, const cxxopts::ParseResult& commandLineOptions)
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
            outputPath.append(LexedFile.originalPath.filename().string());
            outputPath.replace_extension("debug");
            std::filesystem::create_directories(outputPath.parent_path());
            auto outputFile = fmt::output_file(outputPath.string());
            outputFile.print("Parse File[{}] {} Begin\n", LexedFile.fileIndex, LexedFile.originalPath.string());
            for (auto& treeNode : abstractSyntaxTree)
            {
                outputFile.print("{}\n", treeNode.toString("",commandLineOptions));
            }
        }
    }
}