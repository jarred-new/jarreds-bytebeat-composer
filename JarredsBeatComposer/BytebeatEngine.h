#pragma once

#include <stdint.h>
#include <string>
#include <vector>

#include "BytebeatToken.h"
#include "BytebeatLexer.h"

//////////////////////////////////////////////////////////////////////////
// Parse Error
//////////////////////////////////////////////////////////////////////////

struct BytebeatParseError
{
	std::string message;

	int line;
	int column;
	int position;

	BytebeatParseError()
		: line(1)
		, column(1)
		, position(0)
	{
	}
};

//////////////////////////////////////////////////////////////////////////
// Runtime Error
//////////////////////////////////////////////////////////////////////////

struct BytebeatRuntimeError
{
	std::string message;

	uint32_t sampleTime;

	BytebeatRuntimeError()
		: sampleTime(0)
	{
	}
};

//////////////////////////////////////////////////////////////////////////
// AST
//////////////////////////////////////////////////////////////////////////

enum BytebeatNodeType
{
	NODE_NUMBER = 0,

	NODE_T,

	NODE_UNARY,

	NODE_BINARY,

	NODE_TERNARY
};

//////////////////////////////////////////////////////////////////////////
// Operators
//////////////////////////////////////////////////////////////////////////

enum BytebeatOperator
{
	OP_NONE = 0,

	OP_ADD,
	OP_SUB,

	OP_MUL,
	OP_DIV,
	OP_MOD,

	OP_SHL,
	OP_SHR,

	OP_LT,
	OP_LE,
	OP_GT,
	OP_GE,

	OP_EQ,
	OP_NE,

	OP_AND,
	OP_OR,
	OP_XOR,

	OP_LAND,
	OP_LOR,

	OP_POSITIVE,
	OP_NEGATIVE,

	OP_NOT,
	OP_BNOT
};

//////////////////////////////////////////////////////////////////////////
// AST Node
//////////////////////////////////////////////////////////////////////////

struct BytebeatNode
{
	BytebeatNodeType type;

	BytebeatOperator op;

	int64_t value;

	BytebeatNode* left;
	BytebeatNode* right;

	BytebeatNode* condition;
	BytebeatNode* trueBranch;
	BytebeatNode* falseBranch;

	BytebeatNode()
		: type(NODE_NUMBER)
		, op(OP_NONE)
		, value(0)
		, left(NULL)
		, right(NULL)
		, condition(NULL)
		, trueBranch(NULL)
		, falseBranch(NULL)
	{
	}
};

//////////////////////////////////////////////////////////////////////////
// Engine
//////////////////////////////////////////////////////////////////////////

class CBytebeatEngine
{
public:

	CBytebeatEngine();

	~CBytebeatEngine();

public:

	bool SetFormula(
		const std::string& formula);

	bool Evaluate(
		uint32_t t,
		int64_t& result);

	bool Sample(
		uint32_t t,
		uint8_t& sample);

public:

	bool HasParseError() const;

	const BytebeatParseError&
		GetParseError() const;

	bool HasRuntimeError() const;

	const BytebeatRuntimeError&
		GetRuntimeError() const;

public:

	void Clear();

private:

	CBytebeatLexer m_lexer;

	BytebeatToken m_current;

	BytebeatNode* m_root;

	std::vector<BytebeatNode*> m_nodes;

private:

	bool m_hasParseError;

	BytebeatParseError m_parseError;

private:

	bool m_hasRuntimeError;

	BytebeatRuntimeError m_runtimeError;

	//////////////////////////////////////////////////////////////////////////
	// Parser
	//////////////////////////////////////////////////////////////////////////

private:

	void NextToken();

	bool Match(
		BytebeatTokenType type);

	bool Expect(
		BytebeatTokenType type,
		const char* error);

	void SetParseError(
		const char* error);

	//////////////////////////////////////////////////////////////////////////
	// Node Creation
	//////////////////////////////////////////////////////////////////////////

private:

	BytebeatNode* CreateNode(
		BytebeatNodeType type);

	BytebeatNode* CreateNumberNode(
		int64_t value);

	BytebeatNode* CreateVariableNode();

	BytebeatNode* CreateUnaryNode(
		BytebeatOperator op,
		BytebeatNode* expr);

	BytebeatNode* CreateBinaryNode(
		BytebeatOperator op,
		BytebeatNode* left,
		BytebeatNode* right);

	BytebeatNode* CreateTernaryNode(
		BytebeatNode* cond,
		BytebeatNode* yes,
		BytebeatNode* no);

	//////////////////////////////////////////////////////////////////////////
	// Grammar
	//////////////////////////////////////////////////////////////////////////

private:

	BytebeatNode* ParseExpression();

	BytebeatNode* ParseTernary();

	BytebeatNode* ParseLogicalOr();

	BytebeatNode* ParseLogicalAnd();

	BytebeatNode* ParseBitwiseOr();

	BytebeatNode* ParseBitwiseXor();

	BytebeatNode* ParseBitwiseAnd();

	BytebeatNode* ParseEquality();

	BytebeatNode* ParseRelational();

	BytebeatNode* ParseShift();

	BytebeatNode* ParseAddSub();

	BytebeatNode* ParseMulDiv();

	BytebeatNode* ParseUnary();

	BytebeatNode* ParsePrimary();

	//////////////////////////////////////////////////////////////////////////
	// Evaluation
	//////////////////////////////////////////////////////////////////////////

private:

	bool EvaluateNode(
		BytebeatNode* node,
		uint32_t t,
		int64_t& result);

	void SetRuntimeError(
		const char* text,
		uint32_t sampleTime);

	//////////////////////////////////////////////////////////////////////////
	// Cleanup
	//////////////////////////////////////////////////////////////////////////

private:
	void FreeNodes();
};