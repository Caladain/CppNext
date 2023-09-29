#pragma once
#include <vector>

#include <cxxopts.hpp>

#include "parser/parseNodeInterface.h"
#include "token/token.h"

namespace cppnext
{
    struct FunctionNodeData
    {
        std::vector<Token> tokens;
        std::string identifer;
        tokenType type{ Unknown };
        bool publicFunction = false;
        std::string typeString{};
        bool mutableVariable{ false };
        struct paramStruct
        {
            std::string identifer;
            tokenType type{ Unknown };
            std::string typeString{};
            bool mutableParam{ false };
            bool reference{ false };
            std::vector<Token> tokens;
            std::string toString(const std::string& indent, [[maybe_unused]] const cxxopts::ParseResult& commandLineOptions) const
            {
                std::string output;
                output += fmt::format("{} [Parameter] identifer:{}\n", indent, identifer);// magic_enum::enum_name(
                output += fmt::format("{} [Parameter] type:{}\n", indent, magic_enum::enum_name(type));                
                output += fmt::format("{} [Parameter] typeString:{}\n", indent, typeString);
                output += fmt::format("{} [Parameter] reference:{}\n", indent, reference);
                output += fmt::format("{} [Parameter] mutable:{}\n", indent, mutableParam);
                output += fmt::format("{} [Parameter] Tokens:\n", indent);
                for (const auto& token : tokens)
                {
                    output += fmt::format("{} {}\n", indent + "    ", token.toString());
                }
                return output;
            }
        };
        std::vector<paramStruct> parameters;

        std::string toString(const std::string& indent, [[maybe_unused]] const cxxopts::ParseResult& commandLineOptions) const
        {
            std::string output;
            output += fmt::format("{} [FunctionNodeData] identifer:{}\n", indent, identifer);// magic_enum::enum_name(
            output += fmt::format("{} [FunctionNodeData] type:{}\n", indent, magic_enum::enum_name(type));
            output += fmt::format("{} [FunctionNodeData] typeString:{}\n", indent, typeString);
            output += fmt::format("{} [FunctionNodeData] publicFunction:{}\n", indent, publicFunction);            
            output += fmt::format("{} [FunctionNodeData] mutable:{}\n", indent, mutableVariable);
            output += fmt::format("{} [FunctionNodeData] Parameters:\n", indent);
            for (const auto& parameter : parameters)
            {
                output += fmt::format("{}\n", parameter.toString(indent + "    ", commandLineOptions));
            }
            output += fmt::format("{} [FunctionNodeData] Tokens:\n", indent);
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
    class FunctionNode : public ParseNode{
    public:
        virtual std::optional<parser::Node> ConsumeParse(const std::vector<Token>& tokens, int32_t& position) override;
    };
}