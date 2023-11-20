#include "genera.hpp"

GeneraCodigo::GeneraCodigo(const char *unNombreFichero)
{
	if ((salida = fopen(unNombreFichero, "w")) == NULL)
	{
		std::cout << "No se puede crear el fichero" << unNombreFichero << '\n';
		exit(EXIT_FAILURE);
	}
}

GeneraCodigo::~GeneraCodigo(void)
{
	fclose(salida);
}

void GeneraCodigo::code()
{
	std::cout << ".CODE\n";
	fputs(".CODE\n", salida);
}

void GeneraCodigo::pushc(char constante)
{
	std::cout << "PUSHC " << constante << '\n';
	fputs("PUSHC ", salida);
	fputc(constante, salida);
	fputc('\n', salida);
}

void GeneraCodigo::pusha(char direccion)
{
	std::cout << "PUSHA " << direccion << '\n';
	fputs("PUSHA ", salida);
	fputc(direccion, salida);
	fputc('\n', salida);
}

void GeneraCodigo::load()
{
	std::cout << "LOAD\n";
	fputs("LOAD\n", salida);
}

void GeneraCodigo::store()
{
	std::cout << "STORE\n";
	fputs("STORE\n", salida);
}

void GeneraCodigo::neg()
{
	std::cout << "NEG\n";
	fputs("NEG\n", salida);
}

void GeneraCodigo::add()
{
	std::cout << "ADD\n";
	fputs("ADD\n", salida);
}

void GeneraCodigo::mul()
{
	std::cout << "MUL\n";
	fputs("MUL\n", salida);
}
void GeneraCodigo::div()
{
	std::cout << "DIV\n";
	fputs("DIV\n", salida);
}
void GeneraCodigo::mod()
{
	std::cout << "MOD\n";
	fputs("MOD\n", salida);
}

void GeneraCodigo::input(char direccion)
{
	std::cout << "INPUT " << direccion << '\n';
	fputs("INPUT ", salida);
	fputc(direccion, salida);
	fputc('\n', salida);
}

void GeneraCodigo::output(char direccion)
{
	std::cout << "OUTPUT " << direccion << '\n';
	fputs("OUTPUT ", salida);
	fputc(direccion, salida);
	fputc('\n', salida);
}

void GeneraCodigo::end()
{
	std::cout << "END\n";
	fputs("END\n", salida);
}
