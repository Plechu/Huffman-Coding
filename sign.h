#ifndef SIGN_H
#define SIGN_H
#pragma once
#include <vector>
class Sign {
    char sign;
    unsigned int number_of_occurrence;
    std::vector<bool> code;
    bool internal_node;
    Sign* left;
    Sign* right;
public:
    Sign(char z, unsigned int i_w, std::vector<bool> c, bool i_n, Sign* l, Sign* r) : sign(z), number_of_occurrence(i_w), code(c), internal_node(i_n), left(l), right(r) {}
    Sign() : sign(0), number_of_occurrence(0), code(0), internal_node(0), left(nullptr), right(nullptr) {}

    bool operator==(const char z) const {return sign == z;} // do porownania znakow
    Sign& operator++() { number_of_occurrence++; return *this; } // ++ dla zwiekszenia liczby wystapien
    // akcesory
    void setSign(const char z) { sign = z; number_of_occurrence = 1; } // ustawienie znaku
    void setNumber_of_occurrence(unsigned int x) { number_of_occurrence = x; } // ustawienie ilosci wystepowania
    void setCode(const std::vector<bool> z) { code = z; } // ustawianie kodu
    void setInternal_node(const bool i_n) { internal_node = i_n; } // ustawienie wezla jako wewnetrzny
    void setLeft(Sign* x) { left = x; } // ustawienie lewego syna
    void setRight(Sign* x) { right = x; } // ustawienie prawego syna

    unsigned int getNumber_of_occurrence() const { return number_of_occurrence; } // pobranie ilosci wystapienia
    char getSign() const {return sign;} // pobranie znaku
    std::vector<bool>& getCode() { return code; } // pobranie kodu Huffmana
    bool getInternal_node() const { return internal_node; } // sprawdzenie czy wezel jest wewnetrzny
    Sign* getLeft_pointer() const { return left; } // pobranie adresu lewego syna
    Sign* getRight_pointer() const { return right; } //pobranie adresu prawego syna
};

#endif // SIGN_H
