#pragma once
#include <cstdio>
#define TAM_BUFFER 100

class Lexico
{
public:
	char* nombreFichero;
	FILE* entrada;
	int n1;
	int traza;
	char buffer[TAM_BUFFER];
	int pBuffer;

	Lexico(const char* nombreFichero, int traza = 0);
	~Lexico();
	char siguienteToken();
	void devuelveToken(char token);
	int lineaActual();
	int existeTraza();
};
