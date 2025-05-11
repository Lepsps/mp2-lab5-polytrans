#include "cyclic_list.h"
#include "polynoms.h"

#include <gtest.h>

#include <string>
#include <vector>
#include <cmath>

#include <algorithm>
#include <random>

#include <sstream>
#include <stdexcept>


TEST(MonomTest, ConstructorsAndZeroRepresentation) {
    Monom m_def;
    ASSERT_FLOAT_EQ(0.0f, m_def.ratio);
    ASSERT_EQ(std::vector<int>({ 0, 0, 0 }), m_def.powers);

    Monom m_ratio(5.5f);
    ASSERT_FLOAT_EQ(5.5f, m_ratio.ratio);
    ASSERT_EQ(std::vector<int>({ 0, 0, 0 }), m_ratio.powers);

    Monom m_powers(0.0f, { 1, 2, 3 });
    ASSERT_FLOAT_EQ(0.0f, m_powers.ratio);
    ASSERT_EQ(std::vector<int>({ 0, 0, 0 }), m_powers.powers);

    Monom m_full(-3.0f, { 1, 0, 2 });
    ASSERT_FLOAT_EQ(-3.0f, m_full.ratio);
    ASSERT_EQ(std::vector<int>({ 1, 0, 2 }), m_full.powers);

    Monom m_zero_explicit(0.0f);
    ASSERT_FLOAT_EQ(0.0f, m_zero_explicit.ratio);
    ASSERT_EQ(std::vector<int>({ 0, 0, 0 }), m_zero_explicit.powers);

    ASSERT_EQ(m_def, m_powers);
    ASSERT_EQ(m_def, m_zero_explicit);
}

TEST(MonomTest, EqualityOperators) {
    Monom m1(2.0f, { 1, 2, 0 });
    Monom m2(2.0f, { 1, 2, 0 });
    Monom m3(3.0f, { 1, 2, 0 });
    Monom m4(2.0f, { 1, 3, 0 });
    Monom m0;

    ASSERT_TRUE(m1 == m2);
    ASSERT_FALSE(m1 == m3);
    ASSERT_FALSE(m1 == m4);
    ASSERT_FALSE(m1 == m0);
    ASSERT_TRUE(m0 == Monom(0.0f));

    ASSERT_FALSE(m1 != m2);
    ASSERT_TRUE(m1 != m3);
    ASSERT_TRUE(m1 != m4);
    ASSERT_TRUE(m1 != m0);
}

TEST(MonomTest, ComparisonOperators) {
    Monom m_x5(1.0f, { 5, 0, 0 });
    Monom m_y5(1.0f, { 0, 5, 0 });
    Monom m_z5(1.0f, { 0, 0, 5 });
    Monom m_x4y4z4(1.0f, { 4, 4, 4 });
    Monom m_const(10.0f);
    Monom m_x(1.0f, { 1,0,0 });
    Monom m0;

    ASSERT_TRUE(m_x5 > m_y5);
    ASSERT_TRUE(m_y5 > m_z5);
    ASSERT_TRUE(m_x5 > m_z5);
    ASSERT_TRUE(m_x5 > m_x4y4z4);
    ASSERT_TRUE(m_x4y4z4 > m_x);
    ASSERT_TRUE(m_x > m_const);
    ASSERT_TRUE(m_const > m0);

    ASSERT_FALSE(m_const > m_const);

    ASSERT_TRUE(m_y5 < m_x5);
    ASSERT_TRUE(m_z5 < m_y5);
    ASSERT_TRUE(m_z5 < m_x5);
    ASSERT_TRUE(m_x4y4z4 < m_x5);
    ASSERT_TRUE(m_x < m_x4y4z4);
    ASSERT_TRUE(m_const < m_x);
    ASSERT_TRUE(m0 < m_const);
    ASSERT_FALSE(m_x5 < m_x5);
}

TEST(MonomTest, AdditionOperatorSuccess) {
    Monom m1(2.5f, { 1, 2, 0 });
    Monom m2(3.0f, { 1, 2, 0 });
    Monom expected(5.5f, { 1, 2, 0 });
    ASSERT_EQ(expected, m1 + m2);
    ASSERT_EQ(Monom(), m1 + Monom(-2.5f, { 1, 2, 0 }));
}

TEST(MonomTest, AdditionOperatorFailure) {
    Monom m1(2.5f, { 1, 2, 0 });
    Monom m2(3.0f, { 1, 3, 0 });
    ASSERT_THROW(m1 + m2, std::invalid_argument);
}

