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
		std::cout << "El programa no contiene errores!! :D\n";
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return 0;
}