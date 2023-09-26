#include "lexico.hpp"
#include <fstream>
#include <sstream>
#include <regex>
#include <cmath>

const std::vector<std::pair<std::string, std::string>> token_types =
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

const std::string operators_symbols = "+-*/%!=&|<>";
const std::string separators_symbols = "{}()[],;";

static bool is_operator(char c)
{
	for (const char& i : operators_symbols)
		if (c == i) return true;
	return false;
}

static bool is_separator(char c)
{
	for (const auto& i : separators_symbols)
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

static std::vector<std::string> split_strings(const std::string& code)
{
	std::vector<std::string> v;
	std::istringstream stream(code);
	std::string aux;
	char c;

	while (stream.get(c))
	{
		if (c == '"')
		{
			if (!aux.empty())
			{
				v.push_back(aux);
				aux.clear();
			}

			while (stream.get(c))
			{
				if (c == '"')
					break;
				aux += c;
			}
			v.push_back('"' + aux + '"');
			aux.clear();
		}
		else
			aux += c;
	}

	if (!aux.empty())
		v.push_back(aux);

	return v;
}

static void split_white_spaces(std::vector<std::string>& words)
{
	std::vector<std::string> v;

	for (const std::string& word : words)
	{
		// Ignorar strings
		if (word.front() == '"')
		{
			v.push_back(word);
			continue;
		}

		std::istringstream iss(word);
		std::string token;
		
		while (iss >> token)
			v.push_back(token);
	}

	words = v;
}

static void split_separators(std::vector<std::string>& words)
{
	std::vector<std::string> v;

	for (const std::string& word : words)
	{
		// Ignorar strings
		if (word.front() == '"')
		{
			v.push_back(word);
			continue;
		}

		std::string aux;
		for (char c : word)
		{
			if (is_separator(c))
			{
				if (!aux.empty())
				{
					v.push_back(aux);
					aux.clear();
				}
				v.push_back(std::string(1, c));
			}
			else
				aux += c;
		}
		if (!aux.empty())
			v.push_back(aux);
	}

	words = v;
}

static void split_operators(std::vector<std::string>& words)
{
	std::vector<std::string> v;

	for (const std::string& word : words)
	{
		// Ignorar strings
		if (word.front() == '"')
		{
			v.push_back(word);
			continue;
		}

		std::string aux;

		for (auto it = word.begin(); it != word.end(); it++)
		{
			if (is_operator(*it))
			{
				if (!aux.empty())
				{
					v.push_back(aux);
					aux.clear();
				}
				aux += *it;

				// Concatenar a aux todos los caracteres operadores
				for (it = it + 1; it != word.end() && is_operator(*it); it++)
					aux += *it;
				
				// Regresar al caracter que no es operador
				it--;

				v.push_back(aux);
				aux.clear();
			}
			else
				aux += *it;

		}
		if (!aux.empty())
			v.push_back(aux);
	}

	words = v;
};

static std::vector<std::string> split(const std::string& code)
{
	std::vector<std::string> words = split_strings(code);
	split_white_spaces(words);
	split_separators(words);
	split_operators(words);
	return words;
}


std::ostream& operator<<(std::ostream& os, const Token& token)
{
	os << "TOKEN: " << token_types[static_cast<int>(token.type)].second << "\t\tLEXEMA: " << token.lexema;
	return os;
}

std::vector<Token> getTokens(const std::string& code)
{
	auto words = split(code);

	std::vector<Token> tokens;
	for (const auto& w : words)
	{

		// Buscar
		bool found = false;
		int index = 0;
		for (const auto& i : token_types)
		{
			if (i.first == w)
				{found = true; break;}
			index++;
		}

		int tipo_numero;

		if (found)
			tokens.push_back(Token{static_cast<Token::Type>(index), w});
		else if (w.front() == '"')
			tokens.push_back(Token{Token::Type::Cadena, w});
		else if (valid_id(w))
			tokens.push_back(Token{Token::Type::Identificador, w});
		else if ((tipo_numero = valid_number(w)) != 0)
			tokens.push_back(Token{(tipo_numero == 1) ? Token::Type::Numero : Token::Type::NumeroFlotante, w});
		else
			tokens.push_back(Token{Token::Type::Desconocido, w});
	}

	return tokens;
}
