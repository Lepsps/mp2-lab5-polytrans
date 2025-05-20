#pragma once
#include "tokens.h"
#include "polynoms.h"

#include <stack>
#include <string>
#include <stdexcept>    
#include <optional>      
#include <type_traits>    

#include "address_hash.h"
#include "chain_hash.h"
#include "unordered.h"
#include "ordered.h"
#include "bs_tree.h"   
#include "rb_tree.h"
#include "avl_tree.h"
#include <map>


class Сalculation {
public:
    template <typename StorageType>
    Polynom evaluate(const std::vector<Token>& rpn, StorageType& storage) {
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
                std::optional<Polynom> varValue;

                if constexpr (std::is_same_v<StorageType, AddressHashTable<std::string, Polynom>>) {
                    auto it = storage.find(token.value);
                    if (it != storage.end()) {   
                        varValue = it->value;
                    }
                }
                else if constexpr (std::is_same_v<StorageType, ChainHash<std::string, Polynom>>) {
                    auto it = storage.find(token.value);
                    if (it != storage.end()) {   
                        varValue = it->second;     
                    }
                }
                else if constexpr (std::is_base_of_v<BSTree<std::string, Polynom>, StorageType> ||
                    std::is_same_v<StorageType, BSTree<std::string, Polynom>>) {
                    auto* node = storage.find(token.value);      
                    if (node) {
                        varValue = node->value;
                    }
                }
                else if constexpr (std::is_same_v<StorageType, OrderedTable<std::string, Polynom>> ||
                    std::is_same_v<StorageType, UnOrderedTable<std::string, Polynom>>) {
                    auto it = storage.find(token.value);
                    if (it != storage.end()) {    
                        varValue = it.value();     
                    }
                }
                else if constexpr (std::is_same_v<StorageType, std::map<std::string, Polynom>>) {
                    auto it = storage.find(token.value);
                    if (it != storage.end()) {
                        varValue = it->second;
                    }
                }
                else {
                    throw std::runtime_error("Unsupported storage type for variable lookup in Calculation class.");
                }

                if (varValue.has_value()) {
                    values.push(varValue.value());
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
                std::string m_one_str = "-1";     
                minus_one_poly = m_one_str;          
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
            std::string error_msg = "Invalid expression: stack has " + std::to_string(values.size()) + " values after RPN evaluation, expected 1.";
            throw std::invalid_argument(error_msg);
        }
        return values.top();
    }
};