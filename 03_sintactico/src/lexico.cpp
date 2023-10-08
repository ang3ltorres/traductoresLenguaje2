#include "lexico.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <format>
#include <regex>
#include <cmath>

using StringNumber = std::pair<std::string, unsigned int>;

const std::vector<std::pair<std::string, std::string>> tokenTypes =
{
	{"true", "Palabra reservada true"},
	{"false", "Palabra reservada false"},
	{"return", "Palabra reservada return"},
	{"void", "Palabra reservada void"},
	{"char", "Palabra reservada char"},
	{"short", "Palabra reservada short"},
	{"int", "Palabra reservada int"},
	{"long", "Palabra reservada long"},
	{"float", "Palabra reservada float"},
	{"do", "Palabra reservada do"},
	{"while", "Palabra reservada while"},
	{"for", "Palabra reservada for"},
	{"break", "Palabra reservada break"},
	{"switch", "Palabra reservada switch"},
	{"if", "Palabra reservada if"},

	{"+", "Operacion suma"},
	{"-", "Operacion resta"},
	{"++", "Operacion incremento"},
	{"--", "Operacion decremento"},
	{"+=", "Operacion mas igual"},
	{"-=", "Operacion menos igual"},
	{"*", "Operacion multiplicacion"},
	{"/", "Operacion division"},
	{"*=", "Operacion multiplicacion igual"},
	{"/=", "Operacion division igual"},
	{"%", "Operacion modulo"},
	{"%=", "Operacion modulo igual"},

	{"&", "AND a nivel de bits"},
	{"&&", "AND logico"},
	{"|", "OR a nivel de bits"},
	{"||", "OR logico"},
	{">", "Mayor que"},
	{"<", "Menor que"},
	{">=", "Mayor o igual que"},
	{"<=", "Menor o igual que"},
	{"==", "Igual a"},
	{"!=", "Diferente de"},
	{"!", "Negacion"},
	{">>", "Corrimiento hacia la derecha"},
	{"<<", "Corrimiento hacia la izquierda"},
	{"=", "Asignacion"},
	{"$", "Simbolo dolar"},

	{"{", "Llave abre"},
	{"}", "Llave cierra"},
	{"(", "Parentesis abre"},
	{")", "Parentesis cierra"},
	{"[", "Corchete abre"},
	{"]", "Corchete cierra"},
	{",", "Coma"},
	{";", "Punto y coma"},

	{"", "Identificador"},
	{"", "Numero"},
	{"", "Numero flotante"},
	{"", "Cadena"},
};

static bool is_operator(char c)
{
	static const std::string operatorsSymbols = "+-*/%!=&|<>";

	for (const char& i : operatorsSymbols)
		if (c == i) return true;
	return false;
}

static bool is_separator(char c)
{
	static const std::string separatorsSymbols = "{}()[],;";

	for (const auto& i : separatorsSymbols)
		if (i == c) return true;
	return false;
}

static int valid_number(const std::string& string)
{
	std::stringstream stream(string);
	float number;
	stream >> number;

	if (stream.eof() && !stream.fail())
	{
		if (std::floor(number) == number)
			return 1;
		else
			return 2;
	}

	return 0;
}

static bool valid_id(const std::string& nombre)
{
	static const std::regex pattern("[a-zA-Z_][a-zA-Z0-9_]*");
	return std::regex_match(nombre, pattern);
}

