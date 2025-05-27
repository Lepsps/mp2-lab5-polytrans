#pragma once

#include "lexical_analysis.h"
#include "syntax_analysis.h"
#include "parser.h"
#include "calculation.h"   
#include "polynoms.h"

#include "address_hash.h"
#include "chain_hash.h"
#include "unordered.h"
#include "ordered.h"
#include "bs_tree.h"
#include "rb_tree.h"
#include "avl_tree.h"
#include <map>

#include <string>
#include <vector>
#include <iostream>    
#include <algorithm>   
#include <optional>      
#include <type_traits>    

template <typename StorageType>
class Translator {
private:
    Lexical_analysis lex;
    Parser parser;
    Syntax_analysis syn;
    StorageType variables;     

    Polynom processExpression(const std::string& input) {
        Ñalculation evaluator_instance;    
        try {
            std::vector<Token> tokens = lex.lexus(input);
            syn.analyze(tokens);
            std::vector<Token> rpn = parser.transform(tokens);
            return evaluator_instance.evaluate(rpn, variables);
        }
        catch (const std::exception& e) {
            std::cerr << "Error processing expression \"" << input << "\": " << e.what() << std::endl;
            throw;          
        }
    }

public:
    Translator() = default;

    void processInput(const std::string& input_str_orig) {
        std::string input_str = input_str_orig;        

        std::string var_name_candidate = input_str;
        var_name_candidate.erase(std::remove_if(var_name_candidate.begin(), var_name_candidate.end(), ::isspace), var_name_candidate.end());
        if (var_name_candidate.empty()) {
            return;
        }

        size_t equal_pos = input_str.find('=');
        bool is_assignment = (equal_pos != std::string::npos && equal_pos > 0);
        if (is_assignment) {
            if (equal_pos > 0 && (input_str[equal_pos - 1] == '<' || input_str[equal_pos - 1] == '>' || input_str[equal_pos - 1] == '=')) {
                is_assignment = false;        
            }
            if (equal_pos + 1 < input_str.length() && input_str[equal_pos + 1] == '=') {
                is_assignment = false;   
            }
        }


        if (is_assignment) {
            std::string var_name = input_str.substr(0, equal_pos);
            var_name.erase(std::remove_if(var_name.begin(), var_name.end(), ::isspace), var_name.end());

            if (var_name.empty() || !isalpha(var_name[0])) {
                std::cerr << "Error: Invalid variable name for assignment: " << var_name << std::endl;
                return;
            }
            for (size_t i = 1; i < var_name.size(); ++i) {
                if (!isalnum(var_name[i]) && var_name[i] != '_') {
                    std::cerr << "Error: Invalid variable name for assignment: " << var_name << std::endl;
                    return;
                }
            }
            if (var_name == "x" || var_name == "y" || var_name == "z") {
                std::cerr << "Error: Cannot assign to reserved polynom indeterminates (x, y, z)." << std::endl;
                return;
            }

            std::string expression_str = input_str.substr(equal_pos + 1);
            if (expression_str.empty()) {
                std::cerr << "Error: Expression missing after '=' for variable " << var_name << std::endl;
                return;
            }

            try {
                Polynom result = processExpression(expression_str);

                if constexpr (std::is_same_v<StorageType, AddressHashTable<std::string, Polynom>>) {
                    auto it = variables.find(var_name);
                    if (it != variables.end()) {
                        variables.erase(var_name);        
                    }
                    variables.insert(var_name, result);
                }
                else if constexpr (std::is_same_v<StorageType, ChainHash<std::string, Polynom>>) {
                    variables[var_name] = result;
                }
                else if constexpr (std::is_base_of_v<BSTree<std::string, Polynom>, StorageType> ||
                    std::is_same_v<StorageType, BSTree<std::string, Polynom>>) {
                    if (variables.find(var_name) != nullptr) {    
                        variables.erase(var_name);
                    }
                    variables.insert(var_name, result);
                }
                else if constexpr (std::is_same_v<StorageType, OrderedTable<std::string, Polynom>> ||
                    std::is_same_v<StorageType, UnOrderedTable<std::string, Polynom>>) {
                    auto it = variables.find(var_name);
                    if (it != variables.end()) {
                        variables.erase(var_name);    
                    }
                    variables.insert(var_name, result);      
                }
                else if constexpr (std::is_same_v<StorageType, std::map<std::string, Polynom>>) {
                    variables[var_name] = result;
                }
                else {
                    static_assert(!std::is_same_v<StorageType, StorageType>, "Unhandled StorageType in Translator::processInput for assignment.");
                }
                std::cout << var_name << " = " << result << std::endl;

            }
            catch (const std::exception&) {
            }
        }
        else {          
            try {
                std::optional<Polynom> val_to_print_opt;
                std::string potential_var_name = input_str_orig;      
                potential_var_name.erase(std::remove_if(potential_var_name.begin(), potential_var_name.end(), ::isspace), potential_var_name.end());


                if constexpr (std::is_same_v<StorageType, AddressHashTable<std::string, Polynom>>) {
                    auto it = variables.find(potential_var_name);
                    if (it != variables.end()) { val_to_print_opt = it->value; }
                }
                else if constexpr (std::is_same_v<StorageType, ChainHash<std::string, Polynom>>) {
                    auto it = variables.find(potential_var_name);
                    if (it != variables.end()) { val_to_print_opt = it->second; }
                }
                else if constexpr (std::is_base_of_v<BSTree<std::string, Polynom>, StorageType> ||
                    std::is_same_v<StorageType, BSTree<std::string, Polynom>>) {
                    auto* node = variables.find(potential_var_name);
                    if (node) { val_to_print_opt = node->value; }
                }
                else if constexpr (std::is_same_v<StorageType, OrderedTable<std::string, Polynom>> ||
                    std::is_same_v<StorageType, UnOrderedTable<std::string, Polynom>>) {
                    auto it = variables.find(potential_var_name);
                    if (it != variables.end()) { val_to_print_opt = it.value(); }   
                }
                else if constexpr (std::is_same_v<StorageType, std::map<std::string, Polynom>>) {
                    auto it = variables.find(potential_var_name);
                    if (it != variables.end()) { val_to_print_opt = it->second; }
                }
                else {
                    static_assert(!std::is_same_v<StorageType, StorageType>, "Unhandled StorageType in Translator::processInput for variable lookup.");
                }

                if (val_to_print_opt.has_value()) {
                    std::cout << val_to_print_opt.value() << std::endl;
                }
                else {
                    Polynom result = processExpression(input_str_orig);    
                    std::cout << result << std::endl;
                }
            }
            catch (const std::exception&) {
            }
        }
    }

    void execute() {
        std::string line_input;
        std::cout << "\nPolynomial Expression Translator\n" << std::endl;
        std::cout << "Enter polynomial expressions (e.g., 2x^2+y), assignments (e.g., p1 = x+y), or 'exit'.\n" << std::endl;

        while (true) {
            std::cout << ">>> ";
            getline(std::cin, line_input);
            if (std::cin.eof() || line_input == "exit") {
                break;
            }
            if (line_input.empty()) {
                continue;
            }
            processInput(line_input);
        }
    }
};