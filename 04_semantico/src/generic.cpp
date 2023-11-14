#include "generic.hpp"

#include "semantico.hpp"
#include "sintactico.hpp"

ErrorCode::ErrorCode(unsigned int line, std::string errorStr)
: line(line), errorStr(errorStr) {}

const char* ErrorCode::what() const noexcept
{
	static std::string aux;
	aux.clear();
	aux += std::format("Error: {:s}\n\tLinea: {:d}\n", errorStr, line);
	return aux.c_str();
}

#ifdef PYTHON_LIB

std::tuple
<
	std::vector<Token>,
	ErrorCode,
	std::vector<ErrorCode>
>
parse(const std::string& code)
{
	auto tokens = getTokens(code);

	try
	{
		Parser parser(tokens);
		std::shared_ptr<NodeProgram> program = parser.parseProgram();
		
		semanticAnalysis(program);
		
		return std::make_tuple(
			tokens,
			ErrorCode{0, "El programa no contiene errores sintacticos!! :D"},
			(semanticErrors.size()) ? semanticErrors : std::vector<ErrorCode>(1, ErrorCode{0, "El programa no contiene errores semanticos!! :D"})
		);

		semanticAnalysis(program);
	}
	catch (const ErrorCode& e)
	{
		return std::make_tuple(
			tokens,
			e,
			std::vector<ErrorCode>(1, ErrorCode{0, "NULL"})
		);
	}

}

#endif

