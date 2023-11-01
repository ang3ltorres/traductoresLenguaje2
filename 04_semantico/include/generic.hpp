#pragma once

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
