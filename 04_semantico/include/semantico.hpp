#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "sintactico.hpp"
#include "generic.hpp"

struct Value
{
	bool valid;
	bool isFloat;
	float value;

	Value operator+(const Value& other);
	Value operator-(const Value& other);
	Value operator*(const Value& other);
	Value operator/(const Value& other);
};

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
	std::vector<Value> values;
	bool isArray;
};

void semanticAnalysis(const std::shared_ptr<NodeProgram>& program);
