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

Value Value::operator>(const Value& other)
{
	return Value
	{
		true,
		other.isFloat || this->isFloat,
		(this->value > other.value) ? 1.0f : 0.0f
	};
}

Value Value::operator<(const Value& other)
{
	return Value
	{
		true,
		other.isFloat || this->isFloat,
		(this->value < other.value) ? 1.0f : 0.0f
	};
}

Value Value::operator>=(const Value& other)
{
	return Value
	{
		true,
		other.isFloat || this->isFloat,
		(this->value >= other.value) ? 1.0f : 0.0f
	};
}

Value Value::operator<=(const Value& other)
{
	return Value
	{
		true,
		other.isFloat || this->isFloat,
		(this->value <= other.value) ? 1.0f : 0.0f
	};
}

Value Value::operator==(const Value& other)
{
	return Value
	{
		true,
		other.isFloat || this->isFloat,
		(this->value == other.value) ? 1.0f : 0.0f
	};
}

Value Value::operator!=(const Value& other)
{
	return Value
	{
		true,
		other.isFloat || this->isFloat,
		(this->value != other.value) ? 1.0f : 0.0f
	};
}

static std::vector<std::unordered_map<std::string, SymbolInfo>> symbolTable;

///
static Value parseFactor(const Node& node);
static Value parseTerm(const Node& node);
static Value parseExpression(const Node& node);
static void parseAssignment(const std::shared_ptr<NodeAssignment>& node);
static void parseDeclaration(const std::shared_ptr<NodeDeclaration>& node);
static void parseIfStatement(const std::shared_ptr<NodeIfStatement>& ifStatement, DataType functionType);
static void parseStatements(const std::vector<Node>& statements, DataType functionType);
static void parseParameters(const std::shared_ptr<NodeParamaters> parameters);
///

