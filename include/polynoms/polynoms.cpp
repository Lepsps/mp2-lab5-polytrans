#include "polynoms.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cmath>




Monom::Monom(float ratio, std::vector<int> powers) {
	this->ratio = ratio;
	if (ratio == 0.0f) this->powers = { 0, 0, 0 };
	else this->powers = powers;
}

bool Monom::operator==(const Monom& oth) const {
	return (((ratio == oth.ratio) && (powers == oth.powers)));
}

bool Monom::operator!=(const Monom& oth) const {
	return !(*this == oth);
}

Monom Monom::operator+(const Monom& oth) const {
	if (powers != oth.powers) {
		throw std::invalid_argument("Different power");
	}
	return Monom(ratio + oth.ratio, powers);
}

Monom Monom::operator-(const Monom& oth) const {
	if (powers != oth.powers) {
		throw std::invalid_argument("Different power");
	}
	return Monom(ratio - oth.ratio, powers);
}

Monom Monom::operator*(const Monom& oth) const {
	std::vector<int> new_powers = { 0, 0, 0 };
	if (ratio * oth.ratio == 0) return Monom();
	for (int i = 0; i < 3; i++) {
		new_powers[i] = powers[i] + oth.powers[i];
	}
	return Monom(ratio * oth.ratio, new_powers);
}

