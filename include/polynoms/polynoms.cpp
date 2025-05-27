#include "polynoms.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>    
#include <cmath>         
#include <functional>      

const float EPSILON = 1e-6f;

Monom::Monom(float ratio_val, std::vector<int> powers_val) {
    this->ratio = ratio_val;
    if (std::abs(this->ratio) < EPSILON) {
        this->ratio = 0.0f;
        this->powers = { 0, 0, 0 };      
    }
    else {
        if (powers_val.size() != 3) {
            if (powers_val.empty() && ratio_val != 0.0f) {    
                this->powers = { 0,0,0 };  
            }
            else if (powers_val.size() != 3 && ratio_val != 0.0f) {
                throw std::invalid_argument("Monom powers vector must have size 3 or be default.");
            }
            else {
                this->powers = powers_val;
            }
        }
        else {
            this->powers = powers_val;
        }
    }
}

bool Monom::operator==(const Monom& oth) const {
    if (std::abs(ratio) < EPSILON && std::abs(oth.ratio) < EPSILON) {
        return true;
    }
    return (std::abs(ratio - oth.ratio) < EPSILON && powers == oth.powers);
}

bool Monom::operator!=(const Monom& oth) const {
    return !(*this == oth);
}

Monom Monom::operator+(const Monom& oth) const {
    if (powers != oth.powers) {
        throw std::invalid_argument("Cannot add monoms with different powers.");
    }
    return Monom(ratio + oth.ratio, powers);
}

Monom Monom::operator-(const Monom& oth) const {
    if (powers != oth.powers) {
        throw std::invalid_argument("Cannot subtract monoms with different powers.");
    }
    return Monom(ratio - oth.ratio, powers);
}

Monom Monom::operator*(const Monom& oth) const {
    if (std::abs(this->ratio) < EPSILON || std::abs(oth.ratio) < EPSILON) {
        return Monom(0.0f);      
    }
    std::vector<int> new_powers = { 0, 0, 0 };
    for (size_t i = 0; i < 3; ++i) {
        new_powers[i] = powers[i] + oth.powers[i];
    }
    return Monom(ratio * oth.ratio, new_powers);
}

Monom Monom::operator/(const Monom& oth) const {
    if (std::abs(oth.ratio) < EPSILON) {
        throw std::invalid_argument("Division by zero monom.");
    }
    if (std::abs(this->ratio) < EPSILON) {
        return Monom(0.0f);      
    }
    std::vector<int> new_powers = { 0, 0, 0 };
    for (size_t i = 0; i < 3; ++i) {
        new_powers[i] = powers[i] - oth.powers[i];
    }
    return Monom(ratio / oth.ratio, new_powers);
}

Monom& Monom::operator+=(const Monom& oth) {
    *this = *this + oth;
    return *this;
}

Monom& Monom::operator-=(const Monom& oth) {
    *this = *this - oth;
    return *this;
}

Monom& Monom::operator*=(const Monom& oth) {
    *this = *this * oth;
    return *this;
}

Monom& Monom::operator/=(const Monom& oth) {
    *this = *this / oth;
    return *this;
}

bool Monom::operator>(const Monom& oth) const {
    bool this_is_zero = std::abs(this->ratio) < EPSILON;
    bool oth_is_zero = std::abs(oth.ratio) < EPSILON;

    if (this_is_zero && !oth_is_zero) return false;    
    if (!this_is_zero && oth_is_zero) return true;     
    if (this_is_zero && oth_is_zero) return false;       

    int max_this = (powers.empty() ? 0 : *std::max_element(powers.begin(), powers.end()));
    int max_oth = (oth.powers.empty() ? 0 : *std::max_element(oth.powers.begin(), oth.powers.end()));

    if (max_this > max_oth) return true;
    if (max_this < max_oth) return false;

    for (size_t i = 0; i < 3; ++i) {
        if (this->powers[i] > oth.powers[i]) return true;
        if (this->powers[i] < oth.powers[i]) return false;
    }

    return false;
}

