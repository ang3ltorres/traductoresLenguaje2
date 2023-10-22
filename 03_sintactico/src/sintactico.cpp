#include "sintactico.hpp"

#include <stdexcept>
#include <algorithm>
#include <format>

static bool isDataType(Token::Type type)
{
	static const std::vector<Token::Type> dataTypes =
	{
		Token::Type::DataTypeVoid,
		Token::Type::DataTypeChar,
		Token::Type::DataTypeShort,
		Token::Type::DataTypeInt,
		Token::Type::DataTypeLong,
		Token::Type::DataTypeFloat,
		Token::Type::DataTypeBool,
	};

	return std::any_of(dataTypes.begin(), dataTypes.end(), [type](Token::Type dataType){ return type == dataType; });
};

static NodeBinaryExpression::Operation isRelationalOperator(Token::Type type)
{
	switch (type)
	{
		case Token::Type::LessThan:					return NodeBinaryExpression::Operation::LessThan;
		case Token::Type::GreaterThan:				return NodeBinaryExpression::Operation::GreaterThan;
		case Token::Type::LessThanOrEqual:			return NodeBinaryExpression::Operation::LessThanOrEqual;
		case Token::Type::GreaterThanOrEqual:		return NodeBinaryExpression::Operation::GreaterThanOrEqual;
		case Token::Type::EqualTo:					return NodeBinaryExpression::Operation::EqualTo;
		case Token::Type::NotEqualTo:				return NodeBinaryExpression::Operation::NotEqualTo;
		default:									return NodeBinaryExpression::Operation::Unknown;
	}
}

ErrorCode::ErrorCode(unsigned int line, std::string errorStr)
: line(line), errorStr(errorStr) {}

const char* ErrorCode::what() const noexcept
{
	static std::string aux;
	aux.clear();
	aux += std::format("Error: {:s}\n\tLinea: {:d}\n", errorStr, line);
	return aux.c_str();
}

NodeIdentifier::NodeIdentifier(unsigned int line, const std::string& name)
: ASTNode{ASTNode::Type::Identifier, line}, name(name) {}

NodeNumber::NodeNumber(unsigned int line, float value, bool decimal)
: ASTNode{ASTNode::Type::Number, line}, value(value), decimal(decimal) {}

NodeDataType::NodeDataType(unsigned int line, NodeDataType::Type dataType)
: ASTNode{ASTNode::Type::DataType, line}, dataType(dataType) {}

NodeArgument::NodeArgument(unsigned int line, Node dataType, Node identifier)
: ASTNode{ASTNode::Type::Argument, line}, dataType(dataType), identifier(identifier) {}

NodeParamaters::NodeParamaters(unsigned int line, std::vector<Node> args)
: ASTNode{ASTNode::Type::Parameters, line}, args(args) {}

NodeExpression::NodeExpression(Node term)
: ASTNode{ASTNode::Type::Expression, term->lineNumber}, term(term) {}

NodeCondition::NodeCondition(unsigned int line, Node condition)
: ASTNode{ASTNode::Type::Condition, line}, condition(condition) {}

NodeTerm::NodeTerm(Node factor)
: ASTNode{ASTNode::Type::Term, factor->lineNumber}, factor(factor) {}

NodeReturnStatement::NodeReturnStatement(unsigned int line, Node expression)
: ASTNode{ASTNode::Type::ReturnStatement, line}, expression(expression) {}

NodeBinaryExpression::NodeBinaryExpression(Operation operation, Node left, Node right)
: ASTNode{ASTNode::Type::BinaryExpression, left->lineNumber}, operation(operation), left(left), right(right) {}

