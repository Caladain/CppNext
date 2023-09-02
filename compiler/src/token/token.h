#pragma once
#include "token/tokenTable.h"
#include <string>
#include <stdint.h>

namespace cppnext::token {
    struct Token
    {
        int32_t fileIndex;
        int32_t lineNumber;
        int32_t linePosition;
        tokenType type;
        std::string value;
    };
}