TEST(MonomTest, SubtractionOperatorSuccess) {
    Monom m1(5.0f, { 1, 2, 0 });
    Monom m2(3.5f, { 1, 2, 0 });
    Monom expected(1.5f, { 1, 2, 0 });
    ASSERT_EQ(expected, m1 - m2);
    ASSERT_EQ(Monom(), m1 - Monom(5.0f, { 1, 2, 0 }));
}

TEST(MonomTest, SubtractionOperatorFailure) {
    Monom m1(2.5f, { 1, 2, 0 });
    Monom m2(3.0f, { 2, 2, 0 });
    ASSERT_THROW(m1 - m2, std::invalid_argument);
}

TEST(MonomTest, MultiplicationOperator) {
    Monom m1(2.0f, { 1, 2, 0 });
    Monom m2(3.0f, { 2, 1, 3 });
    Monom expected(6.0f, { 3, 3, 3 });
    ASSERT_EQ(expected, m1 * m2);

    Monom m3(-1.0f, { 0,0,0 });
    Monom m4(5.0f, { 1,1,1 });
    Monom expected2(-5.0f, { 1,1,1 });
    ASSERT_EQ(expected2, m3 * m4);

    Monom m_zero;
    Monom m5(2.0f, { 1, 5, 2 });
    ASSERT_EQ(m_zero, m5 * m_zero);
    ASSERT_EQ(m_zero, m_zero * m5);
}

TEST(MonomTest, DivisionOperatorSuccess) {
    Monom m1(6.0f, { 3, 3, 3 });
    Monom m2(3.0f, { 2, 1, 3 });
    Monom expected(2.0f, { 1, 2, 0 });
    ASSERT_EQ(expected, m1 / m2);

    Monom m3(5.0f, { 1,1,1 });
    Monom m4(5.0f, { 1,1,1 });
    Monom expected2(1.0f, { 0,0,0 });
    ASSERT_EQ(expected2, m3 / m4);

    Monom m_zero;
    ASSERT_EQ(m_zero, m_zero / m2);
}

TEST(MonomTest, DivisionOperatorFailureByZero) {
    Monom m1(6.0f, { 3, 3, 3 });
    Monom m_zero_ratio(0.0f, { 1, 1, 1 });
    Monom m_const_zero(0.0f);
    ASSERT_THROW(m1 / m_zero_ratio, std::invalid_argument);
    ASSERT_THROW(m1 / m_const_zero, std::invalid_argument);
}

TEST(MonomTest, AdditionAssignmentOperatorSuccess) {
    Monom m1(2.5f, { 1, 2, 0 });
    Monom m2(3.0f, { 1, 2, 0 });
    Monom expected(5.5f, { 1, 2, 0 });
    m1 += m2;
    ASSERT_EQ(expected, m1);
}

TEST(MonomTest, AdditionAssignmentOperatorFailure) {
    Monom m1(2.5f, { 1, 2, 0 });
    Monom m2(3.0f, { 1, 3, 0 });
    ASSERT_THROW(m1 += m2, std::invalid_argument);
}

TEST(MonomTest, SubtractionAssignmentOperatorSuccess) {
    Monom m1(5.0f, { 1, 2, 0 });
    Monom m2(3.5f, { 1, 2, 0 });
    Monom expected(1.5f, { 1, 2, 0 });
    m1 -= m2;
    ASSERT_EQ(expected, m1);
}

TEST(MonomTest, SubtractionAssignmentOperatorFailure) {
    Monom m1(2.5f, { 1, 2, 0 });
    Monom m2(3.0f, { 2, 2, 0 });
    ASSERT_THROW(m1 -= m2, std::invalid_argument);
}

TEST(MonomTest, MultiplicationAssignmentOperator) {
    Monom m1(2.0f, { 1, 2, 0 });
    Monom m2(3.0f, { 2, 1, 3 });
    Monom expected(6.0f, { 3, 3, 3 });
    m1 *= m2;
    ASSERT_EQ(expected, m1);

    Monom m3(5.0f, { 1,2,3 });
    Monom m_zero;
    m3 *= m_zero;
    ASSERT_EQ(m_zero, m3);
}