NodeBinaryExpression::Operation NodeBinaryExpression::toOperation(Token token)
{
	switch (token.type)
	{
		case Token::Type::Addition:				return NodeBinaryExpression::Operation::Addition;
		case Token::Type::Subtraction:			return NodeBinaryExpression::Operation::Subtraction;
		case Token::Type::Multiplication:		return NodeBinaryExpression::Operation::Multiplication;
		case Token::Type::Division:				return NodeBinaryExpression::Operation::Division;
		case Token::Type::LogicalOR:			return NodeBinaryExpression::Operation::Or;
		case Token::Type::LogicalAND:			return NodeBinaryExpression::Operation::And;
		case Token::Type::LessThan:				return NodeBinaryExpression::Operation::LessThan;
		case Token::Type::GreaterThan:			return NodeBinaryExpression::Operation::GreaterThan;
		case Token::Type::LessThanOrEqual:		return NodeBinaryExpression::Operation::LessThanOrEqual;
		case Token::Type::GreaterThanOrEqual:	return NodeBinaryExpression::Operation::GreaterThanOrEqual;
		case Token::Type::EqualTo:				return NodeBinaryExpression::Operation::EqualTo;
		case Token::Type::NotEqualTo:			return NodeBinaryExpression::Operation::NotEqualTo;
		default:								throw ErrorCode(token.line, "Se esperaba un operador");
	}
}

NodeFactor::NodeFactor(Node factor)
: ASTNode{ASTNode::Type::Factor, factor->lineNumber}, factor(factor) {}

NodeAssignment::NodeAssignment(Node var, Node exp)
: ASTNode{ASTNode::Type::Assignament, var->lineNumber}, var(var), exp(exp) {}

NodeDeclaration::NodeDeclaration(unsigned int line, Node dataType, Node node)
: ASTNode{ASTNode::Type::Declaration, line}, dataType(dataType), node(node) {}

NodeIfStatement::NodeIfStatement(unsigned int line, Node condition, std::vector<Node> statements)
: ASTNode{ASTNode::Type::IfStatement, line}, condition(condition), statements(statements) {}

NodeStatement::NodeStatement(unsigned int line, Node statement)
: ASTNode{ASTNode::Type::Statement, line}, statement(statement) {}

NodeFunction::NodeFunction(unsigned int line,
	Node datatype,
	Node identifier,
	Node parameters,
	std::vector<Node> statements
)
: ASTNode{ASTNode::Type::Function, line}, datatype(datatype), identifier(identifier), parameters(parameters), statements(statements) {}

NodeProgram::NodeProgram(std::vector<std::shared_ptr<NodeFunction>> functions)
: ASTNode{ASTNode::Type::Program, 0}, functions(functions) {}

Parser::Parser(const std::vector<Token>& tokens)
: tokens(tokens), index(0) {}

void Parser::notEnd()
{
	if (!(index < tokens.size()))
		throw ErrorCode(tokens.back().line, "Se alcanzo el final de los tokens inesperadamente");
}

std::shared_ptr<NodeProgram> Parser::parseProgram()
{
	std::vector<std::shared_ptr<NodeFunction>> functions;
	while (index < tokens.size())
		functions.push_back(parseFunction());

	return std::make_shared<NodeProgram>(functions);
}

Token Parser::getNextToken()
{
	notEnd();
	return tokens[index++];
}

std::shared_ptr<NodeIdentifier> Parser::parseIdentifier()
{
	Token t = getNextToken();
	if (t.type == Token::Type::Identifier)
		return std::make_shared<NodeIdentifier>(t.line, t.lexema);
	else
		throw ErrorCode(t.line, "Se esperaba un identificador");
}

std::shared_ptr<NodeNumber> Parser::parseNumber()
{
	Token t = getNextToken();
	if (t.type == Token::Type::Number)
		return std::make_shared<NodeNumber>(t.line, std::stof(t.lexema), false);
	else if (t.type == Token::Type::FloatingPointNumber)
		return std::make_shared<NodeNumber>(t.line, std::stof(t.lexema), true);
	else
		throw ErrorCode(t.line, "Se esperaba un numero");
}

