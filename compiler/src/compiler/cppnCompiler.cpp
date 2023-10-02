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
        bool runLexer = true;
        bool runParser = true;
        if (commandLineOptions.count("lexeronly"))
        {
            runParser = false;
        }
        /*if (commandLineOptions.count("parseronly"))
        {

        }*/
        if (runLexer)
        {
            lexer->Lex(commandLineOptions);
        }
        if(runParser)
        {
            parser->Parse(commandLineOptions);
        }
    }
}