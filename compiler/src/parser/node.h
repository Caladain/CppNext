#pragma once
#include <variant>
#include <vector>
#include <memory>
#include <string>
#include <stdint.h>
#include "token/tokenTable.h"
namespace cppnext
{
    struct Node {
        std::vector<std::unique_ptr<Node>> childNodes;
        tokenType type{ tokenType::Unknown };
        std::variant<            
            int64_t,            
            uint64_t,
            double_t,
            bool,
            char32_t,
            std::string> value;
    };
}