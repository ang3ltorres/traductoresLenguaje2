#pragma once
#include <string>
#include <vector>
#include <ostream>

extern const std::vector<std::pair<std::string, std::string>> token_types;

class Token
{
public:
	enum class Type: int
	{
		PalabraReservadaTrue = 0,
		PalabraReservadaFalse,
		PalabraReservadaReturn,
		PalabraReservadaVoid,
		PalabraReservadaChar,
		PalabraReservadaShort,
		PalabraReservadaInt,
		PalabraReservadaLong,
		PalabraReservadaFloat,
		PalabraReservadaDo,
		PalabraReservadaWhile,
		PalabraReservadaFor,
		PalabraReservadaBreak,
		PalabraReservadaSwitch,
		PalabraReservadaIf,
		OperacionSuma,
		OperacionResta,
		OperacionIncremento,
		OperacionDecremento,
		OperacionMasIgual,
		OperacionMenosIgual,
		OperacionMultiplicacion,
		OperacionDivision,
		OperacionMultiplicacionIgual,
		OperacionDivisionIgual,
		OperacionModulo,
		OperacionModuloIgual,
		ANDBits,
		ANDLogico,
		ORBits,
		ORLogico,
		MayorQue,
		MenorQue,
		MayorIgualQue,
		MenorIgualQue,
		IgualA,
		DiferenteDe,
		Negacion,
		CorrimientoHaciaLaDerecha,
		CorrimientoHaciaLaIzquierda,
		Asignacion,
		Dolar,
		LlaveAbre,
		LlaveCierra,
		ParentesisAbre,
		ParentesisCierra,
		CorcheteAbre,
		CorcheteCierra,
		Coma,
		PuntoComa,
		Identificador,
		Numero,
		NumeroFlotante,
		Cadena,
		Desconocido,
	};

	Type type;
	std::string lexema;
	unsigned int line;

	friend std::ostream& operator<<(std::ostream& os, const Token& token);
};

std::vector<Token> getTokens(const std::string& code);

