#include "cyclic_list.h"
#include "polynoms.h"

#include "gtest.h" 

#include <sstream>    // Для тестирования operator<<

// Вспомогательная функция для сравнения полиномов через строковое представление,
// так как Polynom не имеет operator==
// Это полезно, потому что simplify() приводит полиномы к канонической форме.
std::string polynomToString(const Polynom& p) {
    std::ostringstream oss;
    oss << p;
    return oss.str();
}

// Вспомогательная функция для сравнения мономов через строковое представление
std::string monomToString(const Monom& m) {
    std::ostringstream oss;
    oss << m;
    return oss.str();
}


// --- Тесты для Monom ---

TEST(MonomTest, DefaultConstructor) {
    Monom m;
    ASSERT_FLOAT_EQ(0.0f, m.ratio);
    ASSERT_EQ(std::vector<int>({ 0, 0, 0 }), m.powers);
}

TEST(MonomTest, ConstructorWithValues) {
    Monom m(2.5f, { 1, 2, 3 });
    ASSERT_FLOAT_EQ(2.5f, m.ratio);
    ASSERT_EQ(std::vector<int>({ 1, 2, 3 }), m.powers);
}

TEST(MonomTest, ConstructorZeroRatio) {
    Monom m(0.0f, { 1, 2, 3 }); // Степени должны сброситься
    ASSERT_FLOAT_EQ(0.0f, m.ratio);
    ASSERT_EQ(std::vector<int>({ 0, 0, 0 }), m.powers);
}

TEST(MonomTest, Equality) {
    Monom m1(2.0f, { 1, 0, 0 });
    Monom m2(2.0f, { 1, 0, 0 });
    Monom m3(3.0f, { 1, 0, 0 });
    Monom m4(2.0f, { 0, 1, 0 });
    ASSERT_EQ(m1, m2);
    ASSERT_NE(m1, m3);
    ASSERT_NE(m1, m4);
    ASSERT_TRUE(m1 == m2);
    ASSERT_FALSE(m1 == m3);
    ASSERT_TRUE(m1 != m3);
}

TEST(MonomTest, Addition) {
    Monom m1(2.0f, { 1, 0, 0 }); // 2x
    Monom m2(3.0f, { 1, 0, 0 }); // 3x
    Monom m3(1.0f, { 0, 1, 0 }); // y

    Monom sum = m1 + m2;
    ASSERT_FLOAT_EQ(5.0f, sum.ratio);
    ASSERT_EQ(std::vector<int>({ 1, 0, 0 }), sum.powers);

    ASSERT_THROW(m1 + m3, std::invalid_argument);

    m1 += m2;
    ASSERT_FLOAT_EQ(5.0f, m1.ratio);
    ASSERT_EQ(std::vector<int>({ 1, 0, 0 }), m1.powers);
}

TEST(MonomTest, Subtraction) {
    Monom m1(5.0f, { 1, 0, 0 }); // 5x
    Monom m2(3.0f, { 1, 0, 0 }); // 3x
    Monom m3(1.0f, { 0, 1, 0 }); // y

    Monom diff = m1 - m2;
    ASSERT_FLOAT_EQ(2.0f, diff.ratio);
    ASSERT_EQ(std::vector<int>({ 1, 0, 0 }), diff.powers);

    ASSERT_THROW(m1 - m3, std::invalid_argument);

    m1 -= m2;
    ASSERT_FLOAT_EQ(2.0f, m1.ratio);
    ASSERT_EQ(std::vector<int>({ 1, 0, 0 }), m1.powers);
}

