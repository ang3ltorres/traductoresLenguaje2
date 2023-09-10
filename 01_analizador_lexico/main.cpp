#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <format>
#include <vector>
#include <array>
#include <ranges>
#include <algorithm>

enum TokenType
{
	TipoDeDato = 0,
	Identificador,
	Constante,
	PuntoYComa,
	Coma,
	ParentesisAbre,
	ParentesisCierra,
	CorcheteAbre,
	CorcheteCierra,
	Asignacion,
	If,
	While,
	Return,
	Else,
	For,
	OpAdicion,
	OpMultiplicacion,
	OpLogico,
	OpRelacional,
	SignoDolar,
	Cadena,
	InvalidToken
};

struct Token
{
	static const std::vector<std::string> tokenTypeToString;
	TokenType tokenType;
	std::string lexema;

	friend std::ostream& operator<<(std::ostream& os, const Token& t)
	{
		os << std::format("{:<{}s}- {:s}", t.tokenTypeToString[t.tokenType], 17, t.lexema);
		return os;

	}
};

const std::vector<std::string> Token::tokenTypeToString = {
	"TipoDeDato",
	"Identificador",
	"Constante",
	"PuntoYComa",
	"Coma",
	"ParentesisAbre",
	"ParentesisCierra",
	"CorcheteAbre",
	"CorcheteCierra",
	"Asignacion",
	"If",
	"While",
	"Return",
	"Else",
	"For",
	"OpAdicion",
	"OpMultiplicacion",
	"OpLogico",
	"OpRelacional",
	"SignoDolar",
	"Cadena",
	"InvalidToken"
};

std::vector<std::string> getWordsFromFile(const std::string& fileName)
{
	const auto readFile = [](const std::string& fileName) -> std::string
	{
		std::ifstream file(fileName, std::ios::binary | std::ios::in);
		std::stringstream buffer;
		buffer << file.rdbuf();
		file.close();
		return buffer.str();
	};

	unsigned int index = 0;
	std::string buffer(readFile(fileName));

	const auto getNextWord = [&]() -> std::string
	{
		// Skip whitespaces
		while (index < buffer.size() && std::isspace(buffer[index]))
			index++;

		std::string word;

		// It's a const string
		if (index < buffer.size() && buffer[index] == '"')
		{
			word = "\"";
			index++;

			while (index < buffer.size() && buffer[index] != '"')
				word += buffer[index++];

			word += '"';
			index++;
		}
		else
		{
			while (index < buffer.size() && !std::isspace(buffer[index]) && buffer[index] != '"')
				word += buffer[index++];
		}
			
		return word;
	};

	std::vector<std::string> words;
	std::string word;

	while (!(word = getNextWord()).empty())
		words.push_back(word);

	return words;	
};

void splitSeparatorCharacters(std::vector<std::string>& words)
{
	static const std::string separators = "(){}[];,";
	const auto isSeparator = [](char c) -> bool {
		return std::any_of(separators.begin(), separators.end(), [&c](const char sep) {return sep == c;});
	};

	std::vector<std::string> splitted;

	for (const std::string& word : words)
	{
		// Skip strings
		if (word.front() == '"')
		{
			splitted.push_back(word);
			continue;
		}

		std::string aux;
		for (char letter : word)
		{
			if (isSeparator(letter))
			{
				if (!aux.empty())
				{
					splitted.push_back(aux);
					aux.clear();
				}
				splitted.push_back(std::string(1, letter));
			}
			else
				aux += letter;
		}
		if (!aux.empty())
			splitted.push_back(aux);
	}

	words = splitted;
};

void splitOperatorCharacters(std::vector<std::string>& words)
{
	const auto isOpCharacter = [](char c) -> char
	{
		static const std::string opCharacter = "+-<>=/*&|!";
		for (const auto& oc : opCharacter) { if (c == oc ) return oc; }
		return ' ';
	};

	std::vector<std::string> splitted;

	for (const std::string& word : words)
	{
		// Skip strings
		if (word.front() == '"')
		{
			splitted.push_back(word);
			continue;
		}

		std::string aux = "";
		std::string opAux = "";

		for (auto i = word.begin(); i != word.end(); i++)
		{
			if (isOpCharacter(*i) != ' ')
			{
				if (!aux.empty())
				{
					splitted.push_back(aux);
					aux = "";
				}
				opAux += *i;

				// Count op characters
				for (i = i + 1; i != word.end() && isOpCharacter(*i) != ' '; i++)
					opAux += *i;
				
				// Go back to the non OP character
				i--;

				splitted.push_back(opAux);
				opAux = "";
			}
			else
				aux += *i;

		}
		if (!aux.empty())
			splitted.push_back(aux);
	}

	words = splitted;
};

