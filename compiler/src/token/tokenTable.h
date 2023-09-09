#pragma once
#include <unordered_map>
#include <string>
namespace cppnext
{
    enum tokenType
    {
        Unknown = 0,
        OpenBrace,         //{
        CloseBrace,        //}
        OpenBracket,       //[
        CloseBracket,      //]
        OpenParen,         //(
        CloseParen,        //)
        Backtick,          //`
        Tilde,             //~
        Bang,              //!
        At,                //@
        Hash,              //#
        DollarSign,        //$
        Percent,           //%
        Caret,             //^
        Ampersand,         //&
        Star,              //*
        Minus,             //-
        Underline,         //_
        Equals,            //=
        Plus,              //+
        Backslash,         //
        Pipe,              //|
        Semicolon,         //;
        Colon,             //:
        SingleQuote,       //'
        DoubleQuote,       //"
        Comma,             //,
        OpenAngleBracket,  //<
        Period,            //.
        CloseAngleBracket, //>
        ForwardSlash,      ///
        QuestionMark,      //?
        ArrowOperator,     //->
        AlphaNumeric,      //Starts with _AZaz, may contain AlphaNumeric and _, must not contain other special characters, will not be a reserved keyword.
        NumericAlpha,      //Starts with a number, may end with AlphaNumeric to indicate the type.
        Keyword_bool,      //bool type
        Keyword_catch,     //catch keyword
        Keyword_char8,     //char8 keyword c8 also accepted
        Keyword_char16,    //char16 keyword c16 also accepted
        Keyword_char32,    //char32 keyword c32 also accepted
        Keyword_float16,   //float16 keyword f16 also accepted
        Keyword_float32,   //float32 keyword f32 also accepted
        Keyword_float64,   //float64 keyword f64 also accepted double also accepted d also accepted
        //Keyword_float128,  //float128 keyword, f128 also accepted double128 also accepted d128 also accepted
        Keyword_int8,     //int8 keyword, i8 also accepted
        Keyword_int16,     //int16 keyword, i16 also accepted
        Keyword_int32,     //int32 keyword, i32 also accepted
        Keyword_int64,     //int64 keyword, i64 also accepted
        Keyword_uint8,    //uint8 keyword, u8 also accepted
        Keyword_uint16,    //uint16 keyword, u16 also accepted
        Keyword_uint32,    //uint32 keyword, u32 also accepted
        Keyword_uint64,    //uint64 keyword, u64 also accepted
        Keyword_string,    //string keyword
        Keyword_break,     //break keyword
        Keyword_case,      //case keyword
        Keyword_continue,  //continue keyword
        Keyword_if,        //if keyword
        Keyword_else,      //else keyword
        Keyword_mutable,   //mutable keyword, mut is also accepted
        Keyword_for,       //for keyword
        Keyword_namespace, //namespace keyword
        Keyword_new,       //new keyword
        Keyword_operator,  //operator keyword
        Keyword_private,   //private keyword
        Keyword_public,    //public keyword
        Keyword_trait,     //trait keyword
        Keyword_return,    //return keyword
        Keyword_struct,    //struct keyword
        Keyword_switch,    //switch keyword
        Keyword_throw,     //throw keyword
        Keyword_try,       //try keyword
        Keyword_using,     //using keyword
        Keyword_void,      //void keyword
        Keyword_while,     //while keyword
        Keyword_override,  //override keyword
        Keyword_final,     //final keyword
        Keyword_import,    //#import keyword
        Keyword_class,     //class
        Keyword_auto,      //auto
        NamespaceSeperator,//::
        StringLiteral
    };

    static std::unordered_map<std::string,tokenType> tokenRepresentation {
        { "{", OpenBrace},
        { "}", CloseBrace},
        { "[", OpenBracket},
        { "]", CloseBracket},
        { "(", OpenParen},
        { ")", CloseParen},
        { "`", Backtick},
        { "~", Tilde},
        { "!", Bang},
        { "@", At},
        { "#", Hash},
        { "$", DollarSign},
        { "%", Percent},
        { "^", Caret},
        { "&", Ampersand},
        { "*", Star},
        { "-", Minus},
        { "_", Underline},
        { "=", Equals},
        { "+", Plus},
        { "\\", Backslash},
        { "|", Pipe},
        { ";", Semicolon},
        { ":", Colon},
        { "'", SingleQuote},
        { "\"", DoubleQuote},
        { ",", Comma},
        { "<", OpenAngleBracket},
        { ".", Period},
        { ">", CloseAngleBracket},
        { "/", ForwardSlash},
        { "?", QuestionMark},
        { "->", ArrowOperator},
        { "bool", Keyword_bool},
        { "catch", Keyword_catch},
        { "char8", Keyword_char8},
        { "c8", Keyword_char8 },
        { "char16", Keyword_char16},
        { "c16", Keyword_char16 },
        { "char32", Keyword_char32},
        { "c32", Keyword_char32 },
        { "float16", Keyword_float16},
        { "f16", Keyword_float16 },
        { "float32", Keyword_float32},
        { "f32", Keyword_float32 },
        { "float64", Keyword_float64},
        { "f64", Keyword_float64 },
        //{ "float128", Keyword_float128},
        //{ "f128", Keyword_float128 },
        { "int8", Keyword_int8},
        { "i8", Keyword_int8},
        { "int16", Keyword_int16},
        { "i16", Keyword_int16 },
        { "int32", Keyword_int32},
        { "i32", Keyword_int32 },
        { "int64", Keyword_int64},
        { "i64", Keyword_int64 },
        { "uint8", Keyword_uint8},
        { "u8", Keyword_uint8},
        { "uint16", Keyword_uint16},
        { "u16", Keyword_uint16 },
        { "uint32", Keyword_uint32},
        { "u32", Keyword_uint32 },
        { "uint64", Keyword_uint64},
        { "u64", Keyword_uint64 },
        { "break", Keyword_break},
        { "case", Keyword_case},
        { "continue", Keyword_continue},
        { "if", Keyword_if},
        { "else", Keyword_else},
        { "mut", Keyword_mutable},
        { "mutable", Keyword_mutable },
        { "for", Keyword_for},
        { "namespace", Keyword_namespace},
        { "new", Keyword_new},
        { "operator", Keyword_operator},
        { "private", Keyword_private},
        { "public", Keyword_public},
        { "pub", Keyword_public },
        { "trait", Keyword_trait},
        { "return", Keyword_return},
        { "struct", Keyword_struct},
        { "switch", Keyword_switch},
        { "throw", Keyword_throw},
        { "try", Keyword_try},
        { "using", Keyword_using},
        { "void", Keyword_void},
        { "while", Keyword_while},
        { "override", Keyword_override},
        { "final", Keyword_final},
        { "#import", Keyword_import},
        { "class", Keyword_class },
        { "auto", Keyword_auto },
        { "::", NamespaceSeperator },
    };
}
