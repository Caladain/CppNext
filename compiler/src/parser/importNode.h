#pragma once
#include <vector>

#include <cxxopts.hpp>

#include "parser/parseNodeInterface.h"
#include "token/token.h"

namespace cppnext
{
    struct ImportNodeData
    {
        std::vector<Token> tokens;
        std::string value;
        std::string toString(const std::string& indent, [[maybe_unused]] const cxxopts::ParseResult& commandLineOptions)
        {
            std::string output;
            output += fmt::format("{} [ImportNodeData] Value:{}\n", indent, value);
            output += fmt::format("{} [ImportNodeData] Tokens:\n", indent);
            for (const auto& token : tokens)
            {
                output += fmt::format("{} {}\n", indent+"    ", token.toString());
            }            
            return output;
        }
    };
    namespace parser
    {
        struct Node;
    }    
    class ImportNode : public ParseNode{
    public:
        virtual std::optional<parser::Node> ConsumeParse(const std::vector<Token>& tokens, int32_t& position) override;
    };
}