TEST(MonomTest, DivisionAssignmentOperatorSuccess) {
    Monom m1(6.0f, { 3, 3, 3 });
    Monom m2(3.0f, { 2, 1, 3 });
    Monom expected(2.0f, { 1, 2, 0 });
    m1 /= m2;
    ASSERT_EQ(expected, m1);
}

TEST(MonomTest, DivisionAssignmentOperatorFailureByZero) {
    Monom m1(6.0f, { 3, 3, 3 });
    Monom m_zero(0.0f);
    ASSERT_THROW(m1 /= m_zero, std::invalid_argument);
}

std::string monomToString(const Monom& m) {
    std::stringstream ss;
    ss << m;
    return ss.str();
}

TEST(MonomTest, StreamOutput) {
    ASSERT_EQ("0", monomToString(Monom()));
    ASSERT_EQ("0", monomToString(Monom(0.0f, { 1, 1, 1 })));
    ASSERT_EQ("5", monomToString(Monom(5.0f)));
    ASSERT_EQ("-3.5", monomToString(Monom(-3.5f)));
    ASSERT_EQ("x", monomToString(Monom(1.0f, { 1, 0, 0 })));
    ASSERT_EQ("-y", monomToString(Monom(-1.0f, { 0, 1, 0 })));
    ASSERT_EQ("z", monomToString(Monom(1.0f, { 0, 0, 1 })));
    ASSERT_EQ("-1", monomToString(Monom(-1.0f, { 0, 0, 0 })));
    ASSERT_EQ("1", monomToString(Monom(1.0f, { 0, 0, 0 })));
    ASSERT_EQ("2.5x", monomToString(Monom(2.5f, { 1, 0, 0 })));
    ASSERT_EQ("-4y^3", monomToString(Monom(-4.0f, { 0, 3, 0 })));
    ASSERT_EQ("10z^2", monomToString(Monom(10.0f, { 0, 0, 2 })));
    ASSERT_EQ("7xy", monomToString(Monom(7.0f, { 1, 1, 0 })));
    ASSERT_EQ("-xz", monomToString(Monom(-1.0f, { 1, 0, 1 })));
    ASSERT_EQ("yz", monomToString(Monom(1.0f, { 0, 1, 1 })));
    ASSERT_EQ("3x^2y^3z^4", monomToString(Monom(3.0f, { 2, 3, 4 })));
    ASSERT_EQ("-2x^5yz^2", monomToString(Monom(-2.0f, { 5, 1, 2 })));
    ASSERT_EQ("xy^2z^3", monomToString(Monom(1.0f, { 1, 2, 3 })));
    ASSERT_EQ("-x^3y^2z", monomToString(Monom(-1.0f, { 3, 2, 1 })));
    ASSERT_EQ("15", monomToString(Monom(15.0f, { 0, 0, 0 })));
}

std::string polynomToString(const Polynom& p) {
    std::stringstream ss;
    ss << p;
    return ss.str();
}

void assertPolynomStringEquals(const Polynom& p, const std::string& expected_output) {
    std::string actual = polynomToString(p);
    ASSERT_EQ(expected_output, actual);
}

TEST(PolynomTest, DefaultConstructor) {
    Polynom p;
    assertPolynomStringEquals(p, "0");
}

TEST(PolynomTest, ListConstructorAndSimplify) {
    CyclicList<Monom> list1;
    Monom m_3x(3.0f, { 1, 0, 0 });
    Monom m_2y(2.0f, { 0, 1, 0 });
    list1.push_back(m_3x);
    list1.push_back(m_2y);
    CyclicList<Monom> list1_copy = list1;
    Polynom p1(list1_copy);
    assertPolynomStringEquals(p1, "3x+2y");

    CyclicList<Monom> list2;
    Monom m_x2y3(1.0f, { 2, 3, 0 });
    Monom m_6x(6.0f, { 1, 0, 0 });
    Monom m_n3y(-3.0f, { 0, 1, 0 });
    Monom m_z(1.0f, { 0, 0, 1 });
    list2.push_back(m_6x);
    list2.push_back(m_n3y);
    list2.push_back(m_z);
    list2.push_back(m_x2y3);
    list2.push_back(Monom(0.0f, { 5,5,5 }));
    list2.push_back(Monom(1.0f, { 1, 0, 0 }));
    list2.push_back(Monom(3.0f, { 0, 1, 0 }));
    Polynom p2(list2);
    assertPolynomStringEquals(p2, "x^2y^3+7x+z");

    CyclicList<Monom> list3;
    Polynom p3(list3);
    assertPolynomStringEquals(p3, "0");

    CyclicList<Monom> list4;
    list4.push_back(Monom(5.0f, { 1, 1, 1 }));
    list4.push_back(Monom(-5.0f, { 1, 1, 1 }));
    Polynom p4(list4);
    assertPolynomStringEquals(p4, "0");
}

