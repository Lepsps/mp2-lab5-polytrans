#pragma once

#include "tokens.h"
#include "polynoms.h"
#include <map>
#include <stack>

class Calculation {
public:
    Polynom evaluate(const std::vector<Token>& rpn, std::map<std::string, Polynom>& variables);
};