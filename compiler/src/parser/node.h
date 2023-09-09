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
            int8_t, 
            int16_t,
            int32_t,
            int64_t,
            uint8_t,
            uint16_t,
            uint32_t,
            uint64_t,
            float_t,
            double_t,
            bool,
            char8_t,
            char16_t,
            char32_t,
            std::string> value;
    };
}