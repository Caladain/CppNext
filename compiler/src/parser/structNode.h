#pragma once
#include <vector>

#include <cxxopts.hpp>

#include "parser/parseNodeInterface.h"
#include "token/token.h"

namespace cppnext
{
    struct StructNodeData
    {
        bool isStruct = true;
        std::vector<Token> tokens;
        std::string identifier;
        std::string toString(const std::string& indent, [[maybe_unused]] const cxxopts::ParseResult& commandLineOptions)
        {
            std::string output;
            output += fmt::format("{} [NamespaceNodeData] isStruct:{}\n", indent, isStruct);
            output += fmt::format("{} [NamespaceNodeData] identifier:{}\n", indent, identifier);
            output += fmt::format("{} [NamespaceNodeData] Tokens:\n", indent);
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
    class StructNode : public ParseNode{
    public:
        virtual std::optional<parser::Node> ConsumeParse(const std::vector<Token>& tokens, int32_t& position) override;
    };
}