#include "sintactico.hpp"

#include <stdexcept>

NodeIdentifier::NodeIdentifier(unsigned int line, const std::string& name)
: ASTNode{ASTNode::Type::Identifier, line}, name(name) {}

NodeNumber::NodeNumber(unsigned int line, int value)
: ASTNode{ASTNode::Type::Number, line}, value(value) {}

NodeFloatingPointNumber::NodeFloatingPointNumber(unsigned int line, float value)
: ASTNode{ASTNode::Type::FloatingPointNumber, line}, value(value) {}

NodeDataType::NodeDataType(unsigned int line, NodeDataType::Type dataType)
: ASTNode{ASTNode::Type::DataType, line}, dataType(dataType) {}

NodeRelationalOperator::NodeRelationalOperator(unsigned int line, NodeRelationalOperator::Type operatorType)
: ASTNode{ASTNode::Type::RelationalOperator, line}, operatorType(operatorType) {}

NodeExpression::NodeExpression(std::shared_ptr<ASTNode> term)
: ASTNode{ASTNode::Type::Expression, term->lineNumber}, term(term) {}

NodeTerm::NodeTerm(std::shared_ptr<ASTNode> factor)
: ASTNode{ASTNode::Type::Term, factor->lineNumber}, factor(factor) {}

NodeBinaryExpression::NodeBinaryExpression(Operation operation, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right)
: ASTNode{ASTNode::Type::BinaryExpression, left->lineNumber}, operation(operation), left(left), right(right) {}

NodeFactor::NodeFactor(std::shared_ptr<ASTNode> factor)
: ASTNode{ASTNode::Type::Factor, factor->lineNumber}, factor(factor) {}

NodeAssignment::NodeAssignment(std::shared_ptr<ASTNode> var, std::shared_ptr<ASTNode> exp)
: ASTNode{ASTNode::Type::Assignament, var->lineNumber}, var(var), exp(exp) {}

NodeProgram::NodeProgram(std::vector<std::shared_ptr<NodeAssignment>> node)
: ASTNode{ASTNode::Type::Program, 0}, node(node) {}

Parser::Parser(const std::vector<Token>& tokens)
: tokens(tokens), index(0) {}

bool Parser::notEnd() {return index < tokens.size();}

std::shared_ptr<NodeProgram> Parser::parseProgram()
{
	std::vector<std::shared_ptr<NodeAssignment>> assignments;
	while (notEnd())
		assignments.push_back(parseAssignment());

	return std::make_shared<NodeProgram>(assignments);
}

Token Parser::getNextToken()
{
	if (notEnd())
		return tokens[index++];
	else
		throw std::runtime_error("Se alcanzo el final de los tokens inesperadamente.");
}

std::shared_ptr<NodeIdentifier> Parser::parseIdentifier()
{
	Token t = getNextToken();
	if (t.type == Token::Type::Identifier)
		return std::make_shared<NodeIdentifier>(t.line, t.lexema);
	else
		throw std::runtime_error("Se esperaba un identificador.");
}

std::shared_ptr<NodeNumber> Parser::parseNumber()
{
	Token t = getNextToken();
	if (t.type == Token::Type::Number)
		return std::make_shared<NodeNumber>(t.line, std::stoi(t.lexema));
	else
		throw std::runtime_error("Se esperaba un numero.");
}

std::shared_ptr<ASTNode> Parser::parseExpression()
{
	std::shared_ptr<ASTNode> leftTerm = parseTerm();
	if (notEnd() && (tokens[index].type == Token::Type::Addition || tokens[index].type == Token::Type::Subtraction))
	{
		Token op = getNextToken();
		std::shared_ptr<ASTNode> rightTerm = parseExpression();
		return std::make_shared<NodeBinaryExpression>
		(
			(op.type == Token::Type::Addition) ? NodeBinaryExpression::Operation::Addition : NodeBinaryExpression::Operation::Subtraction,
			leftTerm,
			rightTerm
		);
	}
	
	return leftTerm;
}

std::shared_ptr<ASTNode> Parser::parseTerm()
{
	std::shared_ptr<ASTNode> factor = parseFactor();

	if (notEnd() && (tokens[index].type == Token::Type::Multiplication || tokens[index].type == Token::Type::Division))
	{
		Token op = getNextToken();
		std::shared_ptr<ASTNode> right = parseTerm();
		return std::make_shared<NodeBinaryExpression>
		(
			(op.type == Token::Type::Multiplication) ? NodeBinaryExpression::Operation::Multiplication : NodeBinaryExpression::Operation::Division,
			factor,
			right
		);
	}
	
	return factor;
}

std::shared_ptr<ASTNode> Parser::parseFactor()
{
	if (notEnd())
	{
		Token token = tokens[index];

		if (token.type == Token::Type::Identifier)
			return parseIdentifier();
		else if (token.type == Token::Type::Number)
			return parseNumber();
		else if (token.type == Token::Type::ParenthesisOpen)
		{
			index++; // Saltarnos el (
			std::shared_ptr<ASTNode> expression = parseExpression();
			if (notEnd() && tokens[index].type == Token::Type::ParenthesisClose)
			{
				index++;
				return expression;
			}
			else
				throw std::runtime_error("Se esperaba un parentesis de cierre.");
		}
	}

	throw std::runtime_error("Factor inválido.");
}

std::shared_ptr<NodeAssignment> Parser::parseAssignment()
{
	std::shared_ptr<NodeIdentifier> identifier = parseIdentifier();

	if (notEnd() && tokens[index].type == Token::Type::Assignment)
	{
		index++; // Saltarnos el =
		std::shared_ptr<ASTNode> expression = parseExpression();

		if (notEnd() && tokens[index].type == Token::Type::Semicolon)
		{
			index++; // Saltarnos el ;
			return std::make_shared<NodeAssignment>(identifier, expression);
		}
		else
			throw std::runtime_error("Se esperaba un punto y coma.");
	}
	
	throw std::runtime_error("Se esperaba un operador de asignacion.");
}