TEST(PolynomTest, CopyConstructor) {
    CyclicList<Monom> list;
    Monom m_xyz(1.0f, { 1, 1, 1 });
    Monom m_n2(-2.0f);
    list.push_back(m_xyz);
    list.push_back(m_n2);
    Polynom p1(list);
    Polynom p2(p1);

    assertPolynomStringEquals(p1, "xyz-2");
    assertPolynomStringEquals(p2, "xyz-2");

    p2.addMonom(Monom(3.0f));
    assertPolynomStringEquals(p1, "xyz-2");
    assertPolynomStringEquals(p2, "xyz+1");
}

TEST(PolynomTest, AssignmentOperator) {
    CyclicList<Monom> list1; list1.push_back(Monom(1.0f, { 1,0,0 }));
    Polynom p1(list1);
    CyclicList<Monom> list2; list2.push_back(Monom(2.0f, { 0,1,0 })); list2.push_back(Monom(3.0f, { 0,0,2 }));
    Polynom p2(list2);

    assertPolynomStringEquals(p1, "x");
    assertPolynomStringEquals(p2, "3z^2+2y");

    p1 = p2;
    assertPolynomStringEquals(p1, "3z^2+2y");
    assertPolynomStringEquals(p2, "3z^2+2y");

    p1.addMonom(Monom(-2.0f, { 0,1,0 }));
    assertPolynomStringEquals(p1, "3z^2");
    assertPolynomStringEquals(p2, "3z^2+2y");
}

TEST(PolynomTest, AddMonom) {
    Polynom p;
    assertPolynomStringEquals(p, "0");

    p.addMonom(Monom(5.0f, { 2, 0, 0 }));
    assertPolynomStringEquals(p, "5x^2");

    p.addMonom(Monom(-1.0f, { 0, 0, 3 }));
    assertPolynomStringEquals(p, "-z^3+5x^2");

    p.addMonom(Monom(-5.0f, { 2, 0, 0 }));
    assertPolynomStringEquals(p, "-z^3");

    p.addMonom(Monom(1.0f, { 0, 0, 3 }));
    assertPolynomStringEquals(p, "0");

    p.addMonom(Monom(0.0f, { 1,1,1 }));
    assertPolynomStringEquals(p, "0");
}

TEST(PolynomTest, DeleteMonom) {
    CyclicList<Monom> list;
    Monom m1(3.0f, { 0,0,2 });
    Monom m2(2.0f, { 1,0,0 });
    Monom m3(-1.0f, { 0,1,0 });
    list.push_back(m1); list.push_back(m2); list.push_back(m3);
    Polynom p(list);
    assertPolynomStringEquals(p, "3z^2+2x-y");

    p.deleteMonom(1);
    assertPolynomStringEquals(p, "3z^2-y");

    CyclicList<Monom> list_b = list;
    Polynom p_b(list_b);
    assertPolynomStringEquals(p_b, "3z^2+2x-y");

    p_b.deleteMonom(0);
    assertPolynomStringEquals(p_b, "2x-y");

    CyclicList<Monom> list_c = list;
    Polynom p_c(list_c);
    assertPolynomStringEquals(p_c, "3z^2+2x-y");

    p_c.deleteMonom(2);
    assertPolynomStringEquals(p_c, "3z^2+2x");

    CyclicList<Monom> list_d; list_d.push_back(m1);
    Polynom p_d(list_d);
    assertPolynomStringEquals(p_d, "3z^2");
    p_d.deleteMonom(0);
    assertPolynomStringEquals(p_d, "0");

    Polynom p_empty;
    ASSERT_THROW(p_empty.deleteMonom(0), std::out_of_range);

    CyclicList<Monom> list_e = list;
    Polynom p_e(list_e);
    ASSERT_THROW(p_e.deleteMonom(3), std::out_of_range);
    assertPolynomStringEquals(p_e, "3z^2+2x-y");
}

