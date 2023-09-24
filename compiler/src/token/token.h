#pragma once
#include "token/tokenTable.h"
#include <string>
#include <stdint.h>
#include <variant>
#include <magic_enum.hpp>
#include "fmt/core.h"

namespace cppnext
{
    struct Token
    {
        int32_t fileIndex{ 0 };
        int32_t lineNumber{ 0 };
        int32_t linePosition{ 0 };
        tokenType type;
        std::string value;

        void SetValue(const tokenType& newType, const std::string& valueString)
        {
            type = newType;
            switch (type)
            {
                case tokenType::AlphaNumeric:
                case tokenType::NumericAlpha:
                case tokenType::StringLiteral:
                {
                    value = valueString;
                    break;
                }                
            };
        }
        std::string toString() const
        {
            switch (type)
            {
                case tokenType::AlphaNumeric:
                case tokenType::NumericAlpha:
                case tokenType::StringLiteral:
                {
                    return fmt::format("{}:{}", magic_enum::enum_name(type), value);
                    break;
                }
            };
            return fmt::format("{}", magic_enum::enum_name(type));
        }
    };
}
