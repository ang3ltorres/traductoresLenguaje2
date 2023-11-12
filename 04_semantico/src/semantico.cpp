#include "semantico.hpp"
#include <memory>
#include <iostream>
#include <format>
#include <cmath>

Value Value::operator+(const Value& other)
{
	return Value
	{
		true,
		other.isFloat || this->isFloat,
		this->value + other.value
	};
}

Value Value::operator-(const Value& other)
{
	return Value
	{
		true,
		other.isFloat || this->isFloat,
		this->value - other.value
	};
}

Value Value::operator*(const Value& other)
{
	return Value
	{
		true,
		other.isFloat || this->isFloat,
		this->value * other.value
	};
}

Value Value::operator/(const Value& other)
{
	return Value
	{
		true,
		other.isFloat || this->isFloat,
		this->value / other.value
	};
}

static std::vector<std::unordered_map<std::string, SymbolInfo>> symbolTable;

///
static Value parseFactor(const Node& node);
static Value parseTerm(const Node& node);
static Value parseExpression(const Node& node);
static void parseAssignment(const std::shared_ptr<NodeAssignment>& node);
static void parseDeclaration(const std::shared_ptr<NodeDeclaration>& node);
///

static Value parseFactor(const Node& node)
{
	switch (node->type)
	{
		case ASTNode::Type::Identifier:
		{
			auto id = std::static_pointer_cast<NodeIdentifier>(node);

			auto find = symbolTable.back().find(id->name);

			return find->second.values.front();
		}

		case ASTNode::Type::ArrayAccess:
		{
			auto arrayAccess = std::static_pointer_cast<NodeArrayAccess>(node);

			auto id = std::static_pointer_cast<NodeIdentifier>(arrayAccess->identifier);
			auto expIndex = std::static_pointer_cast<NodeExpression>(arrayAccess->expIndex);

			auto find = symbolTable.back().find(id->name);

			// Evaluate
			auto index = parseExpression(expIndex);

			return find->second.values[index.value];
		}

		case ASTNode::Type::Number: case ASTNode::Type::FloatingPointNumber:
		{
			auto number = std::static_pointer_cast<NodeNumber>(node);

			return Value
			{
				true,
				number->decimal,
				number->value
			};
		}

		case ASTNode::Type::Expression: case ASTNode::Type::BinaryExpression:
			return parseExpression(node);

		default: throw ErrorCode(node->lineNumber, "Nodo invalido");
	}
}

static Value parseTerm(const Node& node)
{
	bool isBinaryExpression = (node->type == ASTNode::Type::BinaryExpression);

	if (isBinaryExpression)
	{
		auto binaryExpression = std::static_pointer_cast<NodeBinaryExpression>(node);
		
		// Term tambien puede ser una Expression
		bool isExpression = binaryExpression->type == NodeBinaryExpression::Type::Expression;

		if (isExpression)
		{
			auto term = std::static_pointer_cast<NodeTerm>(binaryExpression->left);
			auto expression = std::static_pointer_cast<NodeExpression>(binaryExpression->right);

			switch (binaryExpression->operation)
			{
				case NodeBinaryExpression::Operation::Addition: return (parseTerm(term) + parseExpression(expression));
				case NodeBinaryExpression::Operation::Subtraction: return (parseTerm(term) - parseExpression(expression));
				default: throw ErrorCode(node->lineNumber, "Nodo invalido");
			}
		}
		else
		{
			auto factor = std::static_pointer_cast<NodeFactor>(binaryExpression->left);
			auto term = std::static_pointer_cast<NodeTerm>(binaryExpression->right);

			switch (binaryExpression->operation)
			{
				case NodeBinaryExpression::Operation::Multiplication: return (parseFactor(factor) * parseTerm(term));

				case NodeBinaryExpression::Operation::Division:
				{
					auto parsedTerm = parseTerm(term);
					if (parsedTerm.value == 0.0f)
						throw ErrorCode(term->lineNumber, "Division por cero");

					return (parseFactor(factor) / parsedTerm);
				}

				default: throw ErrorCode(node->lineNumber, "Nodo invalido");
			}
		}
	}
	else
	{
		auto factor = std::static_pointer_cast<NodeFactor>(node);
		return parseFactor(factor);
	}
}

static Value parseExpression(const Node& node)
{
	bool isBinaryExpression = (node->type == ASTNode::Type::BinaryExpression);

	if (isBinaryExpression)
	{
		auto binaryExpression = std::static_pointer_cast<NodeBinaryExpression>(node);

		// Es un Term o una Expression??
		bool isExpression = binaryExpression->type == NodeBinaryExpression::Type::Expression;

		if (isExpression)
		{
			auto term = std::static_pointer_cast<NodeTerm>(binaryExpression->left);
			auto expression = std::static_pointer_cast<NodeExpression>(binaryExpression->right);

			switch (binaryExpression->operation)
			{
				case NodeBinaryExpression::Operation::Addition: return (parseTerm(term) + parseExpression(expression));
				case NodeBinaryExpression::Operation::Subtraction: return (parseTerm(term) - parseExpression(expression));
				default: throw ErrorCode(node->lineNumber, "Nodo invalido");
			}
		}
		else
		{
			auto term = std::static_pointer_cast<NodeTerm>(node);
			return parseTerm(term);
		}
	}
	else
	{
		auto term = std::static_pointer_cast<NodeTerm>(node);
		return parseTerm(term);
	}
}

