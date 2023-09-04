#pragma once
#include "token/tokenTable.h"
#include <string>
#include <stdint.h>

namespace cppnext::token {
    struct Token
    {
        int32_t fileIndex{ 0 };
        int32_t lineNumber{ 0 };
        int32_t linePosition{ 0 };
        tokenType type;
        std::string value;
    };
}
