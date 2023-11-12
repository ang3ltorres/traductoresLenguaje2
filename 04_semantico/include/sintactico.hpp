#pragma once
#include <string>
#include <memory>
#include <vector>
#include <utility>

#include "lexico.hpp"
#include "generic.hpp"

struct ASTNode
{
	enum class Type : int
	{
		Program = 0,
		Function,
		Statement,
		IfStatement,
		Condition,
		Assignament,
		Declaration,
		Identifier,
		Expression,
		Argument,
		Parameters,
		Term,
		Factor,
		ArrayAccess,
		Number,
		FloatingPointNumber,
		DataType,
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
	NodeNumber(unsigned int line, float value, bool decimal);
	float value;
	bool decimal;
};

struct NodeDataType: public ASTNode
{
	NodeDataType(unsigned int line, DataType dataType);
	DataType dataType;
};

struct NodeArgument: public ASTNode
{
	NodeArgument(unsigned int line, Node dataType, Node identifier);

	Node dataType;
	Node identifier;
};

struct NodeParamaters: public ASTNode
{
	NodeParamaters(unsigned int line, std::vector<Node> args);

	std::vector<Node> args;
};

struct NodeExpression: public ASTNode
{
	NodeExpression(Node term);

	// Term, BinaryExpression
	Node term;
};

struct NodeCondition : public ASTNode
{
	NodeCondition(unsigned int line, Node condition);

	// Expression, BinaryExpression<Expression (rop) Expression>
	Node condition;
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
		Unknown = 0,
		Addition,
		Subtraction,
		Multiplication,
		Division,
		Or,
		And,
		LessThan,
		GreaterThan,
		LessThanOrEqual,
		GreaterThanOrEqual,
		EqualTo,
		NotEqualTo,
	};

	enum class Type : int
	{
		Expression = 0,
		Term
	};

	NodeBinaryExpression(Operation operation, NodeBinaryExpression::Type type, Node left, Node right);

	// WTF TODO
	static NodeBinaryExpression::Operation toOperation(Token token);
	
	Operation operation;
	Type type;
	Node left;
	Node right;
};

struct NodeFactor : public ASTNode
{
	NodeFactor(Node factor);

	// Identifier, Number, Expression, ArrayAccess
	Node factor;
};

struct NodeArrayAccess : public ASTNode
{
	NodeArrayAccess(Node identifier, Node expIndex);

	Node identifier;
	Node expIndex;
};

struct NodeAssignment : public ASTNode
{
	NodeAssignment(Node var, Node exp, Node expIndex);
	Node var;
	Node exp;
	Node expIndex;
};

struct NodeDeclaration : public ASTNode
{
	NodeDeclaration(unsigned int line, Node dataType, Node node, int size);

	Node dataType;

	// Identifier, Assignment
	Node node;

	// isArray >= 0 | -1 notArray
	int size;
};

struct NodeLogicalExpression : public ASTNode
{
	NodeLogicalExpression(unsigned int line, Node logicalExpression);

	// LogicalTerm, BinaryExpression<LogicalTerm || LogicalExpression>, LogicalExpression
	Node logicalExpression;
};

struct NodeLogicalTerm : public ASTNode
{
	NodeLogicalTerm(unsigned int line, Node logicalTerm);

	// RelationalExpression, BinaryExpression<RelationalExpression && LogicalTerm>
	Node logicalTerm;
};

struct NodeRelationalExpression : public ASTNode
{
	NodeRelationalExpression(unsigned int line, Node expression);

	// Expression, BinaryExpression<Expression (op) Expression>
	Node expression;
};

struct NodeIfStatement : public ASTNode
{
	NodeIfStatement(unsigned int line, Node condition, std::vector<Node> statements, std::vector<Node> elseStatements);

	Node condition;
	std::vector<Node> statements;
	std::vector<Node> elseStatements;
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
		std::vector<Node> statements
	);

	Node datatype;
	Node identifier;
	Node parameters;
	std::vector<Node> statements;
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
	
	void notEnd();

	const std::vector<Token>& tokens;
	size_t index;

	std::shared_ptr<NodeProgram> parseProgram();
	Token getNextToken();

	std::shared_ptr<NodeIdentifier> parseIdentifier();
	std::shared_ptr<NodeNumber> parseNumber();
	Node parseDataType();
	Node parseExpression();
	Node parseTerm();
	Node parseFactor();
	// Node parseArrayAccess();
	Node parseArgument();
	Node parseParameters();
	Node parseReturnStatement();
	std::shared_ptr<NodeAssignment> parseAssignment();
	std::shared_ptr<NodeDeclaration> parseDeclaration();
	Node parseCondition();
	Node parseIfStatement();
	std::shared_ptr<NodeStatement> parseStatement();
	std::shared_ptr<NodeFunction> parseFunction();
};