TEST(PolynomTest, AdditionOperatorPolynom) {
    CyclicList<Monom> l1; l1.push_back(Monom(2.0f, { 1,0,0 })); l1.push_back(Monom(3.0f, { 0,0,2 }));
    Polynom p1(l1);
    CyclicList<Monom> l2; l2.push_back(Monom(1.0f, { 1,0,0 })); l2.push_back(Monom(-1.0f, { 0,1,0 })); l2.push_back(Monom(5.0f, { 0,0,1 }));
    Polynom p2(l2);
    Polynom p_empty;
    CyclicList<Monom> l3; l3.push_back(Monom(-2.0f, { 1,0,0 })); l3.push_back(Monom(-3.0f, { 0,0,2 }));
    Polynom p3(l3);

    Polynom sum1 = p1 + p2;
    assertPolynomStringEquals(sum1, "3z^2+3x-y+5z");

    Polynom sum2 = p1 + p3;
    assertPolynomStringEquals(sum2, "0");

    Polynom sum3 = p1 + p_empty;
    assertPolynomStringEquals(sum3, "3z^2+2x");

    Polynom sum4 = p_empty + p2;
    assertPolynomStringEquals(sum4, "x-y+5z");
}

TEST(PolynomTest, AdditionOperatorMonom) {
    CyclicList<Monom> l1; l1.push_back(Monom(2.0f, { 1,0,0 })); l1.push_back(Monom(3.0f, { 0,1,0 }));
    Polynom p1(l1);
    Monom m_4x(4.0f, { 1, 0, 0 });
    Monom m_n3y(-3.0f, { 0, 1, 0 });
    Monom m_z2(1.0f, { 0, 0, 2 });
    Monom m_zero;

    Polynom sum1 = p1 + m_4x;
    assertPolynomStringEquals(sum1, "6x+3y");

    Polynom sum2 = p1 + m_n3y;
    assertPolynomStringEquals(sum2, "2x");

    Polynom sum3 = p1 + m_z2;
    assertPolynomStringEquals(sum3, "z^2+2x+3y");

    Polynom sum4 = p1 + m_zero;
    assertPolynomStringEquals(sum4, "2x+3y");
}

TEST(PolynomTest, SubtractionOperatorPolynom) {
    CyclicList<Monom> l1; l1.push_back(Monom(2.0f, { 1,0,0 })); l1.push_back(Monom(3.0f, { 0,1,0 }));
    Polynom p1(l1);
    CyclicList<Monom> l2; l2.push_back(Monom(1.0f, { 1,0,0 })); l2.push_back(Monom(-1.0f, { 0,1,0 })); l2.push_back(Monom(5.0f, { 0,0,2 }));
    Polynom p2(l2);
    Polynom p_empty;

    Polynom diff1 = p1 - p2;
    assertPolynomStringEquals(diff1, "-5z^2+x+4y");

    Polynom diff2 = p1 - p1;
    assertPolynomStringEquals(diff2, "0");

    Polynom diff3 = p_empty - p1;
    assertPolynomStringEquals(diff3, "-2x-3y");
}

TEST(PolynomTest, MultiplicationOperatorPolynom) {
    CyclicList<Monom> l_x_1; l_x_1.push_back(Monom(1.0f, { 1,0,0 })); l_x_1.push_back(Monom(1.0f));
    Polynom p_x_1(l_x_1);
    CyclicList<Monom> l_y_2; l_y_2.push_back(Monom(1.0f, { 0,1,0 })); l_y_2.push_back(Monom(-2.0f));
    Polynom p_y_2(l_y_2);
    Polynom p_empty;
    CyclicList<Monom> l_const; l_const.push_back(Monom(3.0f));
    Polynom p_const(l_const);

    Polynom prod1 = p_x_1 * p_y_2;
    assertPolynomStringEquals(prod1, "xy-2x+y-2");

    Polynom prod2 = p_x_1 * p_empty;
    assertPolynomStringEquals(prod2, "0");

    Polynom prod3 = p_empty * p_y_2;
    assertPolynomStringEquals(prod3, "0");

    Polynom prod4 = p_x_1 * p_const;
    assertPolynomStringEquals(prod4, "3x+3");

    Polynom prod5 = p_const * p_y_2;
    assertPolynomStringEquals(prod5, "3y-6");
}