TEST(MonomTest, Multiplication) {
    Monom m1(2.0f, { 1, 2, 0 }); // 2xy^2
    Monom m2(3.0f, { 0, 1, 3 }); // 3yz^3
    Monom m_zero(0.0f, { 1,1,1 });

    Monom prod = m1 * m2;
    ASSERT_FLOAT_EQ(6.0f, prod.ratio);
    ASSERT_EQ(std::vector<int>({ 1, 3, 3 }), prod.powers); // x y^3 z^3

    Monom prod_with_zero = m1 * m_zero;
    ASSERT_FLOAT_EQ(0.0f, prod_with_zero.ratio);
    ASSERT_EQ(std::vector<int>({ 0,0,0 }), prod_with_zero.powers);

    m1 *= m2;
    ASSERT_FLOAT_EQ(6.0f, m1.ratio);
    ASSERT_EQ(std::vector<int>({ 1, 3, 3 }), m1.powers);
}

TEST(MonomTest, Division) {
    Monom m1(6.0f, { 2, 3, 1 }); // 6x^2y^3z
    Monom m2(3.0f, { 1, 1, 1 }); // 3xyz
    Monom m3(2.0f, { 0, 0, 0 }); // 2 (константа)
    Monom m_zero_ratio(0.0f, { 1,1,1 });
    Monom m_zero_val(0.0f, { 0,0,0 });


    Monom quot = m1 / m2;
    ASSERT_FLOAT_EQ(2.0f, quot.ratio);
    ASSERT_EQ(std::vector<int>({ 1, 2, 0 }), quot.powers); // 2xy^2

    Monom quot_by_const = m1 / m3;
    ASSERT_FLOAT_EQ(3.0f, quot_by_const.ratio);
    ASSERT_EQ(std::vector<int>({ 2,3,1 }), quot_by_const.powers);

    Monom quot_zero_by_m1 = m_zero_val / m1;
    ASSERT_FLOAT_EQ(0.0f, quot_zero_by_m1.ratio);
    ASSERT_EQ(std::vector<int>({ 0,0,0 }), quot_zero_by_m1.powers);


    ASSERT_THROW(m1 / m_zero_ratio, std::invalid_argument); // Деление на моном с нулевым коэффициентом
    ASSERT_THROW(m1 / m_zero_val, std::invalid_argument);  // Деление на полностью нулевой моном

    m1 /= m2;
    ASSERT_FLOAT_EQ(2.0f, m1.ratio);
    ASSERT_EQ(std::vector<int>({ 1, 2, 0 }), m1.powers);
}

TEST(MonomTest, Comparison) {
    // x^3 vs x^2 (max power)
    Monom m1(1.0f, { 3, 0, 0 });
    Monom m2(1.0f, { 2, 0, 0 });
    ASSERT_TRUE(m1 > m2);
    ASSERT_FALSE(m1 < m2);

    // x^2y vs x^2z (max power same, compare powers[0], then powers[1])
    Monom m3(1.0f, { 2, 1, 0 }); // x^2y
    Monom m4(1.0f, { 2, 0, 1 }); // x^2z
    ASSERT_TRUE(m3 > m4); // y > z effectively because y is powers[1] and z is powers[2]

    // x^2y vs x^2y (same powers, different ratio)
    Monom m5(2.0f, { 2, 1, 0 }); // 2x^2y
    Monom m6(1.0f, { 2, 1, 0 }); // x^2y
    ASSERT_TRUE(m5 > m6);

    // x vs y
    Monom m7(1.0f, { 1,0,0 }); // x
    Monom m8(1.0f, { 0,1,0 }); // y
    ASSERT_TRUE(m7 > m8); // Max power same (1), x comes before y

    // y vs z
    Monom m9(1.0f, { 0,1,0 }); // y
    Monom m10(1.0f, { 0,0,1 }); // z
    ASSERT_TRUE(m9 > m10);

    // x^2 vs y^3 (max power wins)
    Monom m11(1.0f, { 2,0,0 }); // x^2
    Monom m12(1.0f, { 0,3,0 }); // y^3
    ASSERT_FALSE(m11 > m12); // y^3 is "greater"
    ASSERT_TRUE(m12 > m11);

    // Identical
    Monom m13(1.0f, { 1,1,1 });
    Monom m14(1.0f, { 1,1,1 });
    ASSERT_FALSE(m13 > m14); // Not strictly greater
    ASSERT_FALSE(m13 < m14);

    // Check based on powers array order
    Monom ma(1.0f, { 1,1,2 }); // xy z^2
    Monom mb(1.0f, { 1,2,1 }); // x y^2 z
    // max_this = 2, max_oth = 2
    // this->powers[0] == oth.powers[0] (1 == 1)
    // this->powers[1] < oth.powers[1] (1 < 2) => mb > ma
    ASSERT_TRUE(mb > ma);
    ASSERT_FALSE(ma > mb);
}

