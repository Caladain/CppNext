#include "compiler/cppnCompiler.h"


namespace cppnext
{
    cppnCompiler::cppnCompiler()
    {        
        lexer = std::make_unique<cppnext::Lexer>();
        parser = std::make_unique<cppnext::Parser>(lexer.get()); //cppnCompiler will manage the lifetime of the lexer and parser
    }

    cppnCompiler::~cppnCompiler()
    {
    }

    void cppnCompiler::ProcessFiles(const cxxopts::ParseResult& commandLineOptions)
    {
        lexer->Lex(commandLineOptions);
        parser->Parse(commandLineOptions);
    }
}