Node Parser::parseDataType()
{
	Token t = getNextToken();

	switch (t.type)
	{
		case Token::Type::DataTypeVoid:		return std::make_shared<NodeDataType>(t.line, NodeDataType::Type::Void);
		case Token::Type::DataTypeChar:		return std::make_shared<NodeDataType>(t.line, NodeDataType::Type::Char);
		case Token::Type::DataTypeShort:	return std::make_shared<NodeDataType>(t.line, NodeDataType::Type::Short);
		case Token::Type::DataTypeInt:		return std::make_shared<NodeDataType>(t.line, NodeDataType::Type::Int);
		case Token::Type::DataTypeLong:		return std::make_shared<NodeDataType>(t.line, NodeDataType::Type::Long);
		case Token::Type::DataTypeFloat:	return std::make_shared<NodeDataType>(t.line, NodeDataType::Type::Float);
		case Token::Type::DataTypeBool:		return std::make_shared<NodeDataType>(t.line, NodeDataType::Type::Bool);
		default:							throw ErrorCode(t.line, "Se esperaba un tipo de dato");
	}
}

Node Parser::parseExpression()
{
	Node leftTerm = parseTerm();
	notEnd();
	if (tokens[index].type == Token::Type::Addition || tokens[index].type == Token::Type::Subtraction)
	{
		Token op = getNextToken();
		Node rightTerm = parseExpression();
		return std::make_shared<NodeBinaryExpression>
		(
			(op.type == Token::Type::Addition) ? NodeBinaryExpression::Operation::Addition : NodeBinaryExpression::Operation::Subtraction,
			leftTerm,
			rightTerm
		);
	}
	
	return leftTerm;
}

Node Parser::parseTerm()
{
	Node factor = parseFactor();
	notEnd();

	if (tokens[index].type == Token::Type::Multiplication || tokens[index].type == Token::Type::Division)
	{
		Token op = getNextToken();
		Node right = parseTerm();
		return std::make_shared<NodeBinaryExpression>
		(
			(op.type == Token::Type::Multiplication) ? NodeBinaryExpression::Operation::Multiplication : NodeBinaryExpression::Operation::Division,
			factor,
			right
		);
	}
	
	return factor;
}

Node Parser::parseFactor()
{
	notEnd();
	Token token = tokens[index];

	if (token.type == Token::Type::Identifier)
		return parseIdentifier();
	else if ((token.type == Token::Type::Number) || (token.type == Token::Type::FloatingPointNumber))
		return parseNumber();
	else if (token.type == Token::Type::ParenthesisOpen)
	{
		index++; // Saltarnos el (
		Node expression = parseExpression();
		notEnd();
		if (tokens[index].type == Token::Type::ParenthesisClose)
		{
			index++; // Saltarnos el )
			return expression;
		}
		else
			throw ErrorCode(tokens[index].line, "Se esperaba un parentesis de cierre");
	}
	else
		throw ErrorCode(tokens[index-1].line, "Se esperaba una expresion");
}

Node Parser::parseArgument()
{
	Node dataType = parseDataType();
	Node identifier = parseIdentifier();

	return std::make_shared<NodeArgument>(dataType->lineNumber, dataType, identifier);
}

Node Parser::parseParameters()
{
	std::vector<Node> args;

	notEnd();

	if (tokens[index].type == Token::Type::ParenthesisClose)
		return std::make_shared<NodeParamaters>(tokens[index].line, args);
	else if (!isDataType(tokens[index].type))
		throw ErrorCode(tokens[index-1].line, "Se esperaba parentesis de cierre o una lista de argumentos");

	while (true)
	{
		Node arg = parseArgument();
		args.push_back(arg);

		notEnd();

		if (tokens[index].type == Token::Type::ParenthesisClose)
		{
			return std::make_shared<NodeParamaters>(arg->lineNumber, args);
		}
		else if (tokens[index].type == Token::Type::Comma)
		{
			index++; // Saltarnos la ,
			continue;
		}
		else
			throw ErrorCode(arg->lineNumber, "Se esperaba un parentesis de cierre o una coma");
	}
}

