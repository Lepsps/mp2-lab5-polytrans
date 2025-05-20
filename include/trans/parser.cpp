#include "parser.h"

int Parser::get_priority(TokenType type) {
    switch (type) {
    case TokenType::UNARY_MINUS:
        return 3;
    case TokenType::MULTIPLY:
        return 2;
    case TokenType::PLUS:
    case TokenType::MINUS:
        return 1;
    default:
        return 0;
    }
}

std::vector<Token> Parser::transform(const std::vector<Token>& tokens) {
    std::vector<Token> rpn;
    std::stack<Token> operators;

    for (const auto& token : tokens) {
        switch (token.type) {
        case TokenType::POLYNOM_LITERAL:
        case TokenType::IDENTIFIER:
            rpn.push_back(token);
            break;
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::MULTIPLY:
            while (!operators.empty() && operators.top().type != TokenType::LEFT_PAREN &&
                get_priority(operators.top().type) >= get_priority(token.type)) {
                rpn.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
            break;
        case TokenType::UNARY_MINUS:
            operators.push(token);
            break;
        case TokenType::LEFT_PAREN:
            operators.push(token);
            break;
        case TokenType::RIGHT_PAREN:
            while (!operators.empty() && operators.top().type != TokenType::LEFT_PAREN) {
                rpn.push_back(operators.top());
                operators.pop();
            }
            if (!operators.empty() && operators.top().type == TokenType::LEFT_PAREN) {
                operators.pop();
            }
            else {
                throw std::invalid_argument("Mismatched parentheses");
            }
            break;
        case TokenType::END_OF_EXPRESSION:
            while (!operators.empty()) {
                if (operators.top().type == TokenType::LEFT_PAREN) {
                    throw std::invalid_argument("Mismatched parentheses");
                }
                rpn.push_back(operators.top());
                operators.pop();
            }
            break;
        case TokenType::EQUAL:
            operators.push(token);
            break;
        default:
            throw std::invalid_argument("Invalid token type in parser transform");
        }
    }
    return rpn;
}