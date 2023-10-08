#include "lexico.hpp"
#include "sintactico.hpp"
#include <iostream>
#include <sstream>

int main()
{
	// Obtener los tokens usando tu función getTokens
	std::vector<Token> tokens = getTokens("y= (32 + 99); k = 92;");
	
	try
	{
		Parser parser(tokens);
		std::shared_ptr<NodeProgram> program = parser.parseProgram();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}