static std::vector<StringNumber> splitCode(const std::string& code)
{
	std::vector<StringNumber> v, v_aux;
	std::string string_aux;

	/*** Enumerar lineas ***/
	std::stringstream ss(code);
	for (unsigned int line = 1; std::getline(ss, string_aux); line++)
		v_aux.push_back({string_aux, line});
	
	v.clear();
	v.swap(v_aux);

	/*** Separar strings del resto ***/
	for (unsigned int line = 0; line < v.size();)
	{
		string_aux.clear();

		for (unsigned int index = 0; index < v[line].first.size(); index++)
		{
			char c = v[line].first[index];

			if (c != '"')
				string_aux += c;
			else
			{
				// Agregar aux
				if (!string_aux.empty())
					v_aux.push_back({string_aux, v[line].second});

				// Concatenar hasta encontrar el otro "
				string_aux.clear();

				for (index = index + 1; line < v.size();)
				{
					if (index < v[line].first.size())
					{
						c = v[line].first[index];
						if (c != '"')
						{
							string_aux += c;
							index++;
						}
						else
						{
							v_aux.push_back({'\"' + string_aux + '\"', v[line].second});
							string_aux.clear();
							break;
						}
					}
					else
					{
						string_aux += '\n';
						line++;
						index = 0;
					}
				}
			}
		}

		if (!string_aux.empty())
		{
			v_aux.push_back({string_aux, v[line].second});
			line++;
		}
	}

	v.clear();
	v.swap(v_aux);

	/*** Separar espacios en blanco ***/
	for (const auto& i : v)
	{
		// Ignorar strings
		if (i.first.front() == '"')
		{
			v_aux.push_back(i);
			continue;
		}

		std::istringstream iss(i.first);
		std::string token;
		
		while (iss >> string_aux)
			v_aux.push_back({string_aux, i.second});
	}

	v.clear();
	v.swap(v_aux);

	/*** Dividir separadores ***/
	for (const auto& i : v)
	{
		// Ignorar strings
		if (i.first.front() == '"')
		{
			v_aux.push_back(i);
			continue;
		}

		string_aux.clear();
		for (char c : i.first)
		{
			if (is_separator(c))
			{
				if (!string_aux.empty())
				{
					v_aux.push_back({string_aux, i.second});
					string_aux.clear();
				}
				v_aux.push_back({std::string(1, c), i.second});
			}
			else
				string_aux += c;
		}
		if (!string_aux.empty())
			v_aux.push_back({string_aux, i.second});
	}

	v.clear();
	v.swap(v_aux);

	/*** Separar operadores ***/
	for (const auto& i : v)
	{
		// Ignorar strings
		if (i.first.front() == '"')
		{
			v_aux.push_back(i);
			continue;
		}

		string_aux.clear();
		for (auto it = i.first.begin(); it != i.first.end(); it++)
		{
			if (is_operator(*it))
			{
				if (!string_aux.empty())
				{
					v_aux.push_back({string_aux, i.second});
					string_aux.clear();
				}
				string_aux += *it;

				// Concatenar a string_aux todos los caracteres operadores
				for (it = it + 1; it != i.first.end() && is_operator(*it); it++)
					string_aux += *it;
				
				// Regresar al caracter que no es operador
				it--;

				v_aux.push_back({string_aux, i.second});
				string_aux.clear();
			}
			else
				string_aux += *it;

		}
		if (!string_aux.empty())
			v_aux.push_back({string_aux, i.second});
	}

	v.clear();
	v.swap(v_aux);

	return v;
}

std::vector<Token> getTokens(const std::string& code)
{
	auto words = splitCode(code);

	std::vector<Token> tokens;
	for (const auto& w : words)
	{
		// Buscar
		bool found = false;
		int index = 0;
		for (const auto& i : tokenTypes)
		{
			if (i.first == w.first)
				{found = true; break;}
			index++;
		}

		int tipo_numero;

		if (found)
			tokens.push_back(Token{static_cast<Token::Type>(index), w.first, w.second});
		else if (w.first.front() == '"')
			tokens.push_back(Token{Token::Type::String, w.first, w.second});
		else if (valid_id(w.first))
			tokens.push_back(Token{Token::Type::Identifier, w.first, w.second});
		else if ((tipo_numero = valid_number(w.first)) != 0)
			tokens.push_back(Token{(tipo_numero == 1) ? Token::Type::Number : Token::Type::FloatingPointNumber, w.first, w.second});
		else
			tokens.push_back(Token{Token::Type::Unknown, w.first, w.second});
	}

	return tokens;
}

std::ostream& operator<<(std::ostream& os, const Token& token)
{
	// os << "TOKEN: " << tokenTypes[static_cast<int>(token.type)].second << "\t\tLEXEMA: " << token.lexema << "\t\tLINE: " << token.line;
	// return os;

	os << std::format("Token: {:<32s}Lexema: {:<24s}Linea: {:d}", tokenTypes[static_cast<int>(token.type)].second, token.lexema, token.line);
	return os;
}
