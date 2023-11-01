#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "sintactico.hpp"
#include "generic.hpp"

struct SymbolInfo
{
	enum class SymbolType : int
	{
		Declaration = 0,
		DeclarationInnitialization = 0,
		Assignment,
		Argument,
		If,
	};

	unsigned int scopeId;
	unsigned int line;
	DataType type;
	SymbolType symbolType;
};

void semanticAnalysis(const std::shared_ptr<NodeProgram>& program);
