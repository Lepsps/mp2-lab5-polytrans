#include "lexical_analysis.h"
#include <cctype>

std::vector<Token> Lexical_analysis::lexus(const std::string& input) {
    if (input.empty()) {
        throw std::invalid_argument("SyntaxError: empty string");
    }

    LexerState state = LexerState::START;
    std::vector<Token> tokens;
    std::string current_token;

    for (size_t i = 0; i < input.length(); ++i) {
        char s = input[i];

        switch (state) {
        case LexerState::START:
            if (isspace(s)) {
                continue;
            }
            if (isdigit(s) || s == '.' || s == 'x' || s == 'y' || s == 'z') {
                current_token += s;
                state = LexerState::BUILDING_POLYNOM_LITERAL;
            }
            else if (s == '(') {
                tokens.push_back({ TokenType::LEFT_PAREN, "(" });
            }
            else if (s == ')') {
                tokens.push_back({ TokenType::RIGHT_PAREN, ")" });
            }
            else if (s == '*' || s == '+' || s == '=') {
                TokenType type;
                switch (s) {
                case '+': type = TokenType::PLUS; break;
                case '*': type = TokenType::MULTIPLY; break;
                case '=': type = TokenType::EQUAL; break;
                default:  type = TokenType::UNKNOWN; break;
                }
                tokens.push_back({ type, std::string(1, s) });
            }
            else if (isalpha(s) || s == '_') {
                current_token += s;
                state = LexerState::BUILDING_IDENTIFIER;
            }
            else if (s == '-') {
                if (tokens.empty() ||
                    tokens.back().type == TokenType::LEFT_PAREN ||
                    tokens.back().type == TokenType::PLUS ||
                    tokens.back().type == TokenType::MINUS ||
                    tokens.back().type == TokenType::MULTIPLY ||
                    tokens.back().type == TokenType::EQUAL) {
                    tokens.push_back({ TokenType::UNARY_MINUS, "-" });
                }
                else {
                    tokens.push_back({ TokenType::MINUS, "-" });
                }
            }
            else {
                current_token += s;
                tokens.push_back({ TokenType::UNKNOWN, current_token });
                current_token.clear();
            }
            break;

        case LexerState::BUILDING_POLYNOM_LITERAL:
            if (isdigit(s) || s == '.' || s == 'x' || s == 'y' || s == 'z' || s == '^') {
                current_token += s;
            }
            else {
                tokens.push_back({ TokenType::POLYNOM_LITERAL, current_token });
                current_token.clear();
                state = LexerState::START;
                --i;
            }
            break;

        case LexerState::BUILDING_IDENTIFIER:
            if (isalpha(s) || isdigit(s) || s == '_') {
                current_token += s;
            }
            else {
                if (current_token == "x" || current_token == "y" || current_token == "z") {
                    tokens.push_back({ TokenType::POLYNOM_LITERAL, current_token });
                }
                else {
                    tokens.push_back({ TokenType::IDENTIFIER, current_token });
                }
                current_token.clear();
                state = LexerState::START;
                --i;
            }
            break;
        }
    }

    if (!current_token.empty()) {
        if (state == LexerState::BUILDING_POLYNOM_LITERAL) {
            tokens.push_back({ TokenType::POLYNOM_LITERAL, current_token });
        }
        else if (state == LexerState::BUILDING_IDENTIFIER) {
            if (current_token == "x" || current_token == "y" || current_token == "z") {
                tokens.push_back({ TokenType::POLYNOM_LITERAL, current_token });
            }
            else {
                tokens.push_back({ TokenType::IDENTIFIER, current_token });
            }
        }
        else {
            tokens.push_back({ TokenType::UNKNOWN, current_token });
        }
    }

    tokens.push_back({ TokenType::END_OF_EXPRESSION, "" });
    return tokens;
}