static void parseAssignment(const std::shared_ptr<NodeAssignment>& node)
{
	auto id = std::static_pointer_cast<NodeIdentifier>(node->var)->name;

	// Check symbol table
	auto find = symbolTable.back().find(id);

	if (find == symbolTable.back().end())
		throw ErrorCode(node->lineNumber, std::format("La variable \"{}\" no esta declarada previamente", id));
	
	// Evaluate expression, Change value
	auto newExpression = parseExpression(std::static_pointer_cast<NodeExpression>(node->exp));

	// float-float / int-int Validation
	if (!(find->second.type == DataType::Float && newExpression.isFloat) && !(find->second.type == DataType::Int && !newExpression.isFloat))
		throw ErrorCode(node->lineNumber, "Tipo de dato incorrecto");

	if (find->second.isArray)
	{
		// Index expression
		auto indexExpression = parseExpression(std::static_pointer_cast<NodeExpression>(node->expIndex));

		if (indexExpression.isFloat)
			throw ErrorCode(node->lineNumber, std::format("No es posible acceder a un arreglo usando flotantes"));

		if (indexExpression.value < 0 || indexExpression.value >= find->second.values.size())
			throw ErrorCode(node->lineNumber, std::format("Indice fuera de rango \"{}\", el tamanio del arreglo es {}", indexExpression.value, find->second.values.size()));

		find->second.values[indexExpression.value] = newExpression;
	}
	else
		find->second.values.front() = newExpression;

	std::cout << newExpression.value << '\n';
}

static void parseDeclaration(const std::shared_ptr<NodeDeclaration>& node)
{
	auto type = std::static_pointer_cast<NodeDataType>(node->dataType)->dataType;

	// Declaration or DeclarationInnitialization
	// Identifier or Assignment
	bool declaration = (node->node->type == ASTNode::Type::Identifier);

	std::string id;

	if (declaration)
		id = std::static_pointer_cast<NodeIdentifier>(node->node)->name;
	else		
		id = std::static_pointer_cast<NodeIdentifier>(std::static_pointer_cast<NodeAssignment>(node->node)->var)->name;

	// Check symbol table
	auto find = symbolTable.back().find(id);

	if (find == symbolTable.back().end())
	{
		if (declaration)
		{
			std::vector<Value> values((node->size == -1) ? (1) : (node->size), Value{false, false, 0.0f});

			symbolTable.back()[id] = SymbolInfo
			{
				static_cast<unsigned int>(symbolTable.size() - 1),
				node->lineNumber,
				type,
				SymbolInfo::SymbolType::Declaration,
				values,
				!(node->size == -1)
			};
		}
		else
		{
			auto assignment = std::static_pointer_cast<NodeAssignment>(node->node);
			auto expression = std::static_pointer_cast<NodeExpression>(assignment->exp);
			Value value = parseExpression(expression);

			// float-float / int-int Validation
			if (!(type == DataType::Float && value.isFloat) && !(type == DataType::Int && !value.isFloat))
				throw ErrorCode(expression->lineNumber, "Tipo de dato incorrecto");
			
			std::cout << value.value << '\n';

			symbolTable.back()[id] = SymbolInfo
			{
				static_cast<unsigned int>(symbolTable.size() - 1),
				node->lineNumber,
				type,
				SymbolInfo::SymbolType::DeclarationInnitialization,
				std::vector<Value>(1, value),
				false
			};

		}
	}
	else
		throw ErrorCode(node->lineNumber, std::format("Redefinicion de la variable (Linea: {}) \"{}\"", find->second.line, id));
}

void semanticAnalysis(const std::shared_ptr<NodeProgram>& program)
{
	// Tablas de simbolos, el index representa el scopeLevel - 1

	for (const std::shared_ptr<NodeFunction>& function : program->functions)
	{
		symbolTable.push_back(std::unordered_map<std::string, SymbolInfo>());

		// Function type
		DataType functionType = std::static_pointer_cast<NodeDataType>(function->datatype)->dataType;

		// Parameters
		auto nodeParameters = std::static_pointer_cast<NodeParamaters>(function->parameters);
		for (const auto& i : nodeParameters->args)
		{
			auto argument = std::static_pointer_cast<NodeArgument>(i);
			auto id = std::static_pointer_cast<NodeIdentifier>(argument->identifier);
			auto type = std::static_pointer_cast<NodeDataType>(argument->dataType)->dataType;

			if (symbolTable.back().find(id->name) == symbolTable.back().end())
			{
				Value value = Value
				{
					true,
					(type == DataType::Float) || (type == DataType::Double),
					0.0f
				};

				symbolTable.back()[id->name] = SymbolInfo
				{
					0,
					id->lineNumber,
					type,
					SymbolInfo::SymbolType::Argument,
					std::vector<Value>(1, value),
					false
				};
			}
			else
				throw ErrorCode(id->lineNumber, std::format("Redefinicion del parametro \"{}\"", id->name));
		}

		// Statements
		for (const auto& i : function->statements)
		{
			// Un-wrap statement
			auto statement = std::static_pointer_cast<NodeStatement>(i);

			if (statement->statement->type == ASTNode::Type::Declaration)
			{
				auto declaration = std::static_pointer_cast<NodeDeclaration>(statement->statement);
				parseDeclaration(declaration);
			}
			else if (statement->statement->type == ASTNode::Type::Assignament)
			{
				auto assignment = std::static_pointer_cast<NodeAssignment>(statement->statement);
				parseAssignment(assignment);
			}
		}

		symbolTable.pop_back();
	}
}
