#include "stdafx.h"
#include "BytebeatEngine.h"

#include <stdlib.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////
// Constructor / Destructor
//////////////////////////////////////////////////////////////////////////

CBytebeatEngine::CBytebeatEngine()
{
	m_root = NULL;

	m_hasParseError = false;
	m_hasRuntimeError = false;

	m_current.type = TOK_EOF;
}

CBytebeatEngine::~CBytebeatEngine()
{
	Clear();
}

//////////////////////////////////////////////////////////////////////////
// Clear / Free
//////////////////////////////////////////////////////////////////////////

void CBytebeatEngine::Clear()
{
	FreeNodes();

	m_root = NULL;

	m_hasParseError = false;
	m_hasRuntimeError = false;

	m_parseError = BytebeatParseError();
	m_runtimeError = BytebeatRuntimeError();
}

void CBytebeatEngine::FreeNodes()
{
	for (size_t i = 0; i < m_nodes.size(); ++i)
	{
		delete m_nodes[i];
	}

	m_nodes.clear();
}

//////////////////////////////////////////////////////////////////////////
// Token Control
//////////////////////////////////////////////////////////////////////////

void CBytebeatEngine::NextToken()
{
	m_current = m_lexer.NextToken();
}

bool CBytebeatEngine::Match(BytebeatTokenType type)
{
	if (m_current.type == type)
	{
		NextToken();
		return true;
	}

	return false;
}

bool CBytebeatEngine::Expect(BytebeatTokenType type, const char* error)
{
	if (m_current.type == type)
	{
		NextToken();
		return true;
	}

	SetParseError(error);
	return false;
}

//////////////////////////////////////////////////////////////////////////
// Error Handling
//////////////////////////////////////////////////////////////////////////

void CBytebeatEngine::SetParseError(const char* error)
{
	if (m_hasParseError)
		return;

	m_hasParseError = true;

	m_parseError.message = error;

	m_parseError.line =
		m_current.line;

	m_parseError.column =
		m_current.column;

	m_parseError.position =
		m_current.position;
}

// SetRuntimeError() Alternative
//void CBytebeatEngine::SetRuntimeError(
//	const char* text,
//	uint32_t sampleTime)
//{
//	if (m_hasRuntimeError)
//		return;
//
//	m_hasRuntimeError = true;
//
//	m_runtimeError.message = text;
//
//	m_runtimeError.sampleTime = sampleTime;
//}

//////////////////////////////////////////////////////////////////////////
// Node Creation
//////////////////////////////////////////////////////////////////////////

bool CBytebeatEngine::Evaluate(
	uint32_t t,
	int64_t& result)
{
	if (!m_root)
	{
		SetRuntimeError("No expression loaded", t);
		return false;
	}

	m_hasRuntimeError = false;

	return EvaluateNode(m_root, t, result);
}

BytebeatNode* CBytebeatEngine::CreateNode(
	BytebeatNodeType type)
{
	BytebeatNode* n = new BytebeatNode();
	n->type = type;

	m_nodes.push_back(n);

	return n;
}

BytebeatNode* CBytebeatEngine::CreateNumberNode(
	int64_t value)
{
	BytebeatNode* n =
		CreateNode(NODE_NUMBER);

	n->value = value;

	return n;
}

BytebeatNode* CBytebeatEngine::CreateVariableNode()
{
	BytebeatNode* n =
		CreateNode(NODE_T);

	return n;
}

BytebeatNode* CBytebeatEngine::CreateUnaryNode(
	BytebeatOperator op,
	BytebeatNode* expr)
{
	BytebeatNode* n =
		CreateNode(NODE_UNARY);

	n->op = op;
	n->left = expr;

	return n;
}

BytebeatNode* CBytebeatEngine::CreateBinaryNode(
	BytebeatOperator op,
	BytebeatNode* left,
	BytebeatNode* right)
{
	BytebeatNode* n =
		CreateNode(NODE_BINARY);

	n->op = op;
	n->left = left;
	n->right = right;

	return n;
}

