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
	ErrorCode
>
parse(const std::string& code)
{
	auto tokens = getTokens(code);

	try
	{
		Parser parser(tokens);
		std::shared_ptr<NodeProgram> program = parser.parseProgram();
		
		try
		{
			semanticAnalysis(program);
			return std::make_tuple(
				tokens,
				ErrorCode{0, "El programa no contiene errores sintacticos!! :D"},
				ErrorCode{0, "El programa no contiene errores semanticos!! :D"}
			);
		}
		catch (const ErrorCode& e)
		{
			return std::make_tuple(
				tokens,
				ErrorCode{0, "El programa no contiene errores sintacticos!! :D"},
				e
			);
		}

		semanticAnalysis(program);
	}
	catch (const ErrorCode& e)
	{
		return std::make_tuple(
			tokens,
			e,
			ErrorCode{0, "NULL"}
		);
	}

}

#endif