bool Monom::operator<(const Monom& oth) const {
    bool this_is_zero = std::abs(this->ratio) < EPSILON;
    bool oth_is_zero = std::abs(oth.ratio) < EPSILON;

    if (this_is_zero && !oth_is_zero) return true;     
    if (!this_is_zero && oth_is_zero) return false;    
    if (this_is_zero && oth_is_zero) return false;       

    int max_this = (powers.empty() ? 0 : *std::max_element(powers.begin(), powers.end()));
    int max_oth = (oth.powers.empty() ? 0 : *std::max_element(oth.powers.begin(), oth.powers.end()));

    if (max_this < max_oth) return true;
    if (max_this > max_oth) return false;

    for (size_t i = 0; i < 3; ++i) {
        if (this->powers[i] < oth.powers[i]) return true;
        if (this->powers[i] > oth.powers[i]) return false;
    }
    return false;   
}

std::ostream& operator<<(std::ostream& os, const Monom& monom) {
    if (std::abs(monom.ratio) < EPSILON) {
        os << 0;
        return os;
    }

    bool all_powers_zero = true;
    for (int p : monom.powers) {
        if (p != 0) {
            all_powers_zero = false;
            break;
        }
    }

    if (monom.ratio == -1.0f && !all_powers_zero) {
        os << "-";
    }
    else if (monom.ratio == 1.0f && !all_powers_zero) {
    }
    else {
        if (monom.ratio == static_cast<long long>(monom.ratio)) {
            os << static_cast<long long>(monom.ratio);
        }
        else {
            os << monom.ratio;
        }
    }

    const char vars[] = { 'x', 'y', 'z' };
    for (size_t i = 0; i < monom.powers.size() && i < 3; ++i) {
        if (monom.powers[i] != 0) {
            os << vars[i];
            if (monom.powers[i] != 1) {
                os << "^" << monom.powers[i];
            }
        }
    }
    return os;
}

void Polynom::normalize() {
    if (terms.empty()) {
        return;
    }
    if (terms.size() == 1 && std::abs(terms[0].ratio) < EPSILON) {
        terms.clear();
        return;
    }
    if (terms.size() < 2 && !(terms.size() == 1 && std::abs(terms[0].ratio) < EPSILON)) {       
        return;
    }


    terms.sort(true);       

    CyclicList<Monom> normalized_terms;
    auto* current_node_ptr = terms.get_head_node();      
    if (!current_node_ptr) {          
        terms = normalized_terms;    
        return;
    }

    Monom current_aggregate = current_node_ptr->data;
    current_node_ptr = current_node_ptr->next;     

    for (size_t i = 1; i < terms.size(); ++i) {        
        const Monom& next_monom = current_node_ptr->data;

        if (current_aggregate.powers == next_monom.powers) {
            current_aggregate.ratio += next_monom.ratio;
        }
        else {
            if (std::abs(current_aggregate.ratio) > EPSILON) {
                normalized_terms.push_back(current_aggregate);
            }
            current_aggregate = next_monom;    
        }
        current_node_ptr = current_node_ptr->next;
    }

    if (std::abs(current_aggregate.ratio) > EPSILON) {
        normalized_terms.push_back(current_aggregate);
    }

    terms = std::move(normalized_terms);
}


Polynom::Polynom(const CyclicList<Monom>& list) : terms(list) {    
    normalize();
}

Polynom::Polynom(CyclicList<Monom>&& list) : terms(std::move(list)) {
    normalize();
}


