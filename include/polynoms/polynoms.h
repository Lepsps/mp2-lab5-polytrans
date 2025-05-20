#pragma once

#include "cyclic_list.h"          
#include <string>
#include <vector>
#include <functional>      

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


class Polynom {
private:
    CyclicList<Monom> terms;    
    void normalize();

public:
    Polynom() = default;

    explicit Polynom(const CyclicList<Monom>& list);
    explicit Polynom(CyclicList<Monom>&& list);


    void addMonom(const Monom& monom);

    void deleteMonom(const size_t index);

    Polynom operator+(const Polynom& oth) const;    
    Polynom operator+(const Monom& monom) const;    
    Polynom operator-(const Polynom& oth) const;
    Polynom operator*(const Polynom& oth) const;    

    Polynom& operator+=(const Polynom& oth);
    Polynom& operator+=(const Monom& monom);
    Polynom& operator-=(const Polynom& oth);
    Polynom& operator*=(const Polynom& oth);

    friend std::ostream& operator<<(std::ostream& os, const Polynom& poly);

    Polynom& operator=(std::string& oth_str);

};