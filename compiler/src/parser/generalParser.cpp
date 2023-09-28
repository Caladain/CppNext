#include "parser/generalParser.h"
#include "parser/node.h"
#include "fmt/core.h"
#include <magic_enum.hpp>
namespace cppnext
{
    std::optional<parser::Node> GeneralParser::ConsumeParse(const std::vector<Token>& tokens, int32_t& position)
    {
        auto token = tokens[position];
        switch (token.type)
        {
            case tokenType::Keyword_import:
            {
                auto result = importNodeParser.ConsumeParse(tokens, position);
                if (result)
                {
                    auto value = result.value();
                    return value;
                }
                else
                {
                    //throw;
                }
                break;
            }
            case tokenType::Keyword_class:
            case tokenType::Keyword_struct:
            {
                auto result = structNodeParser.ConsumeParse(tokens, position);
                if (result)
                {
                    auto value = result.value();
                    return value;
                }
                else
                {
                    //throw;
                }
                break;
            }
            case tokenType::Keyword_namespace:
            {
                auto result = namespaceNodeParser.ConsumeParse(tokens, position);
                if (result)
                {
                    auto value = result.value();
                    return value;
                }
                else
                {
                    //throw;
                }
                break;
            }
            case tokenType::AlphaNumeric:
            case tokenType::Keyword_bool:
            case tokenType::Keyword_char8:
            case tokenType::Keyword_char16: 
            case tokenType::Keyword_char32:
            case tokenType::Keyword_int8:
            case tokenType::Keyword_int16:
            case tokenType::Keyword_int32:
            case tokenType::Keyword_int64:
            case tokenType::Keyword_uint8:
            case tokenType::Keyword_uint16:
            case tokenType::Keyword_uint32:
            case tokenType::Keyword_uint64:
            case tokenType::Keyword_string:
            case tokenType::Keyword_float16:
            case tokenType::Keyword_float32:
            case tokenType::Keyword_float64:
            case tokenType::Keyword_void:
            case tokenType::Keyword_auto:
            case tokenType::Keyword_mutable:
            {
                auto variableParseResult = variableNodeParser.ConsumeParse(tokens,position);
                if (variableParseResult)
                {
                    auto value = variableParseResult.value();
                    return value;
                }
                else
                {
                    //Check if it's a function instead

                }
                break;
            }

        }
        return {};
    }
}