TEST(MonomTest, OutputStream) {
    ASSERT_EQ("0", monomToString(Monom(0.0f, { 1,1,1 })));
    ASSERT_EQ("0", monomToString(Monom()));
    ASSERT_EQ("5", monomToString(Monom(5.0f)));
    ASSERT_EQ("-2.5", monomToString(Monom(-2.5f)));
    ASSERT_EQ("x", monomToString(Monom(1.0f, { 1,0,0 })));
    ASSERT_EQ("-x", monomToString(Monom(-1.0f, { 1,0,0 })));
    ASSERT_EQ("2.5x", monomToString(Monom(2.5f, { 1,0,0 })));
    ASSERT_EQ("y^2", monomToString(Monom(1.0f, { 0,2,0 })));
    ASSERT_EQ("-z^3", monomToString(Monom(-1.0f, { 0,0,3 })));
    ASSERT_EQ("3.1x^2y^3z^4", monomToString(Monom(3.1f, { 2,3,4 })));
    ASSERT_EQ("xy", monomToString(Monom(1.0f, { 1,1,0 })));
    ASSERT_EQ("-xyz", monomToString(Monom(-1.0f, { 1,1,1 })));
    ASSERT_EQ("1", monomToString(Monom(1.0f, { 0,0,0 }))); // Constant 1
    ASSERT_EQ("-1", monomToString(Monom(-1.0f, { 0,0,0 })));// Constant -1
    ASSERT_EQ("x^-1", monomToString(Monom(1.0f, { -1,0,0 })));
    ASSERT_EQ("2y^-2", monomToString(Monom(2.0f, { 0,-2,0 })));
}


// --- Тесты для Polynom ---

TEST(PolynomTest, DefaultConstructor) {
    Polynom p;
    ASSERT_EQ("0", polynomToString(p));
}

TEST(PolynomTest, ConstructorWithList) {
    CyclicList<Monom> cl;
    cl.push_back(Monom(2.0f, { 1,0,0 })); // 2x
    cl.push_back(Monom(3.0f, { 0,1,0 })); // 3y
    cl.push_back(Monom(1.0f, { 1,0,0 })); // x (should combine)
    Polynom p(cl);
    // Simplify should sort and combine. x > y in sorting.
    ASSERT_EQ("3x+3y", polynomToString(p));
}

TEST(PolynomTest, AddMonom) {
    Polynom p;
    p.addMonom(Monom(3.0f, { 0,1,0 })); // 3y
    ASSERT_EQ("3y", polynomToString(p));
    p.addMonom(Monom(2.0f, { 1,0,0 })); // 2x
    ASSERT_EQ("2x+3y", polynomToString(p)); // Sorted: x then y
    p.addMonom(Monom(1.0f, { 0,1,0 })); // y
    ASSERT_EQ("2x+4y", polynomToString(p)); // Combined and sorted
    p.addMonom(Monom(-2.0f, { 1,0,0 })); // -2x
    ASSERT_EQ("4y", polynomToString(p)); // 2x cancelled out
    p.addMonom(Monom(-4.0f, { 0,1,0 })); // -4y
    ASSERT_EQ("0", polynomToString(p));   // All cancelled out
    p.addMonom(Monom(0.0f, { 5,5,5 })); // Adding zero monom
    ASSERT_EQ("0", polynomToString(p));
}

