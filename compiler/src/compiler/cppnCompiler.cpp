#include "compiler/cppnCompiler.h"


namespace cppnext::compiler {
    cppnCompiler::cppnCompiler()
    {        
        lexer = std::make_unique<cppnext::lexer::Lexer>();
    }

    cppnCompiler::~cppnCompiler()
    {
    }

    void cppnCompiler::ProcessFiles(const cxxopts::ParseResult& commandLineOptions)
    {
        lexer->Lex(commandLineOptions);
    }
}