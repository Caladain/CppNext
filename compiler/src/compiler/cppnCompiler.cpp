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

    int cppnCompiler::ProcessCommandlineArguments(int argc, char** argv)
    {
        //Lets do the command line song and dance, then kick things off to the compiler.
        cxxopts::Options options("CppNext Compiler", "Experimental Compiler for the CppNext project.");
        options.add_options()
            ("h,help", "Print out all command line options")
            ("s,sourceFolder", "Set the source folder for relative paths in the Input Files", cxxopts::value<std::string>())
            ("r,recursive", "Ignore -f if -s exists, recursively dive down SourceFolder for all .cppn files.")
            ("f,files", "CppNext Input File(s)", cxxopts::value<std::vector<std::string>>())
            ("o,output", "CppNext Output Folder", cxxopts::value<std::string>())
            ("v,verbose", "Verbose Output", cxxopts::value<bool>()->default_value("false"))
            ("lexeronly", "Only Run The Lexer")
            ("lexerdebug", "Output Lexer Debugging information")
            ("lexerdebugtokens", "Output Lexer Tokens for Debugging")
            ("parserdebug", "Output Parser Debugging information")
            ("parseronly", "Run the Lexer, followed by the Parser only.")
            ;
        options.parse_positional({ "files" });
        auto result = options.parse(argc, argv);
        if (result.count("help"))
        {
            fmt::print("{}\n", options.help());
            return 0;
        }
        ProcessFiles(result);
        return 1;
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