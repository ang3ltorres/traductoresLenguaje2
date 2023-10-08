#include "lexico.hpp"
#include "sintactico.hpp"
#include <iostream>
#include <sstream>

int main()
{
	// Obtener los tokens usando tu función getTokens
	std::vector<Token> tokens = getTokens("int main(){return 0; int b = 3; } void xd(int a, int b){return 32;}");
	
	try
	{
		Parser parser(tokens);
		std::shared_ptr<NodeProgram> program = parser.parseProgram();
	}
	catch (const char* e)
	{
		std::cerr << "Error: " << e << std::endl;
	}

	return 0;
}