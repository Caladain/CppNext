#pragma once
#include <vector>

#include <cxxopts.hpp>

#include "parser/parseNodeInterface.h"
#include "token/token.h"

namespace cppnext
{
    struct VariableNodeData
    {
        std::vector<Token> tokens;
        std::string identifer;
        tokenType type{ Unknown };
        std::string typeValue{};
        bool mutableVariable{ false };

        std::string toString(const std::string& indent, [[maybe_unused]] const cxxopts::ParseResult& commandLineOptions)
        {
            std::string output;
            output += fmt::format("{} [VariableNodeData] identifer:{}\n", indent, identifer);// magic_enum::enum_name(
            output += fmt::format("{} [VariableNodeData] type:{}\n", indent, magic_enum::enum_name(type));
            output += fmt::format("{} [VariableNodeData] typeValue:{}\n", indent, typeValue);
            output += fmt::format("{} [VariableNodeData] mutable:{}\n", indent, mutableVariable);
            output += fmt::format("{} [VariableNodeData] Tokens:\n", indent);
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
    class VariableNode : public ParseNode{
    public:
        virtual std::optional<parser::Node> ConsumeParse(const std::vector<Token>& tokens, int32_t& position) override;
    };
}