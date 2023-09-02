#include <cxxopts.hpp>

#include <filesystem>
#include <vector>
#include <string>
#include <memory>

namespace cppnext::lexer {

    enum tokenType
    {
        Unknown = 0,
        OpenBrace = 1,          //{
        CloseBrace = 2,         //}
        OpenBracket = 3,        //[
        CloseBracket = 4,       //]
        OpenParen = 5,          //(
        CloseParen = 6,         //)
        Backtick = 7,           //`
        Tilde = 8,              //~
        Bang = 9,               //!
        At = 10,                //@
        Hash = 11,              //#
        DollarSign = 12,        //$
        Percent = 13,           //%
        Caret = 14,             //^
        Ampersand = 15,         //&
        Star = 16,              //*
        Minus = 17,             //-
        Underline = 18,         //_
        Equals = 19,            //=
        Plus = 20,              //+
        Backslash = 21,         //
        Pipe = 22,              //|
        Semicolon = 23,         //;
        Colon = 24,             //:
        SingleQuote = 25,       //'
        DoubleQuote = 26,       //"
        Comma = 27,             //,
        OpenAngleBracket = 28,  //<
        Period = 29,            //.
        CloseAngleBracket = 30, //>
        ForwardSlash = 31,      ///
        QuestionMark = 32,      //?
        AlphaNumeric = 33,      //Starts with _AZaz, may contain AlphaNumeric and _, must not contain other special characters, will not be a reserved keyword.
        NumericAlpha = 34,      //Starts with a number, may end with AlphaNumeric to indicate the type.
        Keyword_bool = 35,      //bool type
        Keyword_catch = 36,     //catch keyword
        Keyword_char8 = 37,     //char8 keyword c8 also accepted
        Keyword_char16 = 38,    //char16 keyword c16 also accepted
        Keyword_char32 = 39,    //char32 keyword c32 also accepted
        Keyword_float16 = 40,   //float16 keyword f16 also accepted
        Keyword_float32 = 41,   //float32 keyword f32 also accepted
        Keyword_float64 = 42,   //float64 keyword f64 also accepted double also accepted d also accepted
        Keyword_float128 = 43,  //float128 keyword, f128 also accepted double128 also accepted d128 also accepted
        Keyword_int16 = 44,     //int16 keyword, i16 also accepted
        Keyword_int32 = 45,     //int32 keyword, i32 also accepted
        Keyword_int64 = 46,     //int64 keyword, i64 also accepted
        Keyword_uint16 = 47,    //uint16 keyword, u16 also accepted
        Keyword_uint32 = 48,    //uint32 keyword, u32 also accepted
        Keyword_uint64 = 49,    //uint64 keyword, u64 also accepted
        Keyword_break = 50,     //break keyword
        Keyword_case = 51,      //case keyword
        Keyword_continue = 52,  //continue keyword
        Keyword_if = 53,        //if keyword
        Keyword_else = 54,      //else keyword
        Keyword_mutable = 55,   //mutable keyword, mut is also accepted
        Keyword_for = 56,       //for keyword
        Keyword_namespace = 57, //namespace keyword
        Keyword_new = 58,       //new keyword
        Keyword_operator = 59,  //operator keyword
        Keyword_private = 60,   //private keyword
        Keyword_public = 61,    //public keyword
        Keyword_trait = 62,     //trait keyword
        Keyword_return = 63,    //return keyword
        Keyword_struct = 64,    //struct keyword
        Keyword_switch = 65,    //switch keyword
        Keyword_throw = 66,     //throw keyword
        Keyword_try = 67,       //try keyword
        Keyword_using = 68,     //using keyword
        Keyword_void = 69,      //void keyword
        Keyword_while = 70,     //while keyword
        Keyword_override = 71,  //override keyword
        Keyword_final = 72,     //final keyword
    };
    struct token
    {
        int32_t lineNumber;
        int32_t linePosition;
        
    };
    struct lexedFile
    {
        std::filesystem::path originalPath;
        std::vector<std::string> originalLines;
        std::vector<token> tokens;
    };
        
    class Lexer {
    public:
        Lexer();
        ~Lexer();
        Lexer(const Lexer&) = delete;
        Lexer& operator= (const Lexer&) = delete;
        void Lex(const std::vector<std::string>& rawCommandLineFilePaths, const cxxopts::ParseResult& commandLineOptions);
        std::vector<lexedFile>* GetLexedFiles();
    private:
        void ProcessFilePaths(const std::vector<std::string>& rawCommandLineFilePaths, const cxxopts::ParseResult& commandLineOptions);
        void LexFile(lexedFile& fileToLex, const cxxopts::ParseResult& commandLineOptions);
        void LexLine(int32_t lineNumber, std::vector<token>& tokenStream, const std::string& lineToLex, const cxxopts::ParseResult& commandLineOptions);
        std::unique_ptr<std::vector<lexedFile>> lexedFiles =  nullptr;
    };
}