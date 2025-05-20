#pragma once

#include <iostream>
#include <string>
#include <vector>

enum class TokenType {
    POLYNOM_LITERAL,
    IDENTIFIER,
    PLUS,
    MINUS,
    MULTIPLY,
    LEFT_PAREN,
    RIGHT_PAREN,
    EQUAL,
    UNARY_MINUS,
    END_OF_EXPRESSION,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
};

enum class LexerState {
    START,
    BUILDING_POLYNOM_LITERAL,
    BUILDING_IDENTIFIER
};

enum class SyntaxState {
    START,
    LBRACK_S,
    RBRACK_S,
    OPERATOR_S,
    UNARY_MINUS_S,
    POLYNOM_LITERAL_S,
    IDENTIFIER_S
};