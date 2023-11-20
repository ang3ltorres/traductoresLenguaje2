#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>

#include "sintactico.hpp"

int main()
{
	// std::cout << "Abriendo el archivo 'prueba.txt'...";

	// FILE *prueba = fopen("prueba.txt", "r");
	// if (!prueba)
	// {
	// 	std::cout << "Error al abrir el archivo!!\n";
	// 	exit(EXIT_FAILURE);
	// }

	Sintactico sintactico("prueba.txt", 1);


	return 0;
}