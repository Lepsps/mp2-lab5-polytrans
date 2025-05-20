#pragma once

#include "tokens.h"
#include <stack>

class Parser {
private:
	int get_priority(TokenType type);
public:
	std::vector<Token> transform(const std::vector<Token>& tokens);
};