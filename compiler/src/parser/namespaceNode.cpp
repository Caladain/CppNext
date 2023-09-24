#include "parser/namespaceNode.h"
#include "parser/node.h"
#include "parser/generalParser.h"
#include "fmt/core.h"
#include <magic_enum.hpp>
namespace cppnext
{
    std::optional<parser::Node> NamespaceNode::ConsumeParse(const std::vector<Token>& tokens, int32_t& position)
    {
        if (position < tokens.size())
        {
            const auto& currentToken = tokens[position];
            if (currentToken.type == tokenType::Keyword_namespace)
            {
                NamespaceNodeData NodeData;                
                NodeData.tokens.push_back(currentToken);
                bool foundNamespaceOpenBrace = false;
                bool foundNamespaceCloseBrace = false;
                bool foundNamespaceValidName = false;
                std::string namespaceName;
                parser::Node newNode;
                GeneralParser generalParser;
                newNode.type = parser::parseNodeType::Namespace;
                while (position + 1 < tokens.size())
                {
                    const auto& peekToken = tokens[position + 1];                    
                    if (peekToken.type == tokenType::AlphaNumeric)
                    {
                        if (!foundNamespaceOpenBrace)
                        {
                            //throw
                        }
                        foundNamespaceValidName = true;
                        NodeData.tokens.push_back(peekToken);
                        namespaceName += peekToken.value;                        
                    }
                    if (peekToken.type == tokenType::NamespaceSeperator)
                    {
                        if (!foundNamespaceValidName || !foundNamespaceOpenBrace)
                        {
                            //throw
                        }
                        NodeData.tokens.push_back(peekToken);
                        namespaceName += "::";
                        foundNamespaceValidName = false;
                    }
                    if (peekToken.type == tokenType::OpenBrace)
                    {
                        if (!foundNamespaceValidName)
                        {
                            //throw
                        }
                        NodeData.value = namespaceName;
                        foundNamespaceOpenBrace = true;
                        NodeData.tokens.push_back(peekToken);
                    }
                    if (peekToken.type == tokenType::CloseBrace)
                    {
                        if (!foundNamespaceOpenBrace)
                        {
                            //throw
                        }
                        foundNamespaceCloseBrace = true;
                        NodeData.tokens.push_back(peekToken);
                        position++;
                        break;
                    }
                    if (foundNamespaceOpenBrace)
                    {
                        auto result = generalParser.ConsumeParse(tokens, position);
                        if (result)
                        {
                            auto value = result.value();
                            newNode.childNodes.emplace_back(value);
                        }
                    }
                    position++;
                }
                if (foundNamespaceOpenBrace && foundNamespaceCloseBrace)
                {                    
                    newNode.nodeData = NodeData;
                    return newNode;
                }
                else
                {
                    //Throw
                }
            }
        }
        return {};
    }
}