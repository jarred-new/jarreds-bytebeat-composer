#include "stdafx.h"
#include "BytebeatLexer.h"

#include <ctype.h>
#include <stdlib.h>

CBytebeatLexer::CBytebeatLexer()
{
	m_pos = 0;

	m_line = 1;
	m_column = 1;

	m_hasError = false;

	m_errorLine = 1;
	m_errorColumn = 1;
	m_errorPosition = 0;
}

bool CBytebeatLexer::SetSource(
	const std::string& text)
{
	m_source = text;

	m_pos = 0;

	m_line = 1;
	m_column = 1;

	m_hasError = false;
	m_errorText.clear();

	return true;
}

bool CBytebeatLexer::HasError() const
{
	return m_hasError;
}

const std::string&
CBytebeatLexer::GetErrorText() const
{
	return m_errorText;
}

int CBytebeatLexer::GetErrorLine() const
{
	return m_errorLine;
}

int CBytebeatLexer::GetErrorColumn() const
{
	return m_errorColumn;
}

int CBytebeatLexer::GetErrorPosition() const
{
	return m_errorPosition;
}

void CBytebeatLexer::SetError(
	const char* text)
{
	if (m_hasError)
		return;

	m_hasError = true;

	m_errorText = text;

	m_errorLine = m_line;
	m_errorColumn = m_column;
	m_errorPosition =
		static_cast<int>(m_pos);
}

char CBytebeatLexer::Peek() const
{
	if (IsAtEnd())
		return '\0';

	return m_source[m_pos];
}

char CBytebeatLexer::PeekNext() const
{
	if (m_pos + 1 >= m_source.size())
		return '\0';

	return m_source[m_pos + 1];
}

char CBytebeatLexer::Advance()
{
	if (IsAtEnd())
		return '\0';

	char ch = m_source[m_pos++];

	if (ch == '\n')
	{
		++m_line;
		m_column = 1;
	}
	else
	{
		++m_column;
	}

	return ch;
}

bool CBytebeatLexer::IsAtEnd() const
{
	return m_pos >= m_source.size();
}

void CBytebeatLexer::SkipWhitespace()
{
	while (!IsAtEnd())
	{
		char c = Peek();

		if (c == ' ' ||
			c == '\t' ||
			c == '\r' ||
			c == '\n')
		{
			Advance();
			continue;
		}

		if (c == '/' &&
			PeekNext() == '/')
		{
			SkipLineComment();
			continue;
		}

		if (c == '/' &&
			PeekNext() == '*')
		{
			SkipBlockComment();
			continue;
		}

		break;
	}
}

void CBytebeatLexer::SkipLineComment()
{
	Advance();
	Advance();

	while (!IsAtEnd())
	{
		if (Peek() == '\n')
			break;

		Advance();
	}
}

void CBytebeatLexer::SkipBlockComment()
{
	Advance();
	Advance();

	while (!IsAtEnd())
	{
		if (Peek() == '*' &&
			PeekNext() == '/')
		{
			Advance();
			Advance();
			return;
		}

		Advance();
	}

	SetError(
		"Unterminated block comment");
}

bool CBytebeatLexer::IsIdentifierStart(
	char c)
{
	return isalpha(
		(unsigned char)c) ||
		c == '_';
}

bool CBytebeatLexer::IsIdentifierChar(
	char c)
{
	return isalnum(
		(unsigned char)c) ||
		c == '_';
}

BytebeatToken CBytebeatLexer::MakeToken(
	BytebeatTokenType type,
	const std::string& text)
{
	BytebeatToken t;

	t.type = type;
	t.text = text;

	t.line = m_line;
	t.column = m_column;
	t.position =
		static_cast<int>(m_pos);

	return t;
}

BytebeatToken
CBytebeatLexer::MakeNumberToken(
int64_t value,
const std::string& text)
{
	BytebeatToken t;

	t.type = TOK_NUMBER;

	t.text = text;
	t.value = value;

	t.line = m_line;
	t.column = m_column;
	t.position =
		static_cast<int>(m_pos);

	return t;
}

BytebeatToken
CBytebeatLexer::ReadIdentifier()
{
	size_t start = m_pos;

	while (IsIdentifierChar(Peek()))
	{
		Advance();
	}

	std::string text =
		m_source.substr(
		start,
		m_pos - start);

	if (text == "t")
	{
		return MakeToken(
			TOK_T,
			text);
	}

	BytebeatToken tok;

	tok.type = TOK_IDENTIFIER;
	tok.text = text;

	return tok;
}

