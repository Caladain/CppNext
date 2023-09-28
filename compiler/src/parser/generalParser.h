#pragma once
#include <vector>

#include <cxxopts.hpp>

#include "parser/importNode.h"
#include "parser/namespaceNode.h"
#include "parser/variableNode.h"
#include "parser/structNode.h"
#include "token/token.h"

namespace cppnext
{    
    namespace parser
    {
        struct Node;
    }
    class GeneralParser{
    public:
        std::optional<parser::Node> ConsumeParse(const std::vector<Token>& tokens, int32_t& position);
    private:
        ImportNode importNodeParser;
        NamespaceNode namespaceNodeParser;
        VariableNode variableNodeParser;
        StructNode structNodeParser;
    };
}