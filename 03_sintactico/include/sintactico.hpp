#pragma once
#include <string>
#include <memory>
#include <vector>

#include "lexico.hpp"

struct ASTNode
{
	enum class Type : int
	{
		Program = 0,
		Assignament,
		Identifier,
		Expression,
		Term,
		Factor,
		Number,
		FloatingPointNumber,
		DataType,
		RelationalOperator,
		BinaryExpression,
	};

	ASTNode::Type type;
	unsigned int lineNumber;
};

struct NodeIdentifier: public ASTNode
{
	NodeIdentifier(unsigned int line, const std::string& name);
	std::string name;
};

struct NodeNumber: public ASTNode
{
	NodeNumber(unsigned int line, int value);
	int value;
};

struct NodeFloatingPointNumber: public ASTNode
{
	NodeFloatingPointNumber(unsigned int line, float value);
	float value;
};

struct NodeDataType: public ASTNode
{
	enum class Type : int
	{
		Void = 0,
		Char,
		Short,
		Int,
		Long,
		Float,
		Bool,
	};

	NodeDataType(unsigned int line, NodeDataType::Type dataType);
	NodeDataType::Type dataType;
};

struct NodeRelationalOperator: public ASTNode
{
	enum class Type : int
	{
		LessThan = 0,
		GreaterThan,
		LessThanOrEqual,
		GreaterThanOrEqual,
		EqualTo,
		NotEqualTo,
	};

	NodeRelationalOperator(unsigned int line, NodeRelationalOperator::Type operatorType);
	NodeRelationalOperator::Type operatorType;
};

struct NodeExpression: public ASTNode
{
	NodeExpression(std::shared_ptr<ASTNode> term);

	// Term, BinaryExpression
	std::shared_ptr<ASTNode> term;
};

struct NodeTerm : public ASTNode
{
	NodeTerm(std::shared_ptr<ASTNode> factor);

	// Factor, BinaryExpression
	std::shared_ptr<ASTNode> factor;
};

struct NodeBinaryExpression : public ASTNode
{
	enum class Operation : int
	{
		Addition = 0,
		Subtraction,
		Multiplication,
		Division,
	};

	NodeBinaryExpression(Operation operation, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right);
	Operation operation;
	std::shared_ptr<ASTNode> left;
	std::shared_ptr<ASTNode> right;
};

struct NodeFactor : public ASTNode
{
	NodeFactor(std::shared_ptr<ASTNode> factor);

	// Identifier, Number, Expression
	std::shared_ptr<ASTNode> factor;
};

struct NodeAssignment : public ASTNode
{
	NodeAssignment(std::shared_ptr<ASTNode> var, std::shared_ptr<ASTNode> exp);
	std::shared_ptr<ASTNode> var;
	std::shared_ptr<ASTNode> exp;
};

struct NodeProgram : public ASTNode
{
	NodeProgram(std::vector<std::shared_ptr<NodeAssignment>> node);
	std::vector<std::shared_ptr<NodeAssignment>> node;
};

class Parser
{
public:
	Parser(const std::vector<Token>& tokens);
	
	bool notEnd();

	const std::vector<Token>& tokens;
	size_t index;

	std::shared_ptr<NodeProgram> parseProgram();
	Token getNextToken();

	std::shared_ptr<NodeIdentifier> parseIdentifier();
	std::shared_ptr<NodeNumber> parseNumber();
	std::shared_ptr<ASTNode> parseDataType();
	std::shared_ptr<ASTNode> parseRelationalOperator();
	std::shared_ptr<ASTNode> parseExpression();
	std::shared_ptr<ASTNode> parseTerm();
	std::shared_ptr<ASTNode> parseFactor();
	std::shared_ptr<NodeAssignment> parseAssignment();
};
