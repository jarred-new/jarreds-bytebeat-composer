#pragma once

#include <string>
#include <stdint.h>

enum BytebeatTokenType
{
	TOK_EOF = 0,

	TOK_NUMBER,
	TOK_IDENTIFIER,

	TOK_T,

	TOK_PLUS,
	TOK_MINUS,
	TOK_STAR,
	TOK_SLASH,
	TOK_PERCENT,

	TOK_LPAREN,
	TOK_RPAREN,

	TOK_QUESTION,
	TOK_COLON,

	TOK_COMMA,

	TOK_NOT,
	TOK_BNOT,

	TOK_AND,
	TOK_OR,
	TOK_XOR,

	TOK_LAND,
	TOK_LOR,

	TOK_SHL,
	TOK_SHR,

	TOK_LT,
	TOK_LE,
	TOK_GT,
	TOK_GE,

	TOK_EQ,
	TOK_NE
};

struct BytebeatToken
{
	BytebeatTokenType type;

	std::string text;

	int64_t value;

	int line;
	int column;
	int position;

	BytebeatToken()
		: type(TOK_EOF)
		, value(0)
		, line(1)
		, column(1)
		, position(0)
	{
	}
};