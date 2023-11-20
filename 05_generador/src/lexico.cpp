#include "lexico.hpp"

#include <iostream>
#include <cctype>

Lexico::Lexico(const char* nombreFichero, int traza)
{
	entrada = fopen(nombreFichero, "rt");
	if (entrada == NULL)
	{
		std::cout << "No se puede abrir el archivo\n";
		exit(EXIT_FAILURE);
	}

	this->traza = traza;

	n1 = 1;
	pBuffer = 0;
}

Lexico::~Lexico()
{
	fclose(entrada);
}

char Lexico::siguienteToken()
{
	char car;
	while ((car = ((pBuffer > 0) ? buffer[--pBuffer] : getc(entrada))) != EOF)
	{
		if (car == ' ')
			continue;
		
		if (car == '\n')
		{
			++n1;
			continue;
		}

		break;
	}

	if (traza)
		std::cout << "Analizador lexico: Lee el token " << car << "\n";

	switch (car)
	{
		case 'M':
		case 'R':
		case 'W':
		case '=':
		case '(':
		case ')':
		case ';':
		case '}':
		case '{':
		case '.':
		case '+':
		case '*':
		case '-':
		case '/':
		case '%':
		return car;
	}

	if (islower(car))
		return car;
	else if (isdigit(car))
		return car;
	else
	{
		std::cout << "Error lexico: Token desconocido\n";
		exit(EXIT_FAILURE);
	}

	return car;
}

void Lexico::devuelveToken(char token)
{
	if (pBuffer > TAM_BUFFER)
	{
		std::cout << "Error: Desbordamiento del buffer del analizador lexico\n";
		exit(EXIT_FAILURE);
	}
	else
	{
		buffer[pBuffer++] = token;

		if (existeTraza())
			std::cout << "Analizador lexico: Recibe en buffer el token " << token << '\n';			
	}
}

int Lexico::lineaActual()
{
	return n1;
}

int Lexico::existeTraza()
{
	return traza;
}

