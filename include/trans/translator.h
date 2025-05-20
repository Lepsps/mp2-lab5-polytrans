#pragma once

#include "lexical_analysis.h"
#include "syntax_analysis.h"
#include "parser.h"
#include "calculation.h"
#include "polynoms.h"
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>


class Translator {
private:
    Lexical_analysis lex;
    Parser parser;
    Calculation evaluator;
    Syntax_analysis syn;
    std::map<std::string, Polynom> variables;

    Polynom processExpression(const std::string& input);

public:
    void processInput(const std::string& input);
    void execute();
};