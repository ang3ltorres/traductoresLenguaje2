#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>

#include "sintactico.hpp"

#ifndef PYTHON_LIB

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

#else

#include <string>
#include <fstream>
#include <streambuf>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

static void compile(const std::string& fileName)
{
	Sintactico sintactico(fileName.c_str(), 1);
}

PYBIND11_MODULE(generador, m)
{
	m.def("compile_code", &compile, "Obtener codigo assembly a partir de un codigo fuente");
}

#endif
