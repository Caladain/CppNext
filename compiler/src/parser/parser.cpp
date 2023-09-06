#include "parser/parser.h"

namespace cppnext
{
    Parser::Parser(Lexer* lexerToUse)
    {
        lexer = lexerToUse;
        if (lexer == nullptr)
        {
            //throw
        }
    }

    void Parser::Parse()
    {

    }
}