std::vector<Token> tokensFromFile(const std::string& fileName)
{
	const auto isNumber = [](const std::string& str) -> bool
	{
		try
		{
			size_t pos;
			std::stoi(str, &pos);
			return pos == str.length();
		}
		catch (const std::invalid_argument& e)
		{
			return false;
		}
		catch (const std::out_of_range& e)
		{
			return false;
		}
	};

	const auto isOp = [](const std::string& str) -> bool
	{
		static const std::string opCharacter = "+-<>=/*&|!";
		char c = str.front();
		return std::any_of(opCharacter.begin(), opCharacter.end(), [c](const char i){return i == c;});
	};

	const auto getOpToken = [](const std::string& str) -> Token
	{
		static const std::vector<Token> opTokens =
		{
			{TokenType::OpAdicion, "+"},
			{TokenType::OpAdicion, "+="},
			{TokenType::OpAdicion, "-"},
			{TokenType::OpAdicion, "-="},
			{TokenType::OpAdicion, "++"},
			{TokenType::OpAdicion, "--"},

			{TokenType::OpMultiplicacion, "*"},
			{TokenType::OpMultiplicacion, "/"},
			{TokenType::OpMultiplicacion, ">>"},
			{TokenType::OpMultiplicacion, "<<"},

			{TokenType::OpLogico, "&&"},
			{TokenType::OpLogico, "||"},

			{TokenType::OpRelacional, "<"},
			{TokenType::OpRelacional, ">"},
			{TokenType::OpRelacional, ">="},
			{TokenType::OpRelacional, "<="},
			{TokenType::OpRelacional, "=="},
			{TokenType::OpRelacional, "!="},
		};

		for (const auto& t : opTokens)
		{
			if (str == t.lexema)
				return t;
		}
		return {TokenType::InvalidToken, str};
	};

	const auto isValidId = [](const std::string& str) -> bool
	{
		return std::all_of(str.begin(), str.end(), [](char c)
		{
			return (std::isdigit(c) or std::isalpha(c) or c == '_');
		});
	};

	std::vector<std::string> words = getWordsFromFile(fileName);
	splitSeparatorCharacters(words);
	splitOperatorCharacters(words);

	std::vector<Token> tokens;
	for (const auto& t : words)
	{
		// Clasifica tokens
		if (t == "int")
			tokens.push_back({TokenType::TipoDeDato, t});

		else if (t == "float")
			tokens.push_back({TokenType::TipoDeDato, t});

		else if (t == "char")
			tokens.push_back({TokenType::TipoDeDato, t});

		else if (t == "void")
			tokens.push_back({TokenType::TipoDeDato, t});

		else if (t == "string")
			tokens.push_back({TokenType::TipoDeDato, t});

		else if (t == "(")
			tokens.push_back({TokenType::ParentesisAbre, t});

		else if (t == ")")
			tokens.push_back({TokenType::ParentesisCierra, t});

		else if (t == "{" || t == "[")
			tokens.push_back({TokenType::CorcheteAbre, t});

		else if (t == "}" || t == "]")
			tokens.push_back({TokenType::CorcheteCierra, t});

		else if (t == ";")
			tokens.push_back({TokenType::PuntoYComa, t});

		else if (t == ",")
			tokens.push_back({TokenType::Coma, t});

		else if (t == "=")
			tokens.push_back({TokenType::Asignacion, t});

		else if (t == "if")
			tokens.push_back({TokenType::If, t});

		else if (t == "while")
			tokens.push_back({TokenType::While, t});

		else if (t == "return")
			tokens.push_back({TokenType::Return, t});

		else if (t == "else")
			tokens.push_back({TokenType::Else, t});

		else if (t == "for")
			tokens.push_back({TokenType::For, t});

		else if (t == "$")
			tokens.push_back({TokenType::SignoDolar, t});

		else if (t.front() == '"')
			tokens.push_back({TokenType::Cadena, t});
			
		else if (isNumber(t) || t == "pi" || t == "true" || t == "false")
			tokens.push_back({TokenType::Constante, t});

		else if (isOp(t))
			tokens.push_back(getOpToken(t));

		else if (isValidId(t))
			tokens.push_back({TokenType::Identificador, t});

		else
			tokens.push_back({TokenType::InvalidToken, t});
	}

	return tokens;
}

int main()
{
	auto tokens = tokensFromFile("input.txt");

	for (const auto& t : tokens)
		std::cout << t << "\n";

	// Contar tokens de cada categoria
	std::array<unsigned int, 22> count = {0};
	for (const auto& t : tokens)
		count[t.tokenType]++;

	std::cout << "\nCategorias encontradas:\n";
	for (auto i : std::views::iota(0, 22))
		std::cout << std::format("{:<{}s}: {:d}\n", Token::tokenTypeToString[i], 18, count[i]);
}
