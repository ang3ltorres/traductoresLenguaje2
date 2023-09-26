#include "lexico.hpp"
#include <iostream>

int main()
{
	auto tokens = getTokens("int main() {float num = 3.2; return 0;}");
	for (const auto& t : tokens)
			std::cout << t << '\n';

	return 0;
}