TEST(PolynomTest, DeleteMonom) {
    Polynom p;
    p.addMonom(Monom(1.0f, { 2,0,0 })); // x^2
    p.addMonom(Monom(2.0f, { 0,1,0 })); // 2y
    p.addMonom(Monom(3.0f, { 0,0,1 })); // 3z
    // After simplify & sort: x^2 + 2y + 3z
    // Indices: 0: x^2, 1: 2y, 2: 3z
    ASSERT_EQ("x^2+2y+3z", polynomToString(p));

    p.deleteMonom(1); // Delete 2y
    ASSERT_EQ("x^2+3z", polynomToString(p));

    p.deleteMonom(1); // Delete 3z (it's now at index 1)
    ASSERT_EQ("x^2", polynomToString(p));

    p.deleteMonom(0); // Delete x^2
    ASSERT_EQ("0", polynomToString(p));

    // Test deleting from empty or out of bounds (depends on CyclicList behavior)
    // Assuming CyclicList::erase might throw std::out_of_range or handle gracefully.
    // If it throws, these tests would use ASSERT_THROW.
    // If it handles (e.g., no-op), these are fine.
    Polynom p_empty;
    // p_empty.deleteMonom(0); // This would likely cause issues if list is empty.
    // Let's assume CyclicList handles this or it's UB for Polynom.
    // For now, we'll skip testing delete on empty or invalid index if not specified by CyclicList.
}

TEST(PolynomTest, AdditionPolynom) {
    Polynom p1;
    p1.addMonom(Monom(2.0f, { 1,0,0 })); // 2x
    p1.addMonom(Monom(1.0f, { 0,1,0 })); // y

    Polynom p2;
    p2.addMonom(Monom(3.0f, { 1,0,0 })); // 3x
    p2.addMonom(Monom(-1.0f, { 0,1,0 })); // -y

    Polynom sum = p1 + p2; // (2x+y) + (3x-y) = 5x
    ASSERT_EQ("5x", polynomToString(sum));

    Polynom p3; // Empty
    Polynom sum_with_empty = p1 + p3;
    ASSERT_EQ("2x+y", polynomToString(sum_with_empty));

    Polynom sum_empty_with_p1 = p3 + p1;
    ASSERT_EQ("2x+y", polynomToString(sum_empty_with_p1));

    p1 += p2;
    ASSERT_EQ("5x", polynomToString(p1));
}

TEST(PolynomTest, AdditionMonom) {
    Polynom p;
    p.addMonom(Monom(2.0f, { 1,0,0 })); // 2x

    Polynom sum = p + Monom(3.0f, { 1,0,0 }); // 2x + 3x = 5x
    ASSERT_EQ("5x", polynomToString(sum));

    Polynom sum2 = p + Monom(1.0f, { 0,1,0 }); // 2x + y
    ASSERT_EQ("2x+y", polynomToString(sum2));

    p += Monom(1.0f, { 0,1,0 }); // p becomes 2x+y
    ASSERT_EQ("2x+y", polynomToString(p));
}

TEST(PolynomTest, SubtractionPolynom) {
    Polynom p1;
    p1.addMonom(Monom(5.0f, { 1,0,0 })); // 5x
    p1.addMonom(Monom(2.0f, { 0,1,0 })); // 2y

    Polynom p2;
    p2.addMonom(Monom(3.0f, { 1,0,0 })); // 3x
    p2.addMonom(Monom(2.0f, { 0,1,0 })); // 2y

    Polynom diff = p1 - p2; // (5x+2y) - (3x+2y) = 2x
    ASSERT_EQ("2x", polynomToString(diff));

    Polynom p3; // Empty
    Polynom diff_with_empty = p1 - p3;
    ASSERT_EQ("5x+2y", polynomToString(diff_with_empty));

    Polynom diff_empty_with_p1 = p3 - p1; // 0 - (5x+2y) = -5x-2y
    ASSERT_EQ("-5x-2y", polynomToString(diff_empty_with_p1));


    p1 -= p2;
    ASSERT_EQ("2x", polynomToString(p1));
}

