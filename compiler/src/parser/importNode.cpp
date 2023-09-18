#include "parser/importNode.h"
#include "parser/node.h"
#include "fmt/core.h"
#include <magic_enum.hpp>
namespace cppnext
{
    std::optional<parser::Node> ImportNode::ConsumeParse(const std::vector<Token>& tokens, int32_t& position)
    {
        if (position < tokens.size())
        {
            const auto& currentToken = tokens[position];
            if (currentToken.type == tokenType::Keyword_import)
            {
                ImportNodeData NodeData;                
                NodeData.tokens.push_back(currentToken);
                int32_t importTokenLineNumber = currentToken.lineNumber;
                bool foundSomeFormOfImportPath = false;
                while (position + 1 < tokens.size())
                {
                    const auto& peekToken = tokens[position + 1];
                    if (peekToken.lineNumber != importTokenLineNumber)
                    {
                        break;
                    }
                    if (peekToken.type != tokenType::AlphaNumeric && peekToken.type != tokenType::ForwardSlash)
                    {
                        //Throw
                    }
                    foundSomeFormOfImportPath = true;
                    NodeData.tokens.push_back(peekToken);
                    if (peekToken.type == tokenType::ForwardSlash)
                    {
                        NodeData.value += "/";
                    }
                    else
                    {
                        NodeData.value += std::get<std::string>(peekToken.value);
                    }
                    position++;
                }
                if (foundSomeFormOfImportPath)
                {
                    parser::Node newNode;
                    newNode.type = parser::parseNodeType::Import;
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