Monom Monom::operator/(const Monom& oth) const {
	if (oth.ratio == 0.0f) throw std::invalid_argument("Division by 0");
	if (this->ratio == 0.0f) return Monom();

	std::vector<int> new_powers = { 0, 0, 0 };
	for (int i = 0; i < 3; i++) {
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
	int max_this = *std::max_element(begin(this->powers),
		end(this->powers));
	int max_oth = *std::max_element(begin(oth.powers),
		end(oth.powers));

	if (max_this > max_oth) return true;
	if (max_this < max_oth) return false;

	if (this->powers[0] > oth.powers[0]) return true;
	if (this->powers[0] < oth.powers[0]) return false;

	if (this->powers[1] > oth.powers[1]) return true;
	if (this->powers[1] < oth.powers[1]) return false;

	if (this->powers[2] > oth.powers[2]) return true;
	if (this->powers[2] < oth.powers[2]) return false;

	if (this->ratio > oth.ratio) return true;

	return false;
}

bool Monom::operator<(const Monom& oth) const {
	return oth > *this;
}

std::ostream& operator<<(std::ostream& os, const Monom& monom) {
	if (monom.ratio == 0.0f) {
		os << 0;
		return os;
	}

	if ((std::abs(monom.ratio) != 1) || (monom.powers == std::vector<int>{ 0, 0, 0})) {
		os << monom.ratio;
	}

	else if (monom.ratio == -1) {
		os << "-";
	}

	for (int i = 0; i < 3; i++) {
		if (monom.powers[i] != 0) {
			switch (i) {
			case 0: os << "x"; break;
			case 1: os << "y"; break;
			case 2: os << "z"; break;
			}
			if (monom.powers[i] != 1) {
				os << "^" << monom.powers[i];
			}
		}
	}

	return os;
}



Polynom::Polynom(CyclicList<Monom>& list) : terms(list) {
	simplify();
}

void Polynom::addMonom(const Monom& monom) {
	if (monom.ratio != 0.0f) {
		terms.push_back(monom);
		simplify();
	}
}

void Polynom::deleteMonom(const size_t index) {
	this->terms.erase(index);
}

Polynom Polynom::operator+(const Polynom& oth) {
	Polynom result = *this;
	for (size_t i = 0; i < oth.terms.size(); i++) {
		result.addMonom(oth.terms[i]);
	}
	return result;
}

Polynom Polynom::operator+(const Monom& monom) {
	Polynom result = *this;
	result.addMonom(monom);
	return result;
}

Polynom Polynom::operator-(const Polynom& oth) {
	Polynom result = *this;
	for (size_t i = 0; i < oth.terms.size(); i++) {
		Monom temp = oth.terms[i];
		temp.ratio *= -1;
		result.addMonom(temp);
	}
	return result;
}

Polynom Polynom::operator*(const Polynom& oth) const {
	CyclicList<Monom> result_terms;
	if (this->terms.empty() || oth.terms.empty()) {
		return Polynom();
	}

	for (size_t i = 0; i < oth.terms.size(); i++) {
		for (size_t j = 0; j < this->terms.size(); j++) {
			Monom m = oth.terms[i] * this->terms[j];
			if (m.ratio != 0) {
				result_terms.push_back(m);
			}
		}
	}
	Polynom result(result_terms);
	return result;
}

Polynom& Polynom::operator+=(const Polynom& oth) {
	*this = *this + oth;
	return *this;
}

Polynom& Polynom::operator+=(const Monom& oth) {
	*this = *this + oth;
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

	for (size_t i = 0; i < poly.terms.size(); i++) {
		if (i != 0 && poly.terms[i].ratio >= 0) {
			os << "+";
		}
		os << poly.terms[i];
	}
	return os;
}

Polynom& Polynom::operator=(std::string& oth_str) {
	this->terms.clear();
	std::string s = oth_str;
	s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());

	if (s.empty()) return *this;

	std::string current_term_str;
	for (size_t i = 0; i < s.length(); ++i) {
		current_term_str += s[i];
		if (i + 1 < s.length() && (s[i + 1] == '+' || (s[i + 1] == '-' && !isdigit(s[i])))) { // !isdigit(s[i]) to handle x^-2 like things if they were allowed
			// Process current_term_str
			// This simple split won't work for Polynom class's original parser logic
			// The original parser was more stateful.
			// This assignment operator now assumes 'oth_str' is a SINGLE MONOMIAL string.
		}
	}
	// Process the last/only term
	if (current_term_str.empty()) return *this;


	float r = 1.0f;
	std::vector<int> p = { 0, 0, 0 };
	std::string num_part;
	size_t k = 0;

	if (current_term_str[k] == '+') {
		k++;
	}
	else if (current_term_str[k] == '-') {
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
		catch (const std::out_of_range& oor) {
			throw std::invalid_argument("Coefficient number too large in: " + current_term_str);
		}
		catch (const std::invalid_argument& ia) {
			throw std::invalid_argument("Invalid coefficient format in: " + current_term_str);
		}

	}
	else { // No explicit number part before variables, e.g. "x" or "-y"
		if (k < current_term_str.length() && (current_term_str[k] == 'x' || current_term_str[k] == 'y' || current_term_str[k] == 'z')) {
			// r is already 1.0 or -1.0
		}
		else if (k == current_term_str.length() && num_part.empty()) { // String was just "+" or "-"
			throw std::invalid_argument("Invalid term: " + current_term_str);
		}
	}


	bool var_found = false;
	for (; k < current_term_str.length(); ++k) {
		var_found = true;
		char var_char = current_term_str[k];
		int var_idx = -1;
		if (var_char == 'x') var_idx = 0;
		else if (var_char == 'y') var_idx = 1;
		else if (var_char == 'z') var_idx = 2;
		else throw std::invalid_argument("Invalid character in term: " + current_term_str);

		int power = 1;
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
			if (pow_str.empty()) throw std::invalid_argument("Missing exponent in: " + current_term_str);
			try {
				power = std::stoi(pow_str);
				if (neg_pow) power *= -1;
			}
			catch (const std::out_of_range& oor) {
				throw std::invalid_argument("Exponent number too large in: " + current_term_str);
			}
			catch (const std::invalid_argument& ia) {
				throw std::invalid_argument("Invalid exponent format in: " + current_term_str);
			}
			k--;
		}
		p[var_idx] += power;
	}

	if (std::abs(r) < 1e-6 && var_found) { // e.g. 0x, 0y^2 make r=0.0, p={0,0,0}
		this->addMonom(Monom(0.0f, { 0,0,0 }));
	}
	else {
		this->addMonom(Monom(r, p));
	}
	return *this;
}

void Polynom::simplify() {
	if (terms.empty() || terms.size() == 1) {
		return;
	}

	//std::cout << *this << std::endl;
	terms.sort(true);
	//std::cout << *this << std::endl;
	CyclicList<Monom> newTerms;
	Monom curr = terms[0];

	for (size_t i = 1; i < terms.size(); i++) {
		if (terms[i].powers == curr.powers) {
			curr.ratio += terms[i].ratio;
		}
		else {
			if (curr.ratio != 0.0f) {
				newTerms.push_back(curr);
			}
			curr = terms[i];
		}
	}
	if (curr.ratio != 0.0f) newTerms.push_back(curr);
	terms = newTerms;
}