// TODO
// Reutilizar en parseRelationalOperator()
// Usar NodeBinaryExpression
/*
	En plan validar: if ( <expression> ) { <statement>* }
	if (a + b){}
	if (a) {}
	if ((a > b) && (c - 9)) {}

	Agregar a enum class Operation los &&, y asi
	Eliminar NodeCondition ?? y usar NodeBinaryExpression
*/

Node Parser::parseReturnStatement()
{
	notEnd();

	if (tokens[index].type == Token::Type::ReservedWordReturn)
	{
		index++; // Saltar el return
		Node expression = parseExpression();

		notEnd();
		if (tokens[index].type == Token::Type::Semicolon)
		{
			index++; // Saltarnos el ;
			return std::make_shared<NodeReturnStatement>(tokens[index].line, expression);
		}
		else
			throw ErrorCode(expression->lineNumber, "Se esperaba un punto y coma");
	}
	else
		throw ErrorCode(tokens[index].line, "Se esperaba un return");
}

std::shared_ptr<NodeAssignment> Parser::parseAssignment()
{
	std::shared_ptr<NodeIdentifier> identifier = parseIdentifier();

	notEnd();

	if (tokens[index].type == Token::Type::Assignment)
	{
		index++; // Saltarnos el =
		Node expression = parseExpression();
		notEnd();

		if (tokens[index].type == Token::Type::Semicolon)
		{
			index++; // Saltarnos el ;
			return std::make_shared<NodeAssignment>(identifier, expression);
		}
		else
			throw ErrorCode(expression->lineNumber, "Se esperaba un punto y coma");
	}
	else
		throw ErrorCode(identifier->lineNumber, "Se esperaba un operador de asignacion");
}

std::shared_ptr<NodeDeclaration> Parser::parseDeclaration()
{
	Node dataType = parseDataType();
	Node identifier = parseIdentifier();

	Token t = getNextToken();
	if (t.type == Token::Type::Semicolon)
	{
		return std::make_shared<NodeDeclaration>(dataType->lineNumber, dataType, identifier);
	}
	else if (t.type == Token::Type::Assignment)
	{
		Node expression = parseExpression();
		t = getNextToken();
		if (t.type == Token::Type::Semicolon)
		{
			Node assignment = std::make_shared<NodeAssignment>(identifier, expression);

			return std::make_shared<NodeDeclaration>(dataType->lineNumber, dataType, assignment);
		}
		else
			throw ErrorCode(expression->lineNumber, "Se esperaba un punto y coma");
	}
	
	throw ErrorCode(identifier->lineNumber, "Se esperaba un operador de asignacion o un punto y coma");
}

Node Parser::parseCondition()
{

	Node left = parseExpression();
	notEnd();
	NodeBinaryExpression::Operation op = isRelationalOperator(tokens[index].type);

	if (op != NodeBinaryExpression::Operation::Unknown)
	{
		index++; // Saltarnos el op

		Node right = parseExpression();

		return std::make_shared<NodeCondition>(right->lineNumber,
			std::make_shared<NodeBinaryExpression>(op, left, right)
		);
	}
	else
		return std::make_shared<NodeCondition>(left->lineNumber, left);
}

Node Parser::parseIfStatement()
{
	unsigned int lineNumber;
	Token t;

	t = getNextToken();
	lineNumber = t.line;
	if (t.type != Token::Type::ReservedWordIf)
		throw ErrorCode(lineNumber, "Se esperaba un if");

	t = getNextToken();
	lineNumber = t.line;
	if (t.type != Token::Type::ParenthesisOpen)
		throw ErrorCode(lineNumber, "Se esperaba un parentesis abre");

	Node condition = parseCondition();

	t = getNextToken();
	lineNumber = t.line;
	if (t.type != Token::Type::ParenthesisClose)
		throw ErrorCode(lineNumber, "Se esperaba un parentesis de cierre");

	t = getNextToken();
	if (t.type != Token::Type::BraceOpen)
		throw ErrorCode(lineNumber, "Se esperaba un corchete abre");
	
	/******/
	std::vector<Node> statements;

	t = getNextToken();
	if (t.type == Token::Type::BraceClose)
		return std::make_shared<NodeIfStatement>(t.line, condition, statements);

	while (true)
	{
		Node statement = parseStatement();
		statements.push_back(statement);
		notEnd();
		if (tokens[index].type == Token::Type::BraceClose)
		{
			index++; // Saltarnos el }
			return std::make_shared<NodeIfStatement>(statement->lineNumber, condition, statements);
		}
	}
}

