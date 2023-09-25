#include "parser/variableNode.h"
#include "parser/node.h"
#include "fmt/core.h"
#include <magic_enum.hpp>
#pragma warning( push )
#pragma warning( disable : 4702)
#include <ctre-unicode.hpp>
#pragma warning( pop )
namespace cppnext
{
    std::optional<parser::Node> VariableNode::ConsumeParse(const std::vector<Token>& tokens, int32_t& position)
    {
        int32_t startPosition = position;
        bool foundMutable = false;
        bool foundType = false;
        bool foundIdentifer = false;
        bool foundOpenBrace = false;
        bool foundCloseBrace = false;
        bool foundCloseSemiColon = false;
        parser::Node newNode;
        newNode.type = cppnext::parser::parseNodeType::Variable;
        VariableNodeData NodeData;
        while (startPosition < tokens.size())
        {         
            const auto& currentToken = tokens[startPosition];
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
                NodeData.typeValue = currentToken.value;
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
                        NodeData.tokens.push_back(currentToken);
                        foundCloseBrace = true;
                        startPosition++;
                        break;
                    }
                    fullExpressionString += peekToken.value;
                    NodeData.tokens.push_back(peekToken);
                    startPosition++;
                }
                //Now, lets figure out what to do with the entire string.
                //It could be one of a few things.  It could match the type
                //for the node type (uint8 paired with 9 for example).
                //It could *not* match and be an error (uint8 paired with -9)
                //It could be a compile time expression (uint8 myvar{3+4})
                //It could be a user defined literal as well, which will need to be
                //parsed later during variable type resolution.
                //
                //Lets first see if it pairs nicely with the type.
                if (NodeData.type == tokenType::Keyword_uint8
                    || NodeData.type == tokenType::Keyword_uint16
                    || NodeData.type == tokenType::Keyword_uint32
                    || NodeData.type == tokenType::Keyword_uint64)
                {
                    //We do a digit only check first, then we'll check the ranges.
                    if (ctre::match<"^\\d+[u|U]?$">(fullExpressionString))
                    {
                        //We match digits followed by an optional u or U.
                        uint64_t value = std::stoull(fullExpressionString);
                        newNode.value = value;
                        if (NodeData.type == tokenType::Keyword_uint8 && value > std::numeric_limits<uint8_t>::max())
                        {
                            //throw
                        }
                        if (NodeData.type == tokenType::Keyword_uint16 && value > std::numeric_limits<uint16_t>::max())
                        {
                            //throw
                        }
                        if (NodeData.type == tokenType::Keyword_uint32 && value > std::numeric_limits<uint32_t>::max())
                        {
                            //throw
                        }                        
                        continue;
                    }
                }
                continue;
            }
            //if ((currentToken.type == tokenType::AlphaNumeric || currentToken.type == tokenType::NumericAlpha || currentToken.type == tokenType::StringLiteral || currentToken.type == tokenType::CharacterLiteral) && foundType && foundIdentifer)
            //{
            //    std::string valueToBeConverted;
            //    if (minusFound)
            //    {
            //        valueToBeConverted += "-";
            //    }
            //    valueToBeConverted += currentToken.value;
            //    if (foundValue)
            //    {
            //        return {};
            //    }
            //    if (currentToken.type == tokenType::AlphaNumeric)
            //    {
            //        if (NodeData.type == tokenType::Keyword_bool)
            //        {
            //            if (currentToken.value == "true")
            //            {
            //                newNode.value = true;
            //            }
            //            else if (currentToken.value == "false")
            //            {
            //                newNode.value = false;
            //            }
            //            else
            //            {
            //                //throw
            //            }
            //        }
            //        else
            //        {
            //            newNode.value = valueToBeConverted;
            //        }                    
            //    }
            //    if (currentToken.type == tokenType::StringLiteral)
            //    {
            //        if (NodeData.type != tokenType::Keyword_string)
            //        {
            //            //throw
            //        }
            //        newNode.value = currentToken.value;
            //    }
            //    if (currentToken.type == tokenType::CharacterLiteral)
            //    {
            //        if (NodeData.type != tokenType::Keyword_char8 && NodeData.type != tokenType::Keyword_char16 && NodeData.type != tokenType::Keyword_char32)
            //        {
            //            //throw
            //        }
            //        newNode.value = currentToken.value;
            //    }
            //    if (currentToken.type == tokenType::NumericAlpha)
            //    {
            //        try
            //        {
            //            if (NodeData.type == tokenType::Keyword_uint8
            //             || NodeData.type == tokenType::Keyword_uint16
            //             || NodeData.type == tokenType::Keyword_uint32
            //             || NodeData.type == tokenType::Keyword_uint64)
            //            {
            //                if (minusFound)
            //                {
            //                    //throw
            //                }
            //                newNode.value = std::stoull(valueToBeConverted);
            //            }
            //            if (NodeData.type == tokenType::Keyword_int8
            //             || NodeData.type == tokenType::Keyword_int16
            //             || NodeData.type == tokenType::Keyword_int32
            //             || NodeData.type == tokenType::Keyword_int64)
            //            {
            //                newNode.value = std::stoll(valueToBeConverted);
            //            }
            //            if (NodeData.type == tokenType::Keyword_float16
            //             || NodeData.type == tokenType::Keyword_float32
            //             || NodeData.type == tokenType::Keyword_float64)
            //            {
            //                newNode.value = std::stod(valueToBeConverted);
            //            }
            //        }
            //        catch (...)
            //        {
            //            //throw
            //        }
            //            
            //        //newNode.value = currentToken.value;
            //    }
            //    
            //    NodeData.tokens.push_back(currentToken);
            //    foundValue = true;
            //    startPosition++;
            //    continue;
            //}
            //if (currentToken.type == tokenType::CloseBrace && foundValue)
            //{
            //    
            //}
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