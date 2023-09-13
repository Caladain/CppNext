#pragma once
#include <variant>
#include <vector>
#include <memory>
#include <string>
#include <stdint.h>
#include "token/tokenTable.h"
#include "parser/importNode.h"

namespace cppnext::parser
{
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
    };
}