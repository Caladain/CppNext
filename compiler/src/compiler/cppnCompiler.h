#pragma once
#include "lexer/lexer.h"
#include "parser/parser.h"
#include <cxxopts.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>
namespace cppnext {
    
    class cppnCompiler {
    public:
        cppnCompiler();
        ~cppnCompiler();
        cppnCompiler(const cppnCompiler&) = delete;
        cppnCompiler& operator= (const cppnCompiler&) = delete;
        void ProcessFiles(const cxxopts::ParseResult& commandLineOptions);
    private:
        std::unique_ptr<cppnext::Lexer> lexer;
        std::unique_ptr<cppnext::Parser> parser;
    };
}