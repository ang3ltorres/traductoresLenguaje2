#include "lexico.hpp"
#include <iostream>
#include <sstream>

int main()
{
	auto v = getTokens("int main()\n{\n\tprintf(\"Hola mundo\ny mas texto\");\nint num = 32 && 89;\nnum+=99;\nreturn 0;}");
	for (const auto& t : v)
		std::cout << t << '\n';

	return 0;
}