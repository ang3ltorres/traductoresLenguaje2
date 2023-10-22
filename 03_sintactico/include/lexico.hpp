#pragma once
#include <string>
#include <vector>
#include <ostream>

extern const std::vector<std::pair<std::string, std::string>> tokenTypes;

class Token
{
public:
	enum class Type : int
	{
		ReservedWordTrue = 0,
		ReservedWordFalse,
		ReservedWordReturn,
		ReservedWordDo,
		ReservedWordWhile,
		ReservedWordFor,
		ReservedWordBreak,
		ReservedWordSwitch,
		ReservedWordIf,
		ReservedWordElse,

		DataTypeVoid,
		DataTypeChar,
		DataTypeShort,
		DataTypeInt,
		DataTypeLong,
		DataTypeFloat,
		DataTypeBool,

		Addition,
		Subtraction,
		Increment,
		Decrement,
		AdditionAssignment,
		SubtractionAssignment,
		Multiplication,
		Division,
		MultiplicationAssignment,
		DivisionAssignment,
		Modulus,
		ModulusAssignment,

		BitwiseAND,
		LogicalAND,
		BitwiseOR,
		LogicalOR,
		GreaterThan,
		LessThan,
		GreaterThanOrEqual,
		LessThanOrEqual,
		EqualTo,
		NotEqualTo,
		Negation,
		RightShift,
		LeftShift,
		Assignment,
		DollarSign,

		BraceOpen,
		BraceClose,
		ParenthesisOpen,
		ParenthesisClose,
		BracketOpen,
		BracketClose,
		Comma,
		Semicolon,
		
		Identifier,
		Number,
		FloatingPointNumber,
		String,
		Unknown,
	};

	Type type;
	std::string lexema;
	unsigned int line;

	std::string getTypeString() const;

	friend std::ostream& operator<<(std::ostream& os, const Token& token);
};

std::vector<Token> getTokens(const std::string& code);

