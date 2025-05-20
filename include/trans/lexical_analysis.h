#pragma once

#include "tokens.h"

class Lexical_analysis {
public:
    std::vector<Token> lexus(const std::string& input);
};