TEST(PolynomTest, OutputStream) {
    Polynom p;
    ASSERT_EQ("0", polynomToString(p));

    p.addMonom(Monom(3.0f, { 1,0,0 })); // 3x
    ASSERT_EQ("3x", polynomToString(p));

    p.addMonom(Monom(2.0f, { 0,1,0 })); // 2y
    ASSERT_EQ("3x+2y", polynomToString(p)); // Sorted

    p.addMonom(Monom(-1.0f, { 0,0,1 })); // -z
    ASSERT_EQ("3x+2y-z", polynomToString(p));

    p.addMonom(Monom(-3.0f, { 1,0,0 })); // -3x (cancels out 3x)
    ASSERT_EQ("2y-z", polynomToString(p));

    Polynom p2;
    p2.addMonom(Monom(-5.0f, { 0,0,2 })); // -5z^2
    ASSERT_EQ("-5z^2", polynomToString(p2));
    p2.addMonom(Monom(1.0f, { 1,0,0 })); // x
    ASSERT_EQ("-5z^2+x", polynomToString(p2));
}


TEST(PolynomTest, AssignmentFromString_SingleMonomial) {
    Polynom p;
    std::string s;

    s = "0";
    p = s;
    ASSERT_EQ("0", polynomToString(p));

    s = "5";
    p = s;
    ASSERT_EQ("5", polynomToString(p));

    s = "-2.5";
    p = s;
    ASSERT_EQ("-2.5", polynomToString(p));

    s = "x";
    p = s;
    ASSERT_EQ("x", polynomToString(p));

    s = "  x  "; // With spaces
    p = s;
    ASSERT_EQ("x", polynomToString(p));

    s = "-y";
    p = s;
    ASSERT_EQ("-y", polynomToString(p));

    s = "z^3";
    p = s;
    ASSERT_EQ("z^3", polynomToString(p));

    s = "2.5x^2";
    p = s;
    ASSERT_EQ("2.5x^2", polynomToString(p));

    s = "-3y^-1"; // Negative power
    p = s;
    ASSERT_EQ("-3y^-1", polynomToString(p));

    s = "xyz";
    p = s;
    ASSERT_EQ("xyz", polynomToString(p));

    s = "x^1y^1z^1"; // Explicit powers of 1
    p = s;
    ASSERT_EQ("xyz", polynomToString(p));

    s = "  -  10  x ^ 2  y ^ -3  z  "; // Complex with spaces
    p = s;
    ASSERT_EQ("-10x^2y^-3z", polynomToString(p));

    s = "+5x"; // Leading plus
    p = s;
    ASSERT_EQ("5x", polynomToString(p));

    s = "x^0"; // Power of zero
    p = s;
    ASSERT_EQ("1", polynomToString(p)); // x^0 = 1

    s = "5x^0";
    p = s;
    ASSERT_EQ("5", polynomToString(p));

    s = "0x"; // Coefficient is zero
    p = s;
    ASSERT_EQ("0", polynomToString(p));

    s = "0x^5y^2";
    p = s;
    ASSERT_EQ("0", polynomToString(p));
}

TEST(PolynomTest, ComplexScenarioSimplification) {
    Polynom p;
    p.addMonom(Monom(1.0f, { 1,0,0 })); // x
    p.addMonom(Monom(2.0f, { 0,1,0 })); // 2y
    p.addMonom(Monom(-1.0f, { 1,0,0 })); // -x
    p.addMonom(Monom(3.0f, { 0,0,1 })); // 3z
    p.addMonom(Monom(-2.0f, { 0,1,0 })); // -2y
    p.addMonom(Monom(5.0f, { 2,0,0 })); // 5x^2
    p.addMonom(Monom(-3.0f, { 0,0,1 })); // -3z
    // x - x = 0
    // 2y - 2y = 0
    // 3z - 3z = 0
    // Result should be 5x^2
    ASSERT_EQ("5x^2", polynomToString(p));

    p.addMonom(Monom(-5.0f, { 2,0,0 })); // -5x^2
    ASSERT_EQ("0", polynomToString(p)); // Should be completely zero
}