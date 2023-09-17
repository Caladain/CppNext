#pragma once
#include <variant>
#include <vector>
#include <memory>
#include <string>
#include <stdint.h>

#include <magic_enum.hpp>
#include "fmt/core.h"

#include "token/tokenTable.h"
#include "parser/importNode.h"



namespace cppnext::parser
{
    template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
    enum parseNodeType
    {
        Unknown = 0,
        Import
    };
    struct Node {
        std::vector<Node> childNodes;
        parseNodeType type{ parseNodeType::Unknown };
        std::variant<
            ImportNodeData> nodeData;
        std::variant<            
            int64_t,            
            uint64_t,
            double_t,
            bool,
            char32_t,
            std::string> value;
        std::string toString(const std::string& incomingIndent, [[maybe_unused]] const cxxopts::ParseResult& commandLineOptions)
        {
            std::string output;
            std::string indent = "    " + incomingIndent;
            output += fmt::format("{} [Node]\n", indent);
            indent += "    ";
            output += fmt::format("{} Type:{}\n", indent, magic_enum::enum_name(type));
            std::visit(overload{
                [&output, &indent, this](int64_t&) { output += fmt::format("{} Value: int64_t {}\n", indent, std::get<int64_t>(value)); },
                [&output, &indent, this](uint64_t&) { output += fmt::format("{} Value: uint64_t {}\n", indent, std::get<uint64_t>(value)); },
                [&output, &indent, this](double_t&) { output += fmt::format("{} Value: double_t {}\n", indent, std::get<double_t>(value)); },
                [&output, &indent, this](bool&) { output += fmt::format("{} Value: bool {}\n", indent, std::get<bool>(value)); },
                [&output, &indent, this](char32_t&) { output += fmt::format("{} Value: char32_t {}\n", indent, static_cast<uint32_t>(std::get<char32_t>(value))); },
                [&output, &indent, this](std::string&) { output += fmt::format("{} Value: std::string {}\n", indent, std::get<std::string>(value)); }
                }, value);
            std::visit(overload{
                [&commandLineOptions, &output, &indent, this](ImportNodeData&) { auto data = std::get<ImportNodeData>(nodeData); output += fmt::format("{} Data:\n{}", indent, data.toString(indent + "    ", commandLineOptions)); }
                }, nodeData);
            if (childNodes.size() != 0)
            {
                output += fmt::format("{} ChildNodes:\n", indent);
                for (auto& child : childNodes)
                {
                    output += child.toString(indent, commandLineOptions);
                }
            }
            return output;
        }
    };
}