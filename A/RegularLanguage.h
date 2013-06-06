#include <memory>

struct Node;

class RegularLanguage
{
public:
    RegularLanguage(const std::string& regExp); // dopuszczalne symbole: a, b, 0 (język pusty), 1 (singleton słowa pustego),
    // + (suma), * (gwiazdka Kleene'ego), (, ).
    // np. "a*b", "(a+b)b*", "ab***"
    // priorytet operatorów: suma < katenacja < gwiazdka
    // gramatyka dla wyrażeń (plus ewentalne dodatkowe spacje): S ::= S + S | SS | S* | (S) | a | b | 0 | 1
    // dla niepoprawnych argumentów rzuca wyjątek std::invalid_argument

    friend RegularLanguage operator+(RegularLanguage const & a, RegularLanguage const & b); // suma
    friend RegularLanguage operator*(RegularLanguage const & a, RegularLanguage const & b); // katenacja
    friend RegularLanguage operator*(RegularLanguage const & a); // gwiazdka Kleene'ego

    friend bool operator==(RegularLanguage const & a, RegularLanguage const & b);
    friend bool operator!=(RegularLanguage const & a, RegularLanguage const & b);
    friend bool operator<=(RegularLanguage const & a, RegularLanguage const & b); // inkluzja

    std::string ToString() const; // wypisuje wyrażenie opisujące, bez zbędnych nawiasów
    // i maksymalnie uproszczone (a** = a*, a + 0 = a, a1 = a, a0 = 0, 0* = 1, 1* = 1).

private:
    RegularLanguage(std::shared_ptr<const Node> node);
    
private:
    std::shared_ptr<const Node> _node;
};
