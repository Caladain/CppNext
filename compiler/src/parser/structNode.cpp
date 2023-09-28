#include "parser/structNode.h"
#include "parser/node.h"
#include "parser/generalParser.h"
#include "fmt/core.h"
#include <magic_enum.hpp>
namespace cppnext
{
    std::optional<parser::Node> StructNode::ConsumeParse(const std::vector<Token>& tokens, int32_t& position)
    {
        int32_t startPosition = position;
        if (startPosition < tokens.size())
        {
            StructNodeData NodeData;
            bool foundOpenBrace = false;
            bool foundClass = false;
            bool foundCloseBrace = false;
            bool foundValidIdentifier = false;
            parser::Node newNode;
            GeneralParser generalParser;
            newNode.type = parser::parseNodeType::Struct;
            while (startPosition < tokens.size())
            {
                const auto& currentToken = tokens[startPosition];
                if (currentToken.type == tokenType::Keyword_class || currentToken.type == tokenType::Keyword_struct)
                {
                    if (foundClass || foundValidIdentifier)
                    {
                        //throw
                    }
                    foundClass = true;
                    NodeData.isStruct = currentToken.type == tokenType::Keyword_struct ? true : false;
                    NodeData.tokens.push_back(currentToken);
                }
                if (currentToken.type == tokenType::AlphaNumeric)
                {
                    if (!foundOpenBrace || !foundClass)
                    {
                        //throw
                    }
                    foundValidIdentifier = true;
                    NodeData.tokens.push_back(currentToken);
                    NodeData.identifier = currentToken.value;
                        
                }
                if (currentToken.type == tokenType::OpenBrace)
                {
                    if (!foundValidIdentifier)
                    {
                        //throw
                    }
                    foundOpenBrace = true;
                    NodeData.tokens.push_back(currentToken);
                }
                if (currentToken.type == tokenType::CloseBrace)
                {
                    if (!foundOpenBrace)
                    {
                        //throw
                    }
                    foundCloseBrace = true;
                    NodeData.tokens.push_back(currentToken);
                    startPosition++;
                    break;
                }
                if (foundOpenBrace)
                {
                    auto result = generalParser.ConsumeParse(tokens, startPosition);
                    if (result)
                    {
                        auto value = result.value();
                        newNode.childNodes.emplace_back(value);
                    }
                }
                startPosition++;
            }
            if (foundOpenBrace && foundCloseBrace)
            {
                newNode.nodeData = NodeData;
                return newNode;
            }
            else
            {
                //Throw
            }
        }
        return {};
    }
}