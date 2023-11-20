#pragma once

#include <cstdio>
#include <iostream>
#include <cstdlib>

class GeneraCodigo
{
	char *nombreFichero;
	FILE *salida;

public:
	GeneraCodigo(const char *unNombreFichero);
	~GeneraCodigo();

	void code();
	void pushc(char constante);
	void pusha(char direccion);
	void load();
	void store();
	void neg();
	void add();
	void mul();
	void div();
	void mod();
	void input(char direccion);
	void output(char direccion);
	void end();
};