BytebeatToken
CBytebeatLexer::ReadNumber()
{
	size_t start = m_pos;

	int64_t value = 0;

	if (Peek() == '0' &&
		(PeekNext() == 'x' ||
		PeekNext() == 'X'))
	{
		Advance();
		Advance();

		size_t hexStart = m_pos;

		while (isxdigit(
			(unsigned char)Peek()))
		{
			Advance();
		}

		std::string text =
			m_source.substr(
			start,
			m_pos - start);

		value =
			_strtoi64(
			text.c_str(),
			NULL,
			16);

		return MakeNumberToken(
			value,
			text);
	}

	if (Peek() == '0' &&
		(PeekNext() == 'b' ||
		PeekNext() == 'B'))
	{
		Advance();
		Advance();

		while (Peek() == '0' ||
			Peek() == '1')
		{
			value <<= 1;

			if (Peek() == '1')
				value |= 1;

			Advance();
		}

		std::string text =
			m_source.substr(
			start,
			m_pos - start);

		return MakeNumberToken(
			value,
			text);
	}

	if (Peek() == '0' &&
		(PeekNext() == 'o' ||
		PeekNext() == 'O'))
	{
		Advance();
		Advance();

		while (Peek() >= '0' &&
			Peek() <= '7')
		{
			value *= 8;
			value +=
				Peek() - '0';

			Advance();
		}

		std::string text =
			m_source.substr(
			start,
			m_pos - start);

		return MakeNumberToken(
			value,
			text);
	}

	while (isdigit(
		(unsigned char)Peek()))
	{
		value =
			value * 10 +
			(Peek() - '0');

		Advance();
	}

	std::string text =
		m_source.substr(
		start,
		m_pos - start);

	return MakeNumberToken(
		value,
		text);
}

BytebeatToken
CBytebeatLexer::NextToken()
{
	SkipWhitespace();

	if (m_hasError)
		return MakeToken(
		TOK_EOF,
		"");

	if (IsAtEnd())
		return MakeToken(
		TOK_EOF,
		"");

	char c = Peek();

	if (isdigit(
		(unsigned char)c))
	{
		return ReadNumber();
	}

	if (IsIdentifierStart(c))
	{
		return ReadIdentifier();
	}

	Advance();

	switch (c)
	{
	case '+':
		return MakeToken(TOK_PLUS, "+");

	case '-':
		return MakeToken(TOK_MINUS, "-");

	case '*':
		return MakeToken(TOK_STAR, "*");

	case '/':
		return MakeToken(TOK_SLASH, "/");

	case '%':
		return MakeToken(TOK_PERCENT, "%");

	case '(':
		return MakeToken(TOK_LPAREN, "(");

	case ')':
		return MakeToken(TOK_RPAREN, ")");

	case '?':
		return MakeToken(TOK_QUESTION, "?");

	case ':':
		return MakeToken(TOK_COLON, ":");

	case ',':
		return MakeToken(TOK_COMMA, ",");

	case '~':
		return MakeToken(TOK_BNOT, "~");

	case '^':
		return MakeToken(TOK_XOR, "^");
	}

	if (c == '&')
	{
		if (Peek() == '&')
		{
			Advance();
			return MakeToken(
				TOK_LAND,
				"&&");
		}

		return MakeToken(
			TOK_AND,
			"&");
	}

	if (c == '|')
	{
		if (Peek() == '|')
		{
			Advance();
			return MakeToken(
				TOK_LOR,
				"||");
		}

		return MakeToken(
			TOK_OR,
			"|");
	}

	if (c == '<')
	{
		if (Peek() == '<')
		{
			Advance();
			return MakeToken(
				TOK_SHL,
				"<<");
		}

		if (Peek() == '=')
		{
			Advance();
			return MakeToken(
				TOK_LE,
				"<=");
		}

		return MakeToken(
			TOK_LT,
			"<");
	}

	if (c == '>')
	{
		if (Peek() == '>')
		{
			Advance();
			return MakeToken(
				TOK_SHR,
				">>");
		}

		if (Peek() == '=')
		{
			Advance();
			return MakeToken(
				TOK_GE,
				">=");
		}

		return MakeToken(
			TOK_GT,
			">");
	}

	if (c == '=')
	{
		if (Peek() == '=')
		{
			Advance();
			return MakeToken(
				TOK_EQ,
				"==");
		}
	}

	if (c == '!')
	{
		if (Peek() == '=')
		{
			Advance();
			return MakeToken(
				TOK_NE,
				"!=");
		}

		return MakeToken(
			TOK_NOT,
			"!");
	}

	SetError(
		"Unexpected character");

	return MakeToken(
		TOK_EOF,
		"");
}