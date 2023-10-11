#include "lexico.hpp"
#include "sintactico.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

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
		std::cout << "El programa no contiene errores!! :D\n";
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}