#include "parser/namespaceNode.h"
#include "parser/node.h"
#include "parser/generalParser.h"
#include "fmt/core.h"
#include <magic_enum.hpp>
namespace cppnext
{
    std::optional<parser::Node> NamespaceNode::ConsumeParse(const std::vector<Token>& tokens, int32_t& position)
    {
        int32_t startPosition = position;
        NamespaceNodeData NodeData;
        bool foundNamespaceToken = false;
        bool foundNamespaceOpenBrace = false;
        bool foundNamespaceCloseBrace = false;
        bool foundNamespaceValidName = false;
        std::string namespaceName;
        parser::Node newNode;
        GeneralParser generalParser;
        newNode.type = parser::parseNodeType::Namespace;
        while (startPosition < tokens.size())
        {
            const auto& currentToken = tokens[startPosition];
            if (foundNamespaceOpenBrace && currentToken.type != tokenType::CloseBrace)
            {
                auto result = generalParser.ConsumeParse(tokens, startPosition);
                if (result)
                {
                    auto value = result.value();
                    newNode.childNodes.emplace_back(value);
                    startPosition++;
                    continue;
                }
            }
            if (currentToken.type == tokenType::Keyword_namespace)
            {
                if (foundNamespaceToken)
                {
                    //throw;
                }
                foundNamespaceToken = true;
                NodeData.tokens.push_back(currentToken);
                startPosition++;
                continue;
            }
            if (currentToken.type == tokenType::AlphaNumeric && foundNamespaceToken && !foundNamespaceOpenBrace)
            {
                foundNamespaceValidName = true;
                NodeData.tokens.push_back(currentToken);
                namespaceName += currentToken.value;
                startPosition++;
                continue;
            }
            if (currentToken.type == tokenType::NamespaceSeperator)
            {
                if (!foundNamespaceValidName || !foundNamespaceOpenBrace)
                {
                    //throw
                }
                NodeData.tokens.push_back(currentToken);
                namespaceName += "::";
                foundNamespaceValidName = false;
                startPosition++;
                continue;
            }
            if (currentToken.type == tokenType::OpenBrace)
            {
                if (!foundNamespaceValidName)
                {
                    //throw
                }
                NodeData.value = namespaceName;
                foundNamespaceOpenBrace = true;
                NodeData.tokens.push_back(currentToken);
                startPosition++;
                continue;
            }
            if (currentToken.type == tokenType::CloseBrace)
            {
                if (!foundNamespaceOpenBrace)
                {
                    //throw
                }
                foundNamespaceCloseBrace = true;
                NodeData.tokens.push_back(currentToken);
                //startPosition++;
                break;
            }
            
        }
        if (foundNamespaceOpenBrace && foundNamespaceCloseBrace)
        {                    
            newNode.nodeData = NodeData;
            position = startPosition;
            return newNode;
        }
        else
        {
            //Throw
        }
        return {};
    }
}