TEST(PolynomTest, AdditionAssignmentOperatorPolynom) {
    CyclicList<Monom> l1; l1.push_back(Monom(2.0f, { 1,0,0 })); l1.push_back(Monom(3.0f, { 0,1,0 }));
    Polynom p1(l1);
    CyclicList<Monom> l2; l2.push_back(Monom(1.0f, { 1,0,0 })); l2.push_back(Monom(-1.0f, { 0,1,0 }));
    Polynom p2(l2);

    p1 += p2;
    assertPolynomStringEquals(p1, "3x+2y");
    assertPolynomStringEquals(p2, "x-y");
}

TEST(PolynomTest, AdditionAssignmentOperatorMonom) {
    CyclicList<Monom> l1; l1.push_back(Monom(2.0f, { 1,0,0 })); l1.push_back(Monom(3.0f, { 0,1,0 }));
    Polynom p1(l1);
    Monom m_5z2(5.0f, { 0, 0, 2 });

    p1 += m_5z2;
    assertPolynomStringEquals(p1, "5z^2+2x+3y");
}

TEST(PolynomTest, SubtractionAssignmentOperatorPolynom) {
    CyclicList<Monom> l1; l1.push_back(Monom(2.0f, { 1,0,0 })); l1.push_back(Monom(3.0f, { 0,1,0 }));
    Polynom p1(l1);
    CyclicList<Monom> l2; l2.push_back(Monom(1.0f, { 1,0,0 })); l2.push_back(Monom(-1.0f, { 0,1,0 }));
    Polynom p2(l2);

    p1 -= p2;
    assertPolynomStringEquals(p1, "x+4y");
    assertPolynomStringEquals(p2, "x-y");
}


TEST(PolynomTest, MultiplicationAssignmentOperatorPolynom) {
    CyclicList<Monom> l1; l1.push_back(Monom(2.0f, { 1,0,0 })); l1.push_back(Monom(3.0f, { 0,1,0 }));
    Polynom p1(l1);
    CyclicList<Monom> l_x2; l_x2.push_back(Monom(1.0f, { 2,0,0 }));
    Polynom p_x2(l_x2);

    p1 *= p_x2;
    assertPolynomStringEquals(p1, "2x^3+3x^2y");
    assertPolynomStringEquals(p_x2, "x^2");
}

TEST(PolynomTest, StreamOutputComprehensive) {
    Polynom p_empty;
    assertPolynomStringEquals(p_empty, "0");

    CyclicList<Monom> l_const; l_const.push_back(Monom(5.0f));
    Polynom p_const(l_const);
    assertPolynomStringEquals(p_const, "5");

    CyclicList<Monom> l_neg_const; l_neg_const.push_back(Monom(-2.5f));
    Polynom p_neg_const(l_neg_const);
    assertPolynomStringEquals(p_neg_const, "-2.5");

    CyclicList<Monom> l_x; l_x.push_back(Monom(1.0f, { 1,0,0 }));
    Polynom p_x(l_x);
    assertPolynomStringEquals(p_x, "x");

    CyclicList<Monom> l_neg_y; l_neg_y.push_back(Monom(-1.0f, { 0,1,0 }));
    Polynom p_neg_y(l_neg_y);
    assertPolynomStringEquals(p_neg_y, "-y");

    CyclicList<Monom> l_multi;
    l_multi.push_back(Monom(3.0f, { 2,0,0 }));
    l_multi.push_back(Monom(-1.0f, { 0,1,1 }));
    l_multi.push_back(Monom(4.0f));
    Polynom p_multi(l_multi);
    assertPolynomStringEquals(p_multi, "3x^2-yz+4");

    CyclicList<Monom> l_simplify;
    l_simplify.push_back(Monom(5.0f, { 1,0,0 }));
    l_simplify.push_back(Monom(2.0f));
    l_simplify.push_back(Monom(-5.0f, { 1,0,0 }));
    l_simplify.push_back(Monom(-1.0f));
    Polynom p_simplify(l_simplify);
    assertPolynomStringEquals(p_simplify, "1");
}


TEST(PolynomStringAssignmentTest, AssignEmptyString) {
    Polynom p;
    p.addMonom(Monom(5.0f, { 1,0,0 }));
    std::string s = "";
    p = s;
    assertPolynomStringEquals(p, "0");
}

TEST(PolynomStringAssignmentTest, AssignZeroString) {
    Polynom p;
    p.addMonom(Monom(5.0f, { 1,0,0 }));
    std::string s = "0";
    p = s;
    assertPolynomStringEquals(p, "0");
}