void Polynom::addMonom(const Monom& monom_to_add) {
    if (std::abs(monom_to_add.ratio) < EPSILON) {
        return;     
    }

    CyclicList<Monom> new_terms;
    auto* current_node_ptr = terms.get_head_node();
    size_t count = 0;
    const size_t original_size = terms.size();
    bool inserted_or_merged = false;

    while (count < original_size) {
        const Monom& list_monom = current_node_ptr->data;

        if (!inserted_or_merged) {
            if (list_monom.powers == monom_to_add.powers) {
                Monom sum = list_monom;  
                sum.ratio += monom_to_add.ratio;
                if (std::abs(sum.ratio) > EPSILON) {
                    new_terms.push_back(sum);
                }
                inserted_or_merged = true;
            }
            else if (monom_to_add > list_monom) {     
                new_terms.push_back(monom_to_add);
                new_terms.push_back(list_monom);     
                inserted_or_merged = true;
            }
            else {      
                new_terms.push_back(list_monom);
            }
        }
        else {        
            new_terms.push_back(list_monom);
        }
        current_node_ptr = current_node_ptr->next;
        count++;
    }

    if (!inserted_or_merged) {         
        new_terms.push_back(monom_to_add);
    }

    terms = std::move(new_terms);
}

void Polynom::deleteMonom(const size_t index) {
    if (index >= terms.size()) {
        throw std::out_of_range("Polynom::deleteMonom: index out of range");
    }
    terms.erase(index);  
}


Polynom Polynom::operator+(const Polynom& oth) const {
    Polynom result;    
    auto* p1_node = this->terms.get_head_node();
    auto* p2_node = oth.terms.get_head_node();

    size_t p1_count = 0;
    size_t p2_count = 0;
    const size_t p1_size = this->terms.size();
    const size_t p2_size = oth.terms.size();

    while (p1_count < p1_size && p2_count < p2_size) {
        const Monom& m1 = p1_node->data;
        const Monom& m2 = p2_node->data;

        if (m1.powers == m2.powers) {
            Monom sum_monom = m1;      
            sum_monom.ratio += m2.ratio;
            if (std::abs(sum_monom.ratio) > EPSILON) {
                result.terms.push_back(sum_monom);
            }
            p1_node = p1_node->next;
            p2_node = p2_node->next;
            p1_count++;
            p2_count++;
        }
        else if (m1 > m2) {        
            result.terms.push_back(m1);
            p1_node = p1_node->next;
            p1_count++;
        }
        else {        
            result.terms.push_back(m2);
            p2_node = p2_node->next;
            p2_count++;
        }
    }

    while (p1_count < p1_size) {
        result.terms.push_back(p1_node->data);
        p1_node = p1_node->next;
        p1_count++;
    }
    while (p2_count < p2_size) {
        result.terms.push_back(p2_node->data);
        p2_node = p2_node->next;
        p2_count++;
    }
    return result;
}

Polynom Polynom::operator+(const Monom& monom) const {
    Polynom result = *this;      
    result.addMonom(monom);      
    return result;
}

Polynom Polynom::operator-(const Polynom& oth) const {
    Polynom neg_oth;   
    auto* oth_node = oth.terms.get_head_node();
    size_t count = 0;
    const size_t oth_size = oth.terms.size();

    while (count < oth_size) {
        Monom temp = oth_node->data;  
        temp.ratio *= -1.0f;
        if (std::abs(temp.ratio) > EPSILON) {       
            neg_oth.terms.push_back(temp);   
        }
        oth_node = oth_node->next;
        count++;
    }
    return *this + neg_oth;     
}

Polynom Polynom::operator*(const Polynom& oth) const {
    if (this->terms.empty() || oth.terms.empty()) {
        return Polynom();        
    }

    CyclicList<Monom> temp_result_terms;
    auto* this_node = this->terms.get_head_node();
    size_t this_count = 0;
    const size_t this_size = this->terms.size();

    while (this_count < this_size) {
        auto* oth_node = oth.terms.get_head_node();
        size_t oth_count = 0;
        const size_t oth_size = oth.terms.size();
        while (oth_count < oth_size) {
            Monom product = this_node->data * oth_node->data;
            if (std::abs(product.ratio) > EPSILON) {
                temp_result_terms.push_back(product);
            }
            oth_node = oth_node->next;
            oth_count++;
        }
        this_node = this_node->next;
        this_count++;
    }

    return Polynom(temp_result_terms);
}


