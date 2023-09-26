#include "lexico.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(lexico, m)
{
	py::enum_<Token::Type>(m, "Token::Type")
		.value("PalabraReservadaTrue", Token::Type::PalabraReservadaTrue)
		.value("PalabraReservadaFalse", Token::Type::PalabraReservadaFalse)
		.value("PalabraReservadaReturn", Token::Type::PalabraReservadaReturn)
		.value("PalabraReservadaVoid", Token::Type::PalabraReservadaVoid)
		.value("PalabraReservadaChar", Token::Type::PalabraReservadaChar)
		.value("PalabraReservadaShort", Token::Type::PalabraReservadaShort)
		.value("PalabraReservadaInt", Token::Type::PalabraReservadaInt)
		.value("PalabraReservadaLong", Token::Type::PalabraReservadaLong)
		.value("PalabraReservadaFloat", Token::Type::PalabraReservadaFloat)
		.value("PalabraReservadaDo", Token::Type::PalabraReservadaDo)
		.value("PalabraReservadaWhile", Token::Type::PalabraReservadaWhile)
		.value("PalabraReservadaFor", Token::Type::PalabraReservadaFor)
		.value("PalabraReservadaBreak", Token::Type::PalabraReservadaBreak)
		.value("PalabraReservadaSwitch", Token::Type::PalabraReservadaSwitch)
		.value("PalabraReservadaIf", Token::Type::PalabraReservadaIf)
		.value("OperacionSuma", Token::Type::OperacionSuma)
		.value("OperacionResta", Token::Type::OperacionResta)
		.value("OperacionIncremento", Token::Type::OperacionIncremento)
		.value("OperacionDecremento", Token::Type::OperacionDecremento)
		.value("OperacionMasIgual", Token::Type::OperacionMasIgual)
		.value("OperacionMenosIgual", Token::Type::OperacionMenosIgual)
		.value("OperacionMultiplicacion", Token::Type::OperacionMultiplicacion)
		.value("OperacionDivision", Token::Type::OperacionDivision)
		.value("OperacionMultiplicacionIgual", Token::Type::OperacionMultiplicacionIgual)
		.value("OperacionDivisionIgual", Token::Type::OperacionDivisionIgual)
		.value("OperacionModulo", Token::Type::OperacionModulo)
		.value("OperacionModuloIgual", Token::Type::OperacionModuloIgual)
		.value("ANDBits", Token::Type::ANDBits)
		.value("ANDLogico", Token::Type::ANDLogico)
		.value("ORBits", Token::Type::ORBits)
		.value("ORLogico", Token::Type::ORLogico)
		.value("MayorQue", Token::Type::MayorQue)
		.value("MenorQue", Token::Type::MenorQue)
		.value("MayorIgualQue", Token::Type::MayorIgualQue)
		.value("MenorIgualQue", Token::Type::MenorIgualQue)
		.value("IgualA", Token::Type::IgualA)
		.value("DiferenteDe", Token::Type::DiferenteDe)
		.value("Negacion", Token::Type::Negacion)
		.value("CorrimientoHaciaLaDerecha", Token::Type::CorrimientoHaciaLaDerecha)
		.value("CorrimientoHaciaLaIzquierda", Token::Type::CorrimientoHaciaLaIzquierda)
		.value("Asignacion", Token::Type::Asignacion)
		.value("Dolar", Token::Type::Dolar)
		.value("LlaveAbre", Token::Type::LlaveAbre)
		.value("LlaveCierra", Token::Type::LlaveCierra)
		.value("ParentesisAbre", Token::Type::ParentesisAbre)
		.value("ParentesisCierra", Token::Type::ParentesisCierra)
		.value("CorcheteAbre", Token::Type::CorcheteAbre)
		.value("CorcheteCierra", Token::Type::CorcheteCierra)
		.value("Coma", Token::Type::Coma)
		.value("PuntoComa", Token::Type::PuntoComa)
		.value("Identificador", Token::Type::Identificador)
		.value("Numero", Token::Type::Numero)
		.value("NumeroFlotante", Token::Type::NumeroFlotante)
		.value("Cadena", Token::Type::Cadena)
		.value("Desconocido", Token::Type::Desconocido);

	m.def("get_token_types", []() {
		return token_types;
	}, "Obtiene la lista de token types como pares de strings.");
	
	py::class_<Token>(m, "Token")
		.def(py::init<Token::Type, const std::string&>())
		.def_readwrite("token", &Token::type)
		.def_readwrite("lexema", &Token::lexema);

	m.def("get_tokens", &getTokens, "Obtener Tokens a partir de una string");
}