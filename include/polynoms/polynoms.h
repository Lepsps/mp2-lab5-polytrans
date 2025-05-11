#pragma once

#include "cyclic_list.h"
#include <iostream>
#include <string>
#include <vector>


class Monom {
public:
	float ratio;
	std::vector<int> powers;

	Monom(float ratio = 0.0f, std::vector<int> powers = { 0, 0, 0 });

	bool operator==(const Monom& oth) const;
	bool operator!=(const Monom& oth) const;
	Monom operator+(const Monom& oth) const;
	Monom operator-(const Monom& oth) const;
	Monom operator*(const Monom& oth) const;
	Monom operator/(const Monom& oth) const;
	Monom& operator+=(const Monom& oth);
	Monom& operator-=(const Monom& oth);
	Monom& operator*=(const Monom& oth);
	Monom& operator/=(const Monom& oth);
	bool operator>(const Monom& oth) const;
	bool operator<(const Monom& oth) const;

	friend std::ostream& operator<<(std::ostream& os, const Monom& monom);
};


class Polynom{
private:
	CyclicList<Monom> terms;

	void simplify();

public:
	Polynom() = default;

	Polynom(CyclicList<Monom>& list);

	void addMonom(const Monom& monom);
	void deleteMonom(const size_t index);

	Polynom operator+(const Polynom& oth);
	Polynom operator+(const Monom& monom);
	Polynom operator-(const Polynom& oth);
	Polynom operator*(const Polynom& oth) const;
	Polynom& operator+=(const Polynom& oth);
	Polynom& operator+=(const Monom& oth);
	Polynom& operator-=(const Polynom& oth);
	Polynom& operator*=(const Polynom& oth);

	friend std::ostream& operator<<(std::ostream& os, const Polynom& poly);

	Polynom& operator=(std::string& oth);
};