#pragma once
#include "lexer/lexer.h"

#include <cxxopts.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>
namespace cppnext::compiler {
    
    class cppnCompiler {
    public:
        cppnCompiler();
        ~cppnCompiler();
        cppnCompiler(const cppnCompiler&) = delete;
        cppnCompiler& operator= (const cppnCompiler&) = delete;
        void ProcessFiles(const std::vector<std::string>& rawCommandLineFilePaths, const cxxopts::ParseResult& commandLineOptions);
    private:
        std::unique_ptr<cppnext::lexer::Lexer> lexer;
    };
}