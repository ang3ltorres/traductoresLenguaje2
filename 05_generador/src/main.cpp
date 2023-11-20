#include <iostream>
#include <stdlib.h>
#include <string>

#include "list.hpp"

int main()
{
	std::string expresion;

	int opc = 0;

	List<char> pila;

	do
	{
		std::cout << "\n[1] Salir del programa\n";
		std::cout<<"[2] Escribir una expresion a convertir\n";
		std::cout << "\n\nQue opcion elijes: ";
		
		std::cin >> opc;
		std::cout << '\n';

		switch(opc)
		{
			case 1:
				std::cout<<"Gracias por usar el conversor\n";
				break;

			case 2:
				std::cout << "Escribe la expresion algebraica a convertir: ";
				std::cin >> expresion;

				for (int i = 0; i < int(expresion.size()); i++)
					pila.reconocimiento(expresion[i]);

				pila.expresionFinal();
				break;

			default:
				std::cout<<"Opcion no valida, intenta de nuevo\n";
				break;
		}
	}
	while(opc != 1);


	return 0;
}