Polynom& Polynom::operator+=(const Polynom& oth) {
    *this = *this + oth;
    return *this;
}

Polynom& Polynom::operator+=(const Monom& monom) {
    this->addMonom(monom);      
    return *this;
}

Polynom& Polynom::operator-=(const Polynom& oth) {
    *this = *this - oth;
    return *this;
}

Polynom& Polynom::operator*=(const Polynom& oth) {
    *this = *this * oth;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Polynom& poly) {
    if (poly.terms.empty()) {
        os << 0;
        return os;
    }

    auto* current_node = poly.terms.get_head_node();
    size_t count = 0;
    const size_t terms_size = poly.terms.size();

    bool first_term = true;
    while (count < terms_size) {
        const Monom& term = current_node->data;
        if (!first_term && term.ratio > 0) {          
            os << "+";
        }
        os << term;
        first_term = false;
        current_node = current_node->next;
        count++;
    }
    return os;
}

Polynom& Polynom::operator=(std::string& oth_str_ref) {        
    this->terms.clear();
    std::string s = oth_str_ref;        
    s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());

    if (s.empty()) return *this;

    std::string current_term_str = s;      

    float r = 1.0f;
    std::vector<int> p = { 0, 0, 0 };    
    std::string num_part;
    size_t k = 0;

    if (k < current_term_str.length() && current_term_str[k] == '+') {
        k++;
    }
    else if (k < current_term_str.length() && current_term_str[k] == '-') {
        r = -1.0f;
        k++;
    }

    while (k < current_term_str.length() && (isdigit(current_term_str[k]) || current_term_str[k] == '.')) {
        num_part += current_term_str[k];
        k++;
    }

    if (!num_part.empty()) {
        try {
            r *= std::stof(num_part);
        }
        catch (const std::out_of_range&) {
            throw std::invalid_argument("Coefficient number too large in: " + current_term_str);
        }
        catch (const std::invalid_argument&) {             
            if (k < current_term_str.length() && (current_term_str[k] == 'x' || current_term_str[k] == 'y' || current_term_str[k] == 'z')) {
            }
            else {
                throw std::invalid_argument("Invalid coefficient format in: " + current_term_str);
            }
        }
    }
    else {
        if (k == current_term_str.length() && (s == "+" || s == "-")) {
            throw std::invalid_argument("Invalid term (just a sign): " + current_term_str);
        }
    }


    bool var_found_in_term = false;
    for (; k < current_term_str.length(); ++k) {
        var_found_in_term = true;
        char var_char = current_term_str[k];
        int var_idx = -1;
        if (var_char == 'x') var_idx = 0;
        else if (var_char == 'y') var_idx = 1;
        else if (var_char == 'z') var_idx = 2;
        else throw std::invalid_argument("Invalid variable character '" + std::string(1, var_char) + "' in term: " + current_term_str);

        int power_val = 1;
        if (k + 1 < current_term_str.length() && current_term_str[k + 1] == '^') {
            k += 2;
            std::string pow_str;
            bool neg_pow = false;
            if (k < current_term_str.length() && current_term_str[k] == '-') {
                neg_pow = true;
                k++;
            }
            while (k < current_term_str.length() && isdigit(current_term_str[k])) {
                pow_str += current_term_str[k];
                k++;
            }
            if (pow_str.empty()) throw std::invalid_argument("Missing exponent value after '^' in: " + current_term_str);
            try {
                power_val = std::stoi(pow_str);
                if (neg_pow) power_val *= -1;
            }
            catch (const std::out_of_range&) {
                throw std::invalid_argument("Exponent number too large in: " + current_term_str);
            }
            catch (const std::invalid_argument&) {
                throw std::invalid_argument("Invalid exponent format in: " + current_term_str);
            }
            k--;      
        }
        p[var_idx] += power_val;          
    }

    this->addMonom(Monom(r, p));
    return *this;
}