std::shared_ptr<NodeStatement> Parser::parseStatement()
{
	// TODO

	// NO GENERALIZAR APRA ESPECIFICAR ERRORES
	// SOPORTE DE FLOATS
	// SOPORTE DE ELSE, ELSE IF
	// isDataType esta 2 veces xd

	notEnd();

	// ReturnStatement
	if (tokens[index].type == Token::Type::ReservedWordReturn)
	{
		Node node = parseReturnStatement();
		return std::make_shared<NodeStatement>(node->lineNumber, node);
	}
	// IfStatement
	else if (tokens[index].type == Token::Type::ReservedWordIf)
	{
		Node node = parseIfStatement();
		return std::make_shared<NodeStatement>(node->lineNumber, node);
	}
	// Assignment
	else if (tokens[index].type == Token::Type::Identifier)
	{
		Node node = parseAssignment();
		return std::make_shared<NodeStatement>(node->lineNumber, node);
	}
	// Declaration
	else if (isDataType(tokens[index].type))
	{
		Node node = parseDeclaration();
		return std::make_shared<NodeStatement>(node->lineNumber, node);
	}
	else
		throw ErrorCode(tokens[index].line, "Se esperaba un statement (Declaracion, Asignacion, If, Return)");
}

std::shared_ptr<NodeFunction> Parser::parseFunction()
{
	Token t;

	Node dataType = parseDataType();
	Node identifier = parseIdentifier();

	t = getNextToken();
	if (t.type != Token::Type::ParenthesisOpen)
		throw ErrorCode(identifier->lineNumber, "Se esperaba un parentesis abre");

	Node parameters = parseParameters();

	t = getNextToken();
	if (t.type != Token::Type::ParenthesisClose)
		throw ErrorCode(parameters->lineNumber, "Se esperaba parentesis de cierre");

	/******/
	std::vector<Node> statements;

	notEnd();
	if (tokens[index].type != Token::Type::BraceOpen)
		throw ErrorCode(t.line, "Se esperaba corchete abre");
	index++; // Saltarnos el {

	notEnd();
	if (tokens[index].type == Token::Type::BraceClose)
	{
		index++; // Saltar el }
		return std::make_shared<NodeFunction>(t.line, dataType, identifier, parameters, statements);
	}
	// else
	// 	throw std::runtime_error(std::format("Se esperaba corchete de cierre.\n\tLinea: {:d}\n", tokens[index].line));

	while (true)
	{
		Node statement = parseStatement();
		statements.push_back(statement);

		notEnd();
		if (tokens[index].type == Token::Type::BraceClose)
		{
			index++; // Saltar el }
			return std::make_shared<NodeFunction>(statement->lineNumber, dataType, identifier, parameters, statements);
		}
		else
			continue;
	}

	throw ErrorCode(tokens.size(), "Se alcanzo el final de los tokens inesperadamente");
}


#ifdef PYTHON_LIB

	ErrorCode parseTokens(const std::vector<Token>& tokens)
	{
		try
		{
			Parser parser(tokens);
			std::shared_ptr<NodeProgram> program = parser.parseProgram();
			return ErrorCode{0, "El programa no contiene errores!! :D"};
		}
		catch (const ErrorCode& e)
		{
			return e;
		}
	}

#endif
