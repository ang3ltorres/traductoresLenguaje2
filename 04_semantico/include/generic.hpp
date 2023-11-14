#pragma once

#include <tuple>
#include <format>
#include <string>
#include <tuple>

#include "lexico.hpp"

enum class DataType : int
{
	None = 0,
	Int,
	Float,
	Long,
	Char,
	Short,
	Void,
	Double,
	Bool,
};

class ErrorCode : public std::exception
{
public:
	ErrorCode(unsigned int line, std::string errorStr);

	unsigned int line;
	std::string errorStr;

	const char* what() const noexcept override;
};

#ifdef PYTHON_LIB

/* LEXICO */
	// std::vector<Token>
/* SINTACTICO */
	// ErrorCode
/* SEMANTICO */
	// std::vector<ErrorCode>

std::tuple
<
	std::vector<Token>,
	ErrorCode,
	std::vector<ErrorCode>
>
parse(const std::string& code);

#endif