TEST(PolynomStringAssignmentTest, AssignSingleConstant) {
    Polynom p;
    std::string s_pos = "12.5";
    p = s_pos;
    assertPolynomStringEquals(p, "12.5");

    std::string s_neg = "-7";
    p = s_neg;
    assertPolynomStringEquals(p, "-7");

    std::string s_plus = "+3";
    p = s_plus;
    assertPolynomStringEquals(p, "3");
}

TEST(PolynomStringAssignmentTest, AssignSingleVariable) {
    Polynom p;
    std::string s_x = "x";
    p = s_x;
    assertPolynomStringEquals(p, "x");

    std::string s_neg_y = "-y";
    p = s_neg_y;
    assertPolynomStringEquals(p, "-y");

    std::string s_plus_z = "+z";
    p = s_plus_z;
    assertPolynomStringEquals(p, "z");
}

TEST(PolynomStringAssignmentTest, AssignSingleVariableCoefficient) {
    Polynom p;
    std::string s_3x = "3x";
    p = s_3x;
    assertPolynomStringEquals(p, "3x");

    std::string s_n2y = "-2.5y";
    p = s_n2y;
    assertPolynomStringEquals(p, "-2.5y");

    std::string s_1z = "1z";
    p = s_1z;
    assertPolynomStringEquals(p, "z");

    std::string s_n1x = "-1x";
    p = s_n1x;
    assertPolynomStringEquals(p, "-x");

    std::string s_p1y = "+1y";
    p = s_p1y;
    assertPolynomStringEquals(p, "y");
}

TEST(PolynomStringAssignmentTest, AssignSingleVariablePower) {
    Polynom p;
    std::string s_x2 = "x^2";
    p = s_x2;
    assertPolynomStringEquals(p, "x^2");

    std::string s_y5 = "y^5";
    p = s_y5;
    assertPolynomStringEquals(p, "y^5");

    std::string s_nz3 = "-z^3";
    p = s_nz3;
    assertPolynomStringEquals(p, "-z^3");

    std::string s_px1 = "+x^1";
    p = s_px1;
    assertPolynomStringEquals(p, "x");
}

TEST(PolynomStringAssignmentTest, AssignSingleTermCoefficientPower) {
    Polynom p;
    std::string s_5x3 = "5x^3";
    p = s_5x3;
    assertPolynomStringEquals(p, "5x^3");

    std::string s_n2y2 = "-2y^2";
    p = s_n2y2;
    assertPolynomStringEquals(p, "-2y^2");

    std::string s_1z4 = "1z^4";
    p = s_1z4;
    assertPolynomStringEquals(p, "z^4");

    std::string s_n1x5 = "-1x^5";
    p = s_n1x5;
    assertPolynomStringEquals(p, "-x^5");
}

TEST(PolynomStringAssignmentTest, AssignSingleTermMultipleVariables) {
    Polynom p;
    std::string s_xy = "xy";
    p = s_xy;
    assertPolynomStringEquals(p, "xy");

    std::string s_3x2y = "3x^2y";
    p = s_3x2y;
    assertPolynomStringEquals(p, "3x^2y");

    std::string s_n_yz3 = "-yz^3";
    p = s_n_yz3;
    assertPolynomStringEquals(p, "-yz^3");

    std::string s_full = "5.5x^3yz^2";
    p = s_full;
    assertPolynomStringEquals(p, "5.5x^3yz^2");

    std::string s_yx = "yx";
    p = s_yx;
    assertPolynomStringEquals(p, "xy");
}

TEST(PolynomStringAssignmentTest, AssignMultipleTermsSimple) {
    Polynom p;
    std::string s = "x+y-z";
    p = s;
    assertPolynomStringEquals(p, "x+y-z");

    std::string s2 = "-z+x+y";
    p = s2;
    assertPolynomStringEquals(p, "x+y-z");
}

TEST(PolynomStringAssignmentTest, AssignMultipleTermsComplexOrder) {
    Polynom p;
    std::string s = "5 - y^2 + 3x";
    p = s;
    assertPolynomStringEquals(p, "-y^2+3x+5");
}

TEST(PolynomStringAssignmentTest, AssignMultipleTermsWithSimplification) {
    Polynom p;
    std::string s = "3x^2 - 5y + 2x^2 + z + 8y - z";
    p = s;
    assertPolynomStringEquals(p, "5x^2+3y");

    std::string s2 = "x+x+x-x-x-x";
    p = s2;
    assertPolynomStringEquals(p, "0");
}