static Value parseFactor(const Node& node)
{
	switch (node->type)
	{
		case ASTNode::Type::Identifier:
		{
			auto id = std::static_pointer_cast<NodeIdentifier>(node);

			// Check symbol table
			bool found = false;
			static std::unordered_map<std::string, SymbolInfo>::iterator find;
			for (auto& st : symbolTable)
			{
				if ((find = st.find(id->name)) != symbolTable.back().end())
				{
					found = true;
					break;
				}
			}
			if (!found) throw ErrorCode(node->lineNumber, std::format("La variable \"{}\" no esta declarada previamente", id->name));

			if (find->second.isArray) throw ErrorCode(node->lineNumber, std::format("La variable \"{}\" es un arreglo", id->name));

			return find->second.values.front();
		}

		case ASTNode::Type::ArrayAccess:
		{
			auto arrayAccess = std::static_pointer_cast<NodeArrayAccess>(node);

			auto id = std::static_pointer_cast<NodeIdentifier>(arrayAccess->identifier);
			auto expIndex = std::static_pointer_cast<NodeExpression>(arrayAccess->expIndex);

			// Check symbol table
			bool found = false;
			static std::unordered_map<std::string, SymbolInfo>::iterator find;
			for (auto& st : symbolTable)
			{
				if ((find = st.find(id->name)) != symbolTable.back().end())
				{
					found = true;
					break;
				}
			}
			if (!found) throw ErrorCode(node->lineNumber, std::format("La variable \"{}\" no esta declarada previamente", id->name));
			if (!find->second.isArray) throw ErrorCode(node->lineNumber, std::format("La variable \"{}\" no es un arreglo", id->name));

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
				
				case NodeBinaryExpression::Operation::GreaterThan: return (parseTerm(term) > parseExpression(expression));
				case NodeBinaryExpression::Operation::LessThan: return (parseTerm(term) < parseExpression(expression));
				case NodeBinaryExpression::Operation::GreaterThanOrEqual: return (parseTerm(term) >= parseExpression(expression));
				case NodeBinaryExpression::Operation::LessThanOrEqual: return (parseTerm(term) <= parseExpression(expression));
				case NodeBinaryExpression::Operation::EqualTo: return (parseTerm(term) == parseExpression(expression));
				case NodeBinaryExpression::Operation::NotEqualTo: return (parseTerm(term) != parseExpression(expression));

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
	bool found = false;
	static std::unordered_map<std::string, SymbolInfo>::iterator find;
	for (auto& st : symbolTable)
	{
		if ((find = st.find(id)) != symbolTable.back().end())
		{
			found = true;
			break;
		}
	}

	if (!found) throw ErrorCode(node->lineNumber, std::format("La variable \"{}\" no esta declarada previamente", id));
	if (!find->second.isArray && node->expIndex) throw ErrorCode(node->lineNumber, std::format("La variable \"{}\" no es un arreglo", id));
	
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

	// Check symbol table for redeclaration
	static std::unordered_map<std::string, SymbolInfo>::const_iterator find;
	for (const auto& st : symbolTable)
		if ((find = st.find(id)) != symbolTable.back().end())
			throw ErrorCode(node->lineNumber, std::format("Redefinicion de la variable (Linea: {}) \"{}\"", find->second.line, id));

	if (declaration)
	{
		if (node->size == 0)
			throw ErrorCode(node->lineNumber, "No es posible declarar un arreglo vacio");

		std::vector<Value> values((node->size == -1) ? (1) : (node->size), Value{false, false, 0.0f});

		symbolTable.back()[id] = SymbolInfo
		{
			static_cast<unsigned int>(symbolTable.size() - 1),
			node->lineNumber,
			type,
			SymbolInfo::SymbolType::Declaration,
			values,
			!(node->size == -1),
			false,
			std::vector<NodeArgument>()
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
			false,
			false,
			std::vector<NodeArgument>()
		};

	}
}

static void parseStatements(const std::vector<Node>& statements, DataType functionType)
{
	for (const auto& i : statements)
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
		else if (statement->statement->type == ASTNode::Type::ReturnStatement)
		{
			auto returnStatement = std::static_pointer_cast<NodeReturnStatement>(statement->statement);
			auto nodeExpression = std::static_pointer_cast<NodeExpression>(returnStatement->expression);
			auto expression = parseExpression(nodeExpression);

			if ((expression.isFloat && functionType == DataType::Int) || (!expression.isFloat && functionType == DataType::Float))
				throw ErrorCode(nodeExpression->lineNumber, std::format("Tipo de dato de retorno incorrecto"));
		}
		else if (statement->statement->type == ASTNode::Type::IfStatement)
		{
			auto ifStatement = std::static_pointer_cast<NodeIfStatement>(statement->statement);
			parseIfStatement(ifStatement, functionType);
		}
	}
}

static void parseIfStatement(const std::shared_ptr<NodeIfStatement>& ifStatement, DataType functionType)
{
	auto nodeExpression = std::static_pointer_cast<NodeExpression>(ifStatement->expression);
	auto conditionValue = parseExpression(nodeExpression);
	std::cout << conditionValue.value << '\n';

	symbolTable.push_back(std::unordered_map<std::string, SymbolInfo>());
	
	if (conditionValue.value != 0.0f)
		parseStatements(ifStatement->statements, functionType);
	else
		parseStatements(ifStatement->elseStatements, functionType);

	symbolTable.pop_back();
}

static void parseParameters(const std::shared_ptr<NodeParamaters> parameters)
{
	for (const auto& i : parameters->args)
	{
		auto argument = std::static_pointer_cast<NodeArgument>(i);
		auto id = std::static_pointer_cast<NodeIdentifier>(argument->identifier);
		auto type = std::static_pointer_cast<NodeDataType>(argument->dataType)->dataType;

		if (symbolTable.back().find(id->name) == symbolTable.back().end())
		{
			if (argument->size == 0)
				throw ErrorCode(argument->lineNumber, "No es posible declarar un arreglo vacio");

			std::vector<Value> values((argument->size == -1 ? 1 : argument->size), Value{false, type == DataType::Float, 0.0f});

			symbolTable.back()[id->name] = SymbolInfo
			{
				0,
				id->lineNumber,
				type,
				SymbolInfo::SymbolType::Argument,
				values,
				!(argument->size == -1),
				false,
				std::vector<NodeArgument>()
			};
		}
		else
			throw ErrorCode(id->lineNumber, std::format("Redefinicion del parametro \"{}\"", id->name));
	}
}

void semanticAnalysis(const std::shared_ptr<NodeProgram>& program)
{
	// Tablas de simbolos, el index representa el scopeLevel - 1
	symbolTable.push_back(std::unordered_map<std::string, SymbolInfo>());

	for (const Node& functionNode : program->functions)
	{
		if (functionNode->type == ASTNode::Type::FunctionDeclaration)
		{

			// (find = st.find(id)) != symbolTable.back().end()

			auto functionDeclaration = std::static_pointer_cast<NodeFunctionDeclaration>(functionNode);

			auto id = std::static_pointer_cast<NodeIdentifier>(functionDeclaration->identifier)->name;
			auto find = symbolTable.front().find(id);

			if (find != symbolTable.back().end())
				throw ErrorCode(functionDeclaration->lineNumber, std::format("Redeclaracion de la funcion (Linea: {}) \"{}\"", find->second.line, id));

			// Function type
			DataType functionType = std::static_pointer_cast<NodeDataType>(functionDeclaration->datatype)->dataType;

			// Parameters
			auto nodeParameters = std::static_pointer_cast<NodeParamaters>(functionDeclaration->parameters);
			symbolTable.push_back(std::unordered_map<std::string, SymbolInfo>());
			parseParameters(nodeParameters);
			symbolTable.pop_back();

			std::vector<NodeArgument> args;
			for (const auto& a : nodeParameters->args)
			{
				auto argument = std::static_pointer_cast<NodeArgument>(a);
				args.push_back(*argument);
			}

			symbolTable.back()[id] = SymbolInfo
			{
				0,
				functionDeclaration->lineNumber,
				functionType,
				SymbolInfo::SymbolType::Argument,
				std::vector<Value>(),
				false,
				true,
				args
			};
		}
		else if (functionNode->type == ASTNode::Type::Function)
		{
			auto function = std::static_pointer_cast<NodeFunction>(functionNode);

			// Function type
			DataType functionType = std::static_pointer_cast<NodeDataType>(function->datatype)->dataType;

			// Parameters
			auto nodeParameters = std::static_pointer_cast<NodeParamaters>(function->parameters);
			parseParameters(nodeParameters);

			// Statements
			parseStatements(function->statements, functionType);
		}
	}
	symbolTable.pop_back();
}
