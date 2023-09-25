#pragma once
#include <array>
#include <unordered_map>
#include <string>
namespace cppnext
{
    static void ThrowError(int32_t errorNumber, std::string error)
    {
        fmt::print("{}", error);
        throw errorNumber;
    }

    static constexpr std::array ErrorTable = {
        "Error 0000: Placeholder.",
        "Error 0001: File {} Does not exist.\n",
        "Error 0002: Lexed {} without an opening {} ? \n",
        "Error 0003: Lexed a mismatch of {} and {}. Printing all seen {} and {} that are potentially mismatched.\n",
        "Error 0004: Parsed a uint8 as value {}, which is greater than uint8's max value of {}\n"
    };
}