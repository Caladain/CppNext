#include "parser/functionNode.h"
#include "parser/node.h"
#include "fmt/core.h"
#include <magic_enum.hpp>
#pragma warning( push )
#pragma warning( disable : 4702)
#include <ctre-unicode.hpp>
#pragma warning( pop )
namespace cppnext
{
    std::optional<parser::Node> FunctionNode::ConsumeParse(const std::vector<Token>& tokens, int32_t& position)
    {
        int32_t startPosition = position;
        bool foundPubPri = false;
        bool foundMutable = false;
        bool foundType = false;
        bool foundIdentifer = false;
        //bool foundOpenParen = false;
        //bool foundCloseParen = false;
        bool foundOpenBrace = false;
        bool foundCloseBrace = false;
        bool foundCloseSemiColon = false;
        parser::Node newNode;
        newNode.type = cppnext::parser::parseNodeType::Function;
        FunctionNodeData NodeData;
        while (startPosition < tokens.size())
        {         
            const auto& currentToken = tokens[startPosition];
            if (currentToken.type == tokenType::Keyword_public || currentToken.type == tokenType::Keyword_private)
            {
                if (foundPubPri || foundMutable || foundType)
                {
                    return {};
                }
                foundPubPri = true;
                NodeData.publicFunction = currentToken.type == tokenType::Keyword_public ? true : false;
                NodeData.tokens.push_back(currentToken);
                startPosition++;
                continue;
            }            
            if (currentToken.type == tokenType::Keyword_mutable)
            {
                if (foundMutable || foundType)
                {
                    return {};
                }
                foundMutable = true;
                NodeData.mutableVariable = true;
                NodeData.tokens.push_back(currentToken);
                startPosition++;
                continue;
            }
            if (currentToken.type == tokenType::Keyword_auto
                || currentToken.type == tokenType::Keyword_void
                || currentToken.type == tokenType::Keyword_bool
                || currentToken.type == tokenType::Keyword_int8
                || currentToken.type == tokenType::Keyword_int16
                || currentToken.type == tokenType::Keyword_int32
                || currentToken.type == tokenType::Keyword_int64
                || currentToken.type == tokenType::Keyword_uint8
                || currentToken.type == tokenType::Keyword_uint16
                || currentToken.type == tokenType::Keyword_uint32
                || currentToken.type == tokenType::Keyword_uint64
                || currentToken.type == tokenType::Keyword_float16
                || currentToken.type == tokenType::Keyword_float32
                || currentToken.type == tokenType::Keyword_float64
                || currentToken.type == tokenType::Keyword_char8
                || currentToken.type == tokenType::Keyword_char16
                || currentToken.type == tokenType::Keyword_char32
                || currentToken.type == tokenType::Keyword_string)
            {
                if (foundType)
                {
                    return {};
                }
                foundType = true;
                NodeData.type = currentToken.type;
                NodeData.tokens.push_back(currentToken);
                startPosition++;
                continue;
            }
            if (currentToken.type == tokenType::AlphaNumeric && !foundType)
            {
                if (foundIdentifer)
                {
                    return {};
                }
                foundType = true;
                NodeData.type = currentToken.type;
                NodeData.typeString = currentToken.value;
                NodeData.tokens.push_back(currentToken);
                startPosition++;
                continue;
            }
            if (currentToken.type == tokenType::AlphaNumeric && foundType && !foundIdentifer)
            {
                if (foundOpenBrace)
                {
                    return {};
                }
                NodeData.identifer = currentToken.value;
                NodeData.tokens.push_back(currentToken);
                foundIdentifer = true;
                startPosition++;
                continue;
            }
            if (currentToken.type == tokenType::OpenBrace)
            {
                if (foundOpenBrace)
                {
                    return {};
                }
                NodeData.tokens.push_back(currentToken);
                foundOpenBrace = true;
                startPosition++;
                continue;
            }
            if (foundOpenBrace && !foundCloseBrace)
            {                
                std::string fullExpressionString;
                while (startPosition < tokens.size())
                {
                    const auto& peekToken = tokens[startPosition];
                    if (peekToken.type == tokenType::CloseBrace)
                    {
                        if (foundCloseBrace)
                        {
                            return {};
                        }
                        NodeData.tokens.push_back(peekToken);
                        foundCloseBrace = true;
                        startPosition++;
                        break;
                    }
                    fullExpressionString += peekToken.value;
                    NodeData.tokens.push_back(peekToken);
                    startPosition++;
                }                
                continue;
            }
            if (currentToken.type == tokenType::Semicolon && foundCloseBrace)
            {
                if (foundCloseSemiColon)
                {
                    return {};
                }
                NodeData.tokens.push_back(currentToken);
                foundCloseSemiColon = true;
                break;
            }
            return {};
        }
        newNode.nodeData = NodeData;
        position = startPosition;
        return newNode;
    }
}