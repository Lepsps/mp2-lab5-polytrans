#include "syntax_analysis.h"
#include <stdexcept>

void Syntax_analysis::analyze(const std::vector<Token>& tokens) {
    SyntaxState state = SyntaxState::START;
    int paren_count = 0;

    if (tokens.empty() || tokens.front().type == TokenType::END_OF_EXPRESSION) {
        if (tokens.empty() || (tokens.size() == 1 && tokens.front().type == TokenType::END_OF_EXPRESSION))
            throw std::invalid_argument("SyntaxError: empty expression for syntax analysis");
    }


    for (size_t i = 0; i < tokens.size(); ++i) {
        const Token& token = tokens[i];
        if (token.type == TokenType::LEFT_PAREN) paren_count++;
        else if (token.type == TokenType::RIGHT_PAREN) paren_count--;

        if (paren_count < 0) throw std::invalid_argument("SyntaxError: Mismatched parentheses, unexpected ')'");

        switch (state) {
        case SyntaxState::START:
            if (token.type == TokenType::POLYNOM_LITERAL) {
                state = SyntaxState::POLYNOM_LITERAL_S;
            }
            else if (token.type == TokenType::IDENTIFIER) {
                state = SyntaxState::IDENTIFIER_S;
            }
            else if (token.type == TokenType::LEFT_PAREN) {
                state = SyntaxState::LBRACK_S;
            }
            else if (token.type == TokenType::UNARY_MINUS) {
                state = SyntaxState::UNARY_MINUS_S;
            }
            else if (token.type == TokenType::END_OF_EXPRESSION && i == 0 && tokens.size() > 1) {
                throw std::invalid_argument("SyntaxError: expression starts with END_OF_EXPRESSION token improperly");
            }
            else if (token.type == TokenType::END_OF_EXPRESSION) {

            }
            else {
                throw std::invalid_argument("SyntaxError: invalid syntax at start, token: " + token.value);
            }
            break;

        case SyntaxState::POLYNOM_LITERAL_S:
            if (token.type == TokenType::PLUS ||
                token.type == TokenType::MINUS ||
                token.type == TokenType::MULTIPLY) {
                state = SyntaxState::OPERATOR_S;
            }
            else if (token.type == TokenType::RIGHT_PAREN) {
                state = SyntaxState::RBRACK_S;
            }
            else if (token.type == TokenType::END_OF_EXPRESSION) {

            }
            else {
                throw std::invalid_argument("SyntaxError: invalid syntax after polynom literal, token: " + token.value);
            }
            break;

        case SyntaxState::IDENTIFIER_S:
            if (token.type == TokenType::PLUS ||
                token.type == TokenType::MINUS ||
                token.type == TokenType::MULTIPLY) {
                state = SyntaxState::OPERATOR_S;
            }
            else if (token.type == TokenType::RIGHT_PAREN) {
                state = SyntaxState::RBRACK_S;
            }
            else if (token.type == TokenType::END_OF_EXPRESSION) {

            }
            else {
                throw std::invalid_argument("SyntaxError: invalid syntax after identifier, token: " + token.value);
            }
            break;

        case SyntaxState::UNARY_MINUS_S:
            if (token.type == TokenType::POLYNOM_LITERAL) {
                state = SyntaxState::POLYNOM_LITERAL_S;
            }
            else if (token.type == TokenType::LEFT_PAREN) {
                state = SyntaxState::LBRACK_S;
            }
            else if (token.type == TokenType::IDENTIFIER) {
                state = SyntaxState::IDENTIFIER_S;
            }
            else {
                throw std::invalid_argument("SyntaxError: invalid syntax after unary -, token: " + token.value);
            }
            break;

        case SyntaxState::LBRACK_S:
            if (token.type == TokenType::POLYNOM_LITERAL) {
                state = SyntaxState::POLYNOM_LITERAL_S;
            }
            else if (token.type == TokenType::IDENTIFIER) {
                state = SyntaxState::IDENTIFIER_S;
            }
            else if (token.type == TokenType::LEFT_PAREN) {
                state = SyntaxState::LBRACK_S;
            }
            else if (token.type == TokenType::UNARY_MINUS) {
                state = SyntaxState::UNARY_MINUS_S;
            }
            else if (token.type == TokenType::RIGHT_PAREN) {
                throw std::invalid_argument("SyntaxError: empty parentheses ()");
            }
            else {
                throw std::invalid_argument("SyntaxError: invalid syntax after '(', token: " + token.value);
            }
            break;

        case SyntaxState::RBRACK_S:
            if (token.type == TokenType::PLUS ||
                token.type == TokenType::MINUS ||
                token.type == TokenType::MULTIPLY) {
                state = SyntaxState::OPERATOR_S;
            }
            else if (token.type == TokenType::RIGHT_PAREN) {
                state = SyntaxState::RBRACK_S;
            }
            else if (token.type == TokenType::END_OF_EXPRESSION) {

            }
            else {
                throw std::invalid_argument("SyntaxError: invalid syntax after ')', token: " + token.value);
            }
            break;

        case SyntaxState::OPERATOR_S:
            if (token.type == TokenType::POLYNOM_LITERAL) {
                state = SyntaxState::POLYNOM_LITERAL_S;
            }
            else if (token.type == TokenType::IDENTIFIER) {
                state = SyntaxState::IDENTIFIER_S;
            }
            else if (token.type == TokenType::LEFT_PAREN) {
                state = SyntaxState::LBRACK_S;
            }
            else if (token.type == TokenType::UNARY_MINUS) {
                state = SyntaxState::UNARY_MINUS_S;
            }
            else {
                throw std::invalid_argument("SyntaxError: invalid syntax after operator, token: " + token.value);
            }
            break;
        }
    }

    if (paren_count != 0) {
        throw std::invalid_argument("SyntaxError: Mismatched parentheses, count difference: " + std::to_string(paren_count));
    }

    if (tokens.size() > 1) { // END_OF_EXPRESSION is always last
        const Token& last_meaningful_token = tokens[tokens.size() - 2];
        if (last_meaningful_token.type == TokenType::PLUS ||
            last_meaningful_token.type == TokenType::MINUS ||
            last_meaningful_token.type == TokenType::MULTIPLY ||
            last_meaningful_token.type == TokenType::UNARY_MINUS ||
            last_meaningful_token.type == TokenType::LEFT_PAREN) {
            throw std::invalid_argument("SyntaxError: expression ends with an operator or '('");
        }
    }
    else if (tokens.size() == 1 && tokens[0].type != TokenType::END_OF_EXPRESSION) {
        throw std::invalid_argument("SyntaxError: single token expression not ending with END_OF_EXPRESSION correctly");
    }


}