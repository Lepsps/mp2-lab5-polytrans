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

Polynom& Polynom::operator=(std::string& oth) {
	this->terms.clear();
	if (oth.empty()) return *this;

	//std::cout << oth << std::endl;
	float r = 0.0f;
	bool touch = false;
	std::vector<int> p = { 0, 0, 0 };
	size_t c = 0;
	std::string number;
	int last_pow = -1;
	for (size_t j = 0; j < oth.size(); j++) {
		//std::cout << i << std::endl;
		if (isspace(oth[j]) || oth[j] == '^' || oth[j] == '*') continue;
		if (isdigit(oth[j]) || oth[j] == '.') {
			//std::cout << "1if " << oth[j] << std::endl;
			number += oth[j];
			//std::cout << "??? " << number << std::endl;
		}
		else if (oth[j] == 'x' || oth[j] == 'y' || oth[j] == 'z') {
			if (!number.empty() && last_pow != -1) {
				//std::cout << "??? " << number << std::endl;
				p[last_pow] = std::stoi(number);
				//std::cout << "&&& " << number << std::endl;
				number.clear();
			}
			switch (oth[j]) {
			case 'x': {
				p[0] = 1;
				last_pow = 0;
				break;
			}
			case 'y': {
				p[1] = 1;
				last_pow = 1;
				break;
			}
			case 'z': {
				p[2] = 1;
				last_pow = 2;
				break;
			}
			}
			//std::cout << "???rrr " << oth[j] << number << std::endl;
			if (!number.empty() && !touch) {
				if (number == "-") r = -1.0f;
				else r = std::stof(number);
				touch = true;
			}
			if (!touch) { r = 1.0f; touch = true; }
			//std::cout << "???www " << oth[j] << number << std::endl;
			number.clear();
		}
		else if (ispunct(oth[j])) {
			if (!number.empty() && number != "-") {
				if (last_pow == -1) r = std::stof(number);
				else p[last_pow] = std::stoi(number);
			}
			this->addMonom(Monom(r, p));
			r = 0.0f;
			p = { 0, 0, 0 };
			if (oth[j] == '-' && number != "-") {
				number.clear(); number += '-';
			}
			else number.clear();
			last_pow = -1;
			touch = false;
		}
	}

	if (!number.empty() && number != "-") {
		if (last_pow == -1) r = std::stof(number);
		else p[last_pow] = std::stoi(number);
	}
	this->addMonom(Monom(r, p));

	//std::cout << "print " << *this << std::endl;
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