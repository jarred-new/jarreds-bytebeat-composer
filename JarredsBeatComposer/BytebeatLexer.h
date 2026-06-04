#pragma once

#include <string>
#include <stdint.h>

#include "BytebeatToken.h"

class CBytebeatLexer
{
public:

	CBytebeatLexer();

	bool SetSource(const std::string& text);

	BytebeatToken NextToken();

	bool HasError() const;
	const std::string& GetErrorText() const;

	int GetErrorLine() const;
	int GetErrorColumn() const;
	int GetErrorPosition() const;

private:

	std::string m_source;

	size_t m_pos;

	int m_line;
	int m_column;

	bool m_hasError;

	std::string m_errorText;

	int m_errorLine;
	int m_errorColumn;
	int m_errorPosition;

private:

	void SetError(const char* text);

	char Peek() const;
	char PeekNext() const;

	char Advance();

	bool IsAtEnd() const;

	void SkipWhitespace();

	void SkipLineComment();

	void SkipBlockComment();

	BytebeatToken ReadNumber();

	BytebeatToken ReadIdentifier();

	BytebeatToken MakeToken(
		BytebeatTokenType type,
		const std::string& text);

	BytebeatToken MakeNumberToken(
		int64_t value,
		const std::string& text);

	static bool IsIdentifierStart(char c);

	static bool IsIdentifierChar(char c);
};