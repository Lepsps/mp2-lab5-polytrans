#include "calculation.h"
#include <stdexcept>

Polynom Calculation::evaluate(const std::vector<Token>& rpn, std::map<std::string, Polynom>& variables) {
    std::stack<Polynom> values;
    auto rpn_iter = rpn.begin();

    for (; rpn_iter != rpn.end(); ++rpn_iter) {
        const auto& token = *rpn_iter;
        switch (token.type) {
        case TokenType::POLYNOM_LITERAL: {
            Polynom p_val;
            std::string val_str = token.value;
            p_val = val_str;
            values.push(p_val);
            break;
        }
        case TokenType::IDENTIFIER: {
            if (variables.count(token.value)) {
                values.push(variables[token.value]);
            }
            else {
                throw std::invalid_argument("Undefined variable: " + token.value);
            }
            break;
        }
        case TokenType::PLUS: {
            if (values.size() < 2) throw std::invalid_argument("Invalid expression: not enough operands for +");
            Polynom val2 = values.top(); values.pop();
            Polynom val1 = values.top(); values.pop();
            values.push(val1 + val2);
            break;
        }
        case TokenType::MINUS: {
            if (values.size() < 2) {
                throw std::invalid_argument("Invalid expression: not enough operands for -");
            }
            Polynom val2 = values.top(); values.pop();
            Polynom val1 = values.top(); values.pop();
            values.push(val1 - val2);
            break;
        }
        case TokenType::UNARY_MINUS: {
            if (values.size() < 1) throw std::invalid_argument("Invalid expression: not enough operands for unary -");
            Polynom val = values.top(); values.pop();
            Polynom minus_one_poly;
            minus_one_poly.addMonom(Monom(-1.0f));
            values.push(minus_one_poly * val);
            break;
        }
        case TokenType::MULTIPLY: {
            if (values.size() < 2) throw std::invalid_argument("Invalid expression: not enough operands for *");
            Polynom val2 = values.top(); values.pop();
            Polynom val1 = values.top(); values.pop();
            values.push(val1 * val2);
            break;
        }
        default:
            throw std::invalid_argument("Invalid token in RPN evaluation: " + token.value);
        }
    }
    if (values.size() != 1) {
        throw std::invalid_argument("Invalid expression: stack should have 1 value after RPN evaluation");
    }
    return values.top();
}