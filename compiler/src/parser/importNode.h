#pragma once
#include "parser/parseNodeInterface.h"
#include "token/token.h"
#include <vector>


namespace cppnext
{
    struct ImportNodeData
    {
        std::vector<Token> tokens;
        std::string value;
    };
    namespace parser
    {
        struct Node;
    }    
    class ImportNode : public ParseNode{
    public:
        virtual std::optional<parser::Node> ConsumeParse(const std::vector<Token>& tokens, int32_t& position) override;
    };
}