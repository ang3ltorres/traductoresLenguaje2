#include "lexico.hpp"
#include "sintactico.hpp"
#include "semantico.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

#ifdef PYTHON_LIB

	#include <pybind11/pybind11.h>
	#include <pybind11/stl.h>
	namespace py = pybind11;

	PYBIND11_MODULE(sintactico, m)
	{
		py::enum_<Token::Type>(m, "TokenType")
			.value("ReservedWordTrue", Token::Type::ReservedWordTrue)
			.value("ReservedWordFalse", Token::Type::ReservedWordFalse)
			.value("ReservedWordReturn", Token::Type::ReservedWordReturn)
			.value("ReservedWordDo", Token::Type::ReservedWordDo)
			.value("ReservedWordWhile", Token::Type::ReservedWordWhile)
			.value("ReservedWordFor", Token::Type::ReservedWordFor)
			.value("ReservedWordBreak", Token::Type::ReservedWordBreak)
			.value("ReservedWordSwitch", Token::Type::ReservedWordSwitch)
			.value("ReservedWordIf", Token::Type::ReservedWordIf)
			.value("DataTypeVoid", Token::Type::DataTypeVoid)
			.value("DataTypeChar", Token::Type::DataTypeChar)
			.value("DataTypeShort", Token::Type::DataTypeShort)
			.value("DataTypeInt", Token::Type::DataTypeInt)
			.value("DataTypeLong", Token::Type::DataTypeLong)
			.value("DataTypeFloat", Token::Type::DataTypeFloat)
			.value("DataTypeBool", Token::Type::DataTypeBool)
			.value("Addition", Token::Type::Addition)
			.value("Subtraction", Token::Type::Subtraction)
			.value("Increment", Token::Type::Increment)
			.value("Decrement", Token::Type::Decrement)
			.value("AdditionAssignment", Token::Type::AdditionAssignment)
			.value("SubtractionAssignment", Token::Type::SubtractionAssignment)
			.value("Multiplication", Token::Type::Multiplication)
			.value("Division", Token::Type::Division)
			.value("MultiplicationAssignment", Token::Type::MultiplicationAssignment)
			.value("DivisionAssignment", Token::Type::DivisionAssignment)
			.value("Modulus", Token::Type::Modulus)
			.value("ModulusAssignment", Token::Type::ModulusAssignment)
			.value("BitwiseAND", Token::Type::BitwiseAND)
			.value("LogicalAND", Token::Type::LogicalAND)
			.value("BitwiseOR", Token::Type::BitwiseOR)
			.value("LogicalOR", Token::Type::LogicalOR)
			.value("GreaterThan", Token::Type::GreaterThan)
			.value("LessThan", Token::Type::LessThan)
			.value("GreaterThanOrEqual", Token::Type::GreaterThanOrEqual)
			.value("LessThanOrEqual", Token::Type::LessThanOrEqual)
			.value("EqualTo", Token::Type::EqualTo)
			.value("NotEqualTo", Token::Type::NotEqualTo)
			.value("Negation", Token::Type::Negation)
			.value("RightShift", Token::Type::RightShift)
			.value("LeftShift", Token::Type::LeftShift)
			.value("Assignment", Token::Type::Assignment)
			.value("DollarSign", Token::Type::DollarSign)
			.value("BraceOpen", Token::Type::BraceOpen)
			.value("BraceClose", Token::Type::BraceClose)
			.value("ParenthesisOpen", Token::Type::ParenthesisOpen)
			.value("ParenthesisClose", Token::Type::ParenthesisClose)
			.value("BracketOpen", Token::Type::BracketOpen)
			.value("BracketClose", Token::Type::BracketClose)
			.value("Comma", Token::Type::Comma)
			.value("Semicolon", Token::Type::Semicolon)
			.value("Identifier", Token::Type::Identifier)
			.value("Number", Token::Type::Number)
			.value("FloatingPointNumber", Token::Type::FloatingPointNumber)
			.value("String", Token::Type::String)
			.value("Unknown", Token::Type::Unknown);

		py::class_<Token>(m, "Token")
			.def_readwrite("token", &Token::type)
			.def_readwrite("lexema", &Token::lexema)
			.def_readwrite("line", &Token::line)
			.def("get_type_string", &Token::getTypeString);

		m.def("get_tokens", &getTokens, "Obtener Tokens a partir de una string");

		py::class_<ErrorCode>(m, "ErrorCode")
			.def_readonly("line", &ErrorCode::line)
			.def_readonly("error_str", &ErrorCode::errorStr)
			.def("what", &ErrorCode::what);

		m.def("parse_tokens", &parseTokens, "Analizar sintacticamente una lista de tokens");
	}

#else

	int main()
	{
		std::ifstream file("../code.c", std::ios::binary | std::ios::in);
		std::string code{(std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()};

		// Obtener los tokens usando tu función getTokens
		std::vector<Token> tokens = getTokens(code);
		
		try
		{
			Parser parser(tokens);
			std::shared_ptr<NodeProgram> program = parser.parseProgram();
			std::cout << "El programa no contiene errores sintacticos !!\n";

			semanticAnalysis(program);
		}
		catch (const ErrorCode& e)
		{
			std::cerr << e.what() << std::endl;
		}

		return 0;
	}

#endif
