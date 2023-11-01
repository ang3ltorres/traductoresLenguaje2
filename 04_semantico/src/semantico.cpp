#include "semantico.hpp"
#include <memory>
#include <iostream>
#include <format>

static std::vector<std::unordered_map<std::string, SymbolInfo>> symbolTable;

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
	if (symbolTable.back().find(id) == symbolTable.back().end())
	{
		symbolTable.back()[id] = SymbolInfo
		{
			static_cast<unsigned int>(symbolTable.size() - 1),
			node->lineNumber,
			type,
			(declaration) ? (SymbolInfo::SymbolType::Declaration) : (SymbolInfo::SymbolType::DeclarationInnitialization)
		};
	}
	else
		throw ErrorCode(node->lineNumber, std::format("Redefinicion de la variable \"{}\"", id));
}

void semanticAnalysis(const std::shared_ptr<NodeProgram>& program)
{
	// Tablas de simbolos, el index representa el scopeLevel - 1
	symbolTable.push_back(std::unordered_map<std::string, SymbolInfo>());

	for (const std::shared_ptr<NodeFunction>& function : program->functions)
	{

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
				symbolTable.back()[id->name] = SymbolInfo
				{
					0,
					id->lineNumber,
					type,
					SymbolInfo::SymbolType::Argument
				};
			}
			else
				throw ErrorCode(id->lineNumber, std::format("Redefinicion del parametro \"{}\"", id->name));
		}

		// Statements
		symbolTable.push_back(std::unordered_map<std::string, SymbolInfo>());

		for (const auto& i : function->statements)
		{
			// Un-wrap statement
			auto statement = std::static_pointer_cast<NodeStatement>(i);

			if (statement->statement->type == ASTNode::Type::Declaration)
			{
				auto declaration = std::static_pointer_cast<NodeDeclaration>(statement->statement);
				parseDeclaration(declaration);
			}
		}

		symbolTable.pop_back();
	}

}