#pragma once
#include <variant>
#include <vector>
#include <memory>
#include <string>
#include <stdint.h>
#include <optional>
#include "token/token.h"
#include "token/tokenTable.h"

namespace cppnext
{
    namespace parser
    {
        struct Node;
    }
    class ParseNode
    {
    public:
        virtual std::optional<parser::Node> ConsumeParse(const std::vector<Token>& tokens, int32_t& position) = 0;
    };
}