#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "sintactico.hpp"
#include "generic.hpp"

extern std::vector<ErrorCode> semanticErrors;

struct Value
{
	bool valid;
	bool isFloat;
	float value;

	Value operator+(const Value& other);
	Value operator-(const Value& other);
	Value operator*(const Value& other);
	Value operator/(const Value& other);
	
	Value operator>(const Value& other);
	Value operator<(const Value& other);
	Value operator>=(const Value& other);
	Value operator<=(const Value& other);
	Value operator==(const Value& other);
	Value operator!=(const Value& other);
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

	// ARRAY
	std::vector<Value> values;
	bool isArray;

	// FUNCTION
	enum class FunctionType : int
	{
		None,
		Declaration,
		Implementation
	};

	FunctionType functionType;
	std::vector<NodeArgument> parameters; // Parameters
	std::vector<Node> statements; // Statements
};

void semanticAnalysis(const std::shared_ptr<NodeProgram>& program);
