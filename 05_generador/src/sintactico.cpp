#include "sintactico.hpp"

#include <iostream>

Sintactico::Sintactico(char *fuente, int traza)
 : lexico(fuente, traza)
//se inicializa el constructor de la clase léxico
{
	if (lexico.existeTraza())
		std::cout<<"INICIO DE ANALISIS SINTACTICO\n";

	programa();
}

Sintactico::~Sintactico(void)
{
	if (lexico.existeTraza())
	{
		std::cout<<"FIN DE ANALISIS SINTACTICO\n";
		std::cout<<"FIN DE COMPILACION\n";
	}
}

void Sintactico::programa(void)
{
	char token;

	if (lexico.existeTraza())
		std::cout << "ANALISIS SINTACTICO: <PROGRAMA>\n";

	token = lexico.siguienteToken();
	if (token=='M')
		std::cout << "M";
	else
		errores(8);

	token = lexico.siguienteToken();

	if (token != '{')
		errores(9);

	bloque();
	token = lexico.siguienteToken();

	if (token == '}')
		std::cout << "}";
	else
		errores(2);
}


void Sintactico::bloque(void)
{
	if (lexico.existeTraza())
		std::cout << "ANALISIS SINTACTICO: <BLOQUE>\n";

	sentencia();
	otra_sentencia();
}

void Sintactico::otra_sentencia(void)
{
	char token;

	if (lexico.existeTraza())
		std::cout << "ANALISIS SINTACTICO: <OTRA_SENTENCIA>\n";

	token = lexico.siguienteToken();

	if (token == ';')
	{
		sentencia();
		otra_sentencia();
	}
	else
		lexico.devuelveToken(token); //vacio
}


void Sintactico::sentencia(void)
{
	char token;

	if (lexico.existeTraza())
		std::cout << "ANALISIS SINTACTICO: <SENTENCIA>\n";

	token = lexico.siguienteToken();

	if ((token >= 'a') && (token <= 'z'))
	{
		lexico.devuelveToken(token);
		asignacion();
	}
	else if (token == 'R')
		lectura();
	else if (token == 'W')
		escritura();
	else
		errores(6);
}


void Sintactico::asignacion()
{
	char token;

	if (lexico.existeTraza())
		std::cout << "ANALISIS SINTACTICO: <ASIGNACION>\n";

	variable();

	token = lexico.siguienteToken();

	if (token != '=')
		errores(3);

	expresion();
}


void Sintactico::variable(void)
{
	char token;

	if (lexico.existeTraza())
		std::cout<<"ANALISIS SINTACTICO: <VARIABLE>\n";

	token = lexico.siguienteToken();

	if ((token >= 'a') && (token <= 'z'))
		std::cout << token;
	else
		errores(5);
}


void Sintactico::expresion(void)
{
	if (lexico.existeTraza())
		std::cout<<"ANALISIS SINTACTICO: <EXPRESION>\n";

	termino();
	mas_terminos();
}


void Sintactico::termino(void)
{
	if (lexico.existeTraza())
		std::cout << "ANALISIS SINTACTICO: <TERMINO>\n";

	factor();
	mas_factores();
}

void Sintactico::mas_terminos(void)
{
	char token;

	if (lexico.existeTraza())
		std::cout<<"ANALISIS SINTACTICO: <MAS_TERMINOS>\n";

	token = lexico.siguienteToken();

	if (token=='+')
	{
		termino();
		mas_terminos();
	}
	else if (token =='-')
	{
		termino();
		mas_terminos();
	}
	else
		lexico.devuelveToken(token); // <vacio>
}

void Sintactico::factor(void)
{
	char token;

	if (lexico.existeTraza())
		std::cout << "ANALISIS SINTACTICO: <FACTOR>\n";

	token = lexico.siguienteToken();

	if ((token >= '0') && (token <= '9'))
	{
		lexico.devuelveToken(token);
		constante();
	}
	else if (token == '(')
	{
		expresion();
		token = lexico.siguienteToken();

		if (token != ')')
			errores(4);
	}
	else
	{
		lexico.devuelveToken(token);
		variable();
	}
}
void
Sintactico::mas_factores(void)
{
	char token;

	if (lexico.existeTraza())
		std::cout<<"ANALISIS SINTACTICO: <MAS_FACTORES>\n";

	token = lexico.siguienteToken();

	switch (token)
	{
		case '*':
			factor();
			mas_factores();
			break;

		case '/':
			factor();
			mas_factores();
			break;

		case '%':
			factor();
			mas_factores();
			break;

		default: //<vacio>
			lexico.devuelveToken(token);
	}
}

void Sintactico::lectura(void)
{
	char token;

	token = lexico.siguienteToken();

	if (lexico.existeTraza())
		std::cout<<"ANALISIS SINTACTICO: <LECTURA> " << token << '\n';

	if ((token<'a') || (token>'z'))
		errores(5);
}

void Sintactico::escritura(void)
{
	char token;

	token = lexico.siguienteToken();

	if (lexico.existeTraza())
		std::cout<<"ANALISIS SINTACTICO: <ESCRITURA> " << token << '\n';

	if ((token<'a') || (token>'z'))
		errores(5);
}

void Sintactico::constante(void)
{
	char token;

	if (lexico.existeTraza())
		std::cout<<"ANALISIS SINTACTICO: <CONSTANTE>\n";

	token = lexico.siguienteToken();

	if ((token>='0') && (token<='9'))
		std::cout<<token;
	else
		errores(7);
}

void Sintactico::errores(int codigo)
{
	std::cout << "LINEA " << lexico.lineaActual();
	std::cout << " ERROR SINTACTICO " << codigo;

	switch (codigo)
	{
		case 1 : std::cout << " :ESPERABA UN ;\n"; break;
		case 2 : std::cout << " :ESPERABA UNA }\n"; break;
		case 3 : std::cout << " :ESPERABA UN =\n"; break;
		case 4 : std::cout << " :ESPERABA UN )\n"; break;
		case 5 : std::cout << " :ESPERABA UN IDENTIFICADOR\n"; break;
		case 6 : std::cout << " :INSTRUCCION DESCONOCIDA\n"; break;
		case 7 : std::cout << " :ESPERABA UNA CONSTANTE\n"; break;
		case 8 : std::cout << " :ESPERABA UNA M DE MAIN\n"; break;
		case 9 : std::cout << " :ESPERABA UNA {\n"; break;
		default: std::cout << " :NO DOCUMENTADO\n";
	}

	exit(EXIT_FAILURE);
}
