#pragma once
#include "token/token.h"
#include <filesystem>
#include <vector>
#include <string>
#include <memory>
namespace cppnext {
	struct lexedFile
	{
		std::filesystem::path originalPath;
		std::vector<std::string> originalLines;
		int32_t fileIndex{ 0 };
		std::vector<Token> tokens;
	};
	
	static std::unique_ptr<std::vector<lexedFile>> lexedFiles = nullptr;
}
