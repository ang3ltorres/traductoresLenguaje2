#pragma once
#include <string>
#include <memory>
#include <vector>

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
		BinaryExpression,
	};

	ASTNode(ASTNode::Type type, unsigned int lineNumber);

	Type type;
	unsigned int lineNumber;
};

struct NodeIdentifier: public ASTNode
{
	NodeIdentifier(ASTNode::Type type, unsigned int lineNumber, const std::string& name);
	std::string name;
};

struct NodeNumber: public ASTNode
{
	NodeNumber(ASTNode::Type type, unsigned int lineNumber, int value);
	int value;
};

struct NodeExpression: public ASTNode
{
	NodeExpression(ASTNode::Type type, unsigned int lineNumber, std::shared_ptr<ASTNode> t);
	// Term, BinaryExpression
	std::shared_ptr<ASTNode> term;
};

struct NodeTerm: public ASTNode
{
	// Factor, BinaryExpression
	std::shared_ptr<ASTNode> factor;
};

struct NodeBinaryExpression: public ASTNode
{
	enum class Operation: int
	{
		Addition = 0,
		Subtraction,
		Multiplication,
		Division,
	};

	NodeBinaryExpression(ASTNode::Type type, unsigned int lineNumber, Operation operation, std::shared_ptr<ASTNode> l, std::shared_ptr<ASTNode> r);
	
	Operation operation;
	std::shared_ptr<ASTNode> left;
	std::shared_ptr<ASTNode> right;
};

struct NodeFactor: public ASTNode
{
	// Identifier, Number, Expression
	std::shared_ptr<ASTNode> factor;
};

struct NodeAssignment: public ASTNode
{
	NodeAssignment(ASTNode::Type type, unsigned int lineNumber, std::shared_ptr<NodeIdentifier> v);
	std::shared_ptr<NodeIdentifier> var;
};

struct NodeProgram: public ASTNode
{
	NodeProgram(ASTNode::Type type, unsigned int lineNumber, std::vector<std::shared_ptr<NodeAssignment>> nodeAssignment);
	std::vector<std::shared_ptr<NodeAssignment>> node;
};
