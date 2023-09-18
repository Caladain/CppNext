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
        }
        return {};
    }
}