TEST(PolynomStringAssignmentTest, AssignMultipleTermsWithSpaces) {
    Polynom p;
    std::string s = "  -4z^5   +  2.5xy  -  10  +xy ";
    p = s;
    assertPolynomStringEquals(p, "-4z^5+3.5xy-10");
}

TEST(PolynomStringAssignmentTest, AssignStringStartsWithPlus) {
    Polynom p;
    std::string s = "+2x - y";
    p = s;
    assertPolynomStringEquals(p, "2x-y");
}

TEST(PolynomStringAssignmentTest, AssignRealisticExample) {
    Polynom p;
    std::string s = "-z + 4x^2y - 5 + y - 2x^2y + 3z";
    p = s;
    assertPolynomStringEquals(p, "2x^2y+y+2z-5");
}

TEST(PolynomStringAssignmentTest, AssignNoCoefficientBeforeVar) {
    Polynom p;
    std::string s = "x^3+y^2";
    p = s;
    assertPolynomStringEquals(p, "x^3+y^2");
}

TEST(PolynomStringAssignmentTest, AssignNoPowerAfterVar) {
    Polynom p;
    std::string s = "2x+3y";
    p = s;
    assertPolynomStringEquals(p, "2x+3y");
}

TEST(PolynomStringAssignmentTest, AssignTermEndsWithString) {
    Polynom p;
    std::string s1 = "5x";
    p = s1;
    assertPolynomStringEquals(p, "5x");

    std::string s2 = "y^4";
    p = s2;
    assertPolynomStringEquals(p, "y^4");

    std::string s3 = "3";
    p = s3;
    assertPolynomStringEquals(p, "3");
}

void assertPolynomStringNotEquals(const Polynom& p, const std::string& unexpected_output) {
    std::string actual = polynomToString(p);
    ASSERT_NE(unexpected_output, actual);
}

TEST(PolynomStringAssignmentBugs, AssignNumberAfterVarNoCaret) {
    Polynom p;
    std::string s = "2x3";
    p = s;
    assertPolynomStringNotEquals(p, "2x");
    assertPolynomStringNotEquals(p, "6x");
    assertPolynomStringEquals(p, "2x^3");

    std::string s2 = "y2";
    p = s2;
    assertPolynomStringEquals(p, "y^2");

    std::string s3 = "10z5 + 1";
    p = s3;
    assertPolynomStringEquals(p, "10z^5+1");
}

TEST(PolynomStringAssignmentBugs, AssignMultipleVarsAdjacent) {
    Polynom p;
    std::string s1 = "xyz";
    p = s1;
    assertPolynomStringEquals(p, "xyz");

    std::string s2 = "3xyz";
    p = s2;
    assertPolynomStringEquals(p, "3xyz");

    std::string s3 = "xy3z";
    p = s3;
    assertPolynomStringEquals(p, "xy^3z");

    std::string s4 = "2yx5z3";
    p = s4;
    assertPolynomStringEquals(p, "2x^5yz^3");
}

TEST(PolynomStringAssignmentBugs, AssignInvalidOrIncompleteTerms) {
    Polynom p;
    std::string s1 = "3*x";
    p = s1;
    assertPolynomStringEquals(p, "3x");

    std::string s2 = "x^";
    p = s2;
    assertPolynomStringEquals(p, "x");

    std::string s3 = "xy^";
    p = s3;
    assertPolynomStringEquals(p, "xy");

    std::string s4 = "x+";
    p = s4;
    assertPolynomStringEquals(p, "x");

    std::string s5 = "+";
    p = s5;
    assertPolynomStringEquals(p, "0");

    std::string s6 = "-";
    p = s6;
    assertPolynomStringEquals(p, "0");
}

TEST(PolynomStringAssignmentBugs, AssignRepeatedSigns) {
    Polynom p;
    std::string s1 = "x + - y";
    p = s1;
    assertPolynomStringEquals(p, "x-y");

    std::string s2 = "x--y";
    p = s2;

    assertPolynomStringEquals(p, "x+y");
    assertPolynomStringNotEquals(p, "x-y");
    assertPolynomStringNotEquals(p, "x");
}

TEST(PolynomStringAssignmentBugs, AssignMixedTermsNoCaret) {
    Polynom p;
    std::string s = "x^2 + 5y3 - z4 + 2x2";
    p = s;
    assertPolynomStringEquals(p, "-z^4+5y^3+3x^2");
}