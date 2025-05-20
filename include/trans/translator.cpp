#include "translator.h"
#include <iostream>
#include <algorithm> // Required for std::remove_if

Polynom Translator::processExpression(const std::string& input) {
    try {
        std::vector<Token> tokens = lex.lexus(input);
        syn.analyze(tokens);
        std::vector<Token> rpn = parser.transform(tokens);
        return evaluator.evaluate(rpn, variables);
    }
    catch (const std::exception& e) {
        std::cerr << "Error processing expression \"" << input << "\": " << e.what() << std::endl;
        throw;
    }
}

void Translator::processInput(const std::string& input_str) {
    std::string input = input_str; // Make a mutable copy
    input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());
    if (input.empty()) {
        return;
    }


    size_t equal_pos = input.find('=');
    if (equal_pos != std::string::npos && equal_pos > 0 && input[equal_pos - 1] != '+' && input[equal_pos - 1] != '-' && input[equal_pos - 1] != '*') { // Basic check for assignment
        std::string var_name = input.substr(0, equal_pos);

        if (var_name.empty() || !isalpha(var_name[0])) {
            if (var_name != "x" && var_name != "y" && var_name != "z") { // x,y,z are not assignable vars
                std::cerr << "Error: Invalid variable name for assignment: " << var_name << std::endl;
                return;
            }
        }
        for (size_t i = 1; i < var_name.size(); i++) {
            if (!isalnum(var_name[i]) && var_name[i] != '_') {
                if (var_name != "x" && var_name != "y" && var_name != "z") {
                    std::cerr << "Error: Invalid variable name for assignment: " << var_name << std::endl;
                    return;
                }
            }
        }
        if (var_name == "x" || var_name == "y" || var_name == "z") {
            std::cerr << "Error: Cannot assign to reserved polynom indeterminates (x, y, z)." << std::endl;
            return;
        }


        std::string expression_str = input.substr(equal_pos + 1);
        if (expression_str.empty()) {
            std::cerr << "Error: Expression missing after '=' for variable " << var_name << std::endl;
            return;
        }

        try {
            Polynom result = processExpression(expression_str);
            variables[var_name] = result;
            std::cout << var_name << " = " << result << std::endl;
        }
        catch (const std::exception&) {

        }

    }
    else { // Not an assignment, evaluate directly
        try {
            Polynom result = processExpression(input_str); // Use original input string with spaces for lexer
            std::cout << result << std::endl;
        }
        catch (const std::exception&) {

        }
    }
}

void Translator::execute() {
    std::string line_input;
    std::cout << "\nPolynomial Expression Translator\n" << std::endl;
    std::cout << "Enter polynomial expressions (e.g., 2x^2 + y - 3z), assignments (e.g., p1 = x+y), or 'exit'.\n" << std::endl;

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