BytebeatNode* CBytebeatEngine::CreateTernaryNode(
	BytebeatNode* cond,
	BytebeatNode* yes,
	BytebeatNode* no)
{
	BytebeatNode* n =
		CreateNode(NODE_TERNARY);

	n->condition = cond;
	n->trueBranch = yes;
	n->falseBranch = no;

	return n;
}

bool CBytebeatEngine::SetFormula(
	const std::string& formula)
{
	Clear();

	if (formula.empty())
	{
		SetParseError("Empty formula");
		return false;
	}

	m_lexer.SetSource(formula);

	NextToken();

	m_root = ParseExpression();

	if (m_hasParseError)
		return false;

	if (m_current.type != TOK_EOF)
	{
		SetParseError("Unexpected trailing input");
		return false;
	}

	return true;
}

BytebeatNode* CBytebeatEngine::ParseExpression()
{
	return ParseTernary();
}

BytebeatNode* CBytebeatEngine::ParseTernary()
{
	BytebeatNode* cond =
		ParseLogicalOr();

	if (m_hasParseError)
		return NULL;

	if (Match(TOK_QUESTION))
	{
		BytebeatNode* yes =
			ParseExpression();

		if (!Expect(TOK_COLON, "Expected ':' in ternary"))
			return NULL;

		BytebeatNode* no =
			ParseTernary();

		return CreateTernaryNode(cond, yes, no);
	}

	return cond;
}

BytebeatNode* CBytebeatEngine::ParseLogicalOr()
{
	BytebeatNode* node =
		ParseLogicalAnd();

	while (Match(TOK_LOR))
	{
		BytebeatNode* rhs =
			ParseLogicalAnd();

		node =
			CreateBinaryNode(OP_LOR, node, rhs);
	}

	return node;
}

BytebeatNode* CBytebeatEngine::ParseLogicalAnd()
{
	BytebeatNode* node =
		ParseBitwiseOr();

	while (Match(TOK_LAND))
	{
		BytebeatNode* rhs =
			ParseBitwiseOr();

		node =
			CreateBinaryNode(OP_LAND, node, rhs);
	}

	return node;
}

BytebeatNode* CBytebeatEngine::ParseBitwiseOr()
{
	BytebeatNode* node =
		ParseBitwiseXor();

	while (Match(TOK_OR))
	{
		BytebeatNode* rhs =
			ParseBitwiseXor();

		node =
			CreateBinaryNode(OP_OR, node, rhs);
	}

	return node;
}

BytebeatNode* CBytebeatEngine::ParseBitwiseXor()
{
	BytebeatNode* node =
		ParseBitwiseAnd();

	while (Match(TOK_XOR))
	{
		BytebeatNode* rhs =
			ParseBitwiseAnd();

		node =
			CreateBinaryNode(OP_XOR, node, rhs);
	}

	return node;
}

BytebeatNode* CBytebeatEngine::ParseBitwiseAnd()
{
	BytebeatNode* node =
		ParseEquality();

	while (Match(TOK_AND))
	{
		BytebeatNode* rhs =
			ParseEquality();

		node =
			CreateBinaryNode(OP_AND, node, rhs);
	}

	return node;
}

BytebeatNode* CBytebeatEngine::ParseEquality()
{
	BytebeatNode* node =
		ParseRelational();

	while (true)
	{
		if (Match(TOK_EQ))
		{
			BytebeatNode* rhs =
				ParseRelational();

			node =
				CreateBinaryNode(OP_EQ, node, rhs);
		}
		else
		if (Match(TOK_NE))
		{
			BytebeatNode* rhs =
				ParseRelational();

			node =
				CreateBinaryNode(OP_NE, node, rhs);
		}
		else
		{
			break;
		}
	}

	return node;
}

BytebeatNode* CBytebeatEngine::ParseRelational()
{
	BytebeatNode* node =
		ParseShift();

	while (true)
	{
		if (Match(TOK_LT))
		{
			BytebeatNode* rhs =
				ParseShift();

			node =
				CreateBinaryNode(OP_LT, node, rhs);
		}
		else
		if (Match(TOK_LE))
		{
			BytebeatNode* rhs =
				ParseShift();

			node =
				CreateBinaryNode(OP_LE, node, rhs);
		}
		else
		if (Match(TOK_GT))
		{
			BytebeatNode* rhs =
				ParseShift();

			node =
				CreateBinaryNode(OP_GT, node, rhs);
		}
		else
		if (Match(TOK_GE))
		{
			BytebeatNode* rhs =
				ParseShift();

			node =
				CreateBinaryNode(OP_GE, node, rhs);
		}
		else
		{
			break;
		}
	}

	return node;
}

BytebeatNode* CBytebeatEngine::ParseShift()
{
	BytebeatNode* node =
		ParseAddSub();

	while (true)
	{
		if (Match(TOK_SHL))
		{
			BytebeatNode* rhs =
				ParseAddSub();

			node =
				CreateBinaryNode(OP_SHL, node, rhs);
		}
		else
		if (Match(TOK_SHR))
		{
			BytebeatNode* rhs =
				ParseAddSub();

			node =
				CreateBinaryNode(OP_SHR, node, rhs);
		}
		else
		{
			break;
		}
	}

	return node;
}

BytebeatNode* CBytebeatEngine::ParseAddSub()
{
	BytebeatNode* node =
		ParseMulDiv();

	while (true)
	{
		if (Match(TOK_PLUS))
		{
			BytebeatNode* rhs =
				ParseMulDiv();

			node =
				CreateBinaryNode(OP_ADD, node, rhs);
		}
		else
		if (Match(TOK_MINUS))
		{
			BytebeatNode* rhs =
				ParseMulDiv();

			node =
				CreateBinaryNode(OP_SUB, node, rhs);
		}
		else
		{
			break;
		}
	}

	return node;
}

BytebeatNode* CBytebeatEngine::ParseMulDiv()
{
	BytebeatNode* node =
		ParseUnary();

	while (true)
	{
		if (Match(TOK_STAR))
		{
			BytebeatNode* rhs =
				ParseUnary();

			node =
				CreateBinaryNode(OP_MUL, node, rhs);
		}
		else
		if (Match(TOK_SLASH))
		{
			BytebeatNode* rhs =
				ParseUnary();

			node =
				CreateBinaryNode(OP_DIV, node, rhs);
		}
		else
		if (Match(TOK_PERCENT))
		{
			BytebeatNode* rhs =
				ParseUnary();

			node =
				CreateBinaryNode(OP_MOD, node, rhs);
		}
		else
		{
			break;
		}
	}

	return node;
}

BytebeatNode* CBytebeatEngine::ParseUnary()
{
	if (Match(TOK_PLUS))
	{
		BytebeatNode* expr =
			ParseUnary();

		return CreateUnaryNode(OP_POSITIVE, expr);
	}

	if (Match(TOK_MINUS))
	{
		BytebeatNode* expr =
			ParseUnary();

		return CreateUnaryNode(OP_NEGATIVE, expr);
	}

	if (Match(TOK_NOT))
	{
		BytebeatNode* expr =
			ParseUnary();

		return CreateUnaryNode(OP_NOT, expr);
	}

	if (Match(TOK_BNOT))
	{
		BytebeatNode* expr =
			ParseUnary();

		return CreateUnaryNode(OP_BNOT, expr);
	}

	return ParsePrimary();
}

BytebeatNode* CBytebeatEngine::ParsePrimary()
{
	if (m_current.type == TOK_NUMBER)
	{
		int64_t v = m_current.value;
		NextToken();
		return CreateNumberNode(v);
	}

	if (m_current.type == TOK_T)
	{
		NextToken();
		return CreateVariableNode();
	}

	if (Match(TOK_LPAREN))
	{
		BytebeatNode* expr =
			ParseExpression();

		if (!Expect(TOK_RPAREN, "Expected ')'"))
			return NULL;

		return expr;
	}

	SetParseError("Unexpected token in expression");
	return NULL;
}

bool CBytebeatEngine::Sample(
	uint32_t t,
	uint8_t& sample)
{
	int64_t result = 0;

	if (!Evaluate(t, result))
	{
		sample = 0;
		return false;
	}

	// Normalize to 8-bit bytebeat output
	sample = static_cast<uint8_t>(result & 0xFF);

	return true;
}

bool CBytebeatEngine::SampleSigned(
	uint32_t t,
	int8_t& sample)
{
	int64_t result = 0;
	if (!Evaluate(t, result))
	{
		sample = 0;
		return false;
	}
	// Normalize to signed 8-bit bytebeat output (map 0..255 -> -128..127)
	uint8_t u = static_cast<uint8_t>(result & 0xFF);
	sample = static_cast<int8_t>(static_cast<int16_t>(u) - 128);

	return true;
}

bool CBytebeatEngine::EvaluateNode(
	BytebeatNode* node,
	uint32_t t,
	int64_t& result)
{
	if (!node)
	{
		SetRuntimeError("Null node", t);
		return false;
	}

	switch (node->type)
	{
	case NODE_NUMBER:
		result = node->value;
		return true;

	case NODE_T:
		result = (int64_t)t;
		return true;

	case NODE_UNARY:
	{
					   int64_t v = 0;

					   if (!EvaluateNode(node->left, t, v))
						   return false;

					   switch (node->op)
					   {
					   case OP_POSITIVE:
						   result = +v;
						   return true;

					   case OP_NEGATIVE:
						   result = -v;
						   return true;

					   case OP_NOT:
						   result = (v == 0);
						   return true;

					   case OP_BNOT:
						   result = ~v;
						   return true;

					   default:
						   SetRuntimeError("Invalid unary operator", t);
						   return false;
					   }
	}

	case NODE_BINARY:
	{
						int64_t l = 0, r = 0;

						if (!EvaluateNode(node->left, t, l))
							return false;

						if (!EvaluateNode(node->right, t, r))
							return false;

						switch (node->op)
						{
						case OP_ADD:
							result = l + r;
							return true;

						case OP_SUB:
							result = l - r;
							return true;

						case OP_MUL:
							result = l * r;
							return true;

						case OP_DIV:
							if (r == 0)
							{
								SetRuntimeError("Division by zero", t);
								return false;
							}
							result = l / r;
							return true;

						case OP_MOD:
							if (r == 0)
							{
								SetRuntimeError("Modulo by zero", t);
								return false;
							}
							result = l % r;
							return true;

						case OP_SHL:
							result = l << (r & 31);
							return true;

						case OP_SHR:
							result = l >> (r & 31);
							return true;

						case OP_AND:
							result = l & r;
							return true;

						case OP_OR:
							result = l | r;
							return true;

						case OP_XOR:
							result = l ^ r;
							return true;

						case OP_LAND:
							result = (l && r);
							return true;

						case OP_LOR:
							result = (l || r);
							return true;

						case OP_LT:
							result = (l < r);
							return true;

						case OP_LE:
							result = (l <= r);
							return true;

						case OP_GT:
							result = (l > r);
							return true;

						case OP_GE:
							result = (l >= r);
							return true;

						case OP_EQ:
							result = (l == r);
							return true;

						case OP_NE:
							result = (l != r);
							return true;

						default:
							SetRuntimeError("Invalid binary operator", t);
							return false;
						}
	}

	case NODE_TERNARY:
	{
						 int64_t cond = 0;

						 if (!EvaluateNode(node->condition, t, cond))
							 return false;

						 if (cond)
							 return EvaluateNode(node->trueBranch, t, result);

						 return EvaluateNode(node->falseBranch, t, result);
	}

	default:
		SetRuntimeError("Unknown node type", t);
		return false;
	}
}

void CBytebeatEngine::SetRuntimeError(
	const char* text,
	uint32_t sampleTime)
{
	if (m_hasRuntimeError)
		return;

	m_hasRuntimeError = true;

	m_runtimeError.message = text;
	m_runtimeError.sampleTime = sampleTime;
}

bool CBytebeatEngine::HasRuntimeError() const
{
	return m_hasRuntimeError;
}

const BytebeatRuntimeError&
CBytebeatEngine::GetRuntimeError() const
{
	return m_runtimeError;
}

bool CBytebeatEngine::HasParseError() const
{
	return m_hasParseError;
}

const BytebeatParseError&
CBytebeatEngine::GetParseError() const
{
	return m_parseError;
}