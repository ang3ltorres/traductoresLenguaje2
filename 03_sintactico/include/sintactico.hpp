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
		Function,
		Statement,
		IfStatement,
		LogicalExpression,
		LogicalTerm,
		RelationalExpression,
		Assignament,
		Declaration,
		Identifier,
		Expression,
		Argument,
		Parameters,
		Term,
		Factor,
		Number,
		FloatingPointNumber,
		DataType,
		RelationalOperator,
		ReturnStatement,
		BinaryExpression,
	};

	ASTNode::Type type;
	unsigned int lineNumber;
};

using Node = std::shared_ptr<ASTNode>;

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

struct NodeArgument: public ASTNode
{
	NodeArgument(unsigned int line, Node dataType, Node identifier);

	Node dataType;
	Node identifier;
};

struct NodeParamaters: public ASTNode
{
	NodeParamaters(unsigned int line, std::vector< Node > args);

	std::vector< Node > args;
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
	NodeExpression(Node term);

	// Term, BinaryExpression
	Node term;
};

struct NodeTerm : public ASTNode
{
	NodeTerm(Node factor);

	// Factor, BinaryExpression
	Node factor;
};

struct NodeReturnStatement : public ASTNode
{
	NodeReturnStatement(unsigned int line, Node expression);
	Node expression;
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

	NodeBinaryExpression(Operation operation, Node left, Node right);
	Operation operation;
	Node left;
	Node right;
};

struct NodeFactor : public ASTNode
{
	NodeFactor(Node factor);

	// Identifier, Number, Expression
	Node factor;
};

struct NodeAssignment : public ASTNode
{
	NodeAssignment(Node var, Node exp);
	Node var;
	Node exp;
};

struct NodeDeclaration : public ASTNode
{
	NodeDeclaration(unsigned int line, Node dataType, Node node);

	Node dataType;

	// Identifier, Assignment
	Node node;
};

struct NodeLogicalExpression : public ASTNode
{
	NodeLogicalExpression(unsigned int line, Node logicalExpression);

	// LogicalTerm, BinaryExpression<LogicalExpression || LogicalTerm>
	Node logicalExpression;
};

struct NodeLogicalTerm : public ASTNode
{
	NodeLogicalTerm(unsigned int line, Node logicalTerm);

	// RelationalExpression, BinaryExpression<LogicalTerm && RelationalExpression>
	Node logicalTerm;
};

struct NodeRelationalExpression : public ASTNode
{
	NodeRelationalExpression(unsigned int line, Node left, Node op, Node right);

	// Expression
	Node left;
	Node right;

	// RelationalOperator
	Node op;
};

struct NodeIfStatement : public ASTNode
{
	NodeIfStatement(unsigned int line, Node expression, std::vector< Node > statements);

	Node expression;
	std::vector< Node > statements;
};

struct NodeStatement : public ASTNode
{
	NodeStatement(unsigned int line, Node statement);

	// Declaration, Assignment, IfStatement, ReturnStatement
	Node statement;
};

struct NodeFunction : public ASTNode
{
	NodeFunction(unsigned int line,
		Node datatype,
		Node identifier,
		Node parameters,
		std::vector< Node > statements
	);

	Node datatype;
	Node identifier;
	Node parameters;
	std::vector< Node > statements;
};

struct NodeProgram : public ASTNode
{
	NodeProgram(std::vector<std::shared_ptr<NodeFunction>> functions);
	std::vector<std::shared_ptr<NodeFunction>> functions;
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
	Node parseDataType();
	Node parseRelationalOperator();
	Node parseExpression();
	Node parseTerm();
	Node parseFactor();
	Node parseArgument();
	Node parseParameters();
	Node parseReturnStatement();
	std::shared_ptr<NodeAssignment> parseAssignment();
	std::shared_ptr<NodeDeclaration> parseDeclaration();
	Node parseIfStatement();
	std::shared_ptr<NodeStatement> parseStatement();
	std::shared_ptr<NodeFunction> parseFunction();
};

#ifdef PYTHON_LIB
	
	std::string parseTokens(const std::vector<Token>& tokens);

#endif
