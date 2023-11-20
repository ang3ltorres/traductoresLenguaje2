#pragma once
#include "lexico.hpp"
#include "genera.hpp"

class Sintactico
{
	void programa (void);
	void bloque (void);
	void sentencia (void);
	void otra_sentencia (void);
	void asignacion(void);
	void lectura (void);
	void escritura(void);
	void variable(void);
	void expresion(void);
	void termino(void);
	void mas_terminos(void);
	void factor(void);
	void mas_factores(void);
	void constante(void);
	void errores (int codigo);

	Lexico lexico;
	GeneraCodigo generaCodigo;

public:
	Sintactico(const char *fuente, int traza);
	~Sintactico(void);
};