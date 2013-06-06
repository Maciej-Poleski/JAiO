//Maciej Poleski

#include "RegularLanguage.h"

#include <stdexcept>
#include <iostream>

int main(int argc, char** argv)
{
    RegularLanguage a("(((a*)*)*)");
    std::cout << a.ToString() << std::endl; // wypisuje "a*"
    try {
        RegularLanguage b("(((a*)*)*"); // rzuca wyjątkiem typu std::invalid_argument
        std::cout<<"Błąd - oczekiwano wyjątku\n";
    } catch(const std::invalid_argument &e)
    {
        std::cout<<"Zgodnie z oczekiwaniami: "<<e.what()<<'\n';
    }
    RegularLanguage b("b+0*");
    std::cout<<b.ToString()<<'\n';
    {
        std::cout<<RegularLanguage("aaaabbbb(ab)(ab)*a1(1+1)1(1)b").ToString()<<'\n';
    }
    RegularLanguage c = a*b;
    std::cout << c.ToString() << std::endl; // wypisuje "a*(b+1)"
    RegularLanguage d = *c;
    std::cout << d.ToString() << std::endl; // wypisuje "(a*(b+1))*"
    std::cout<<RegularLanguage("(a*b)*(a*(a*b)*)*").ToString()<<'\n';
    std::cout << (d == RegularLanguage("(a*b)*(a*(a*b)*)*")) << std::endl; // wypisuje "1"
//     std::cout << (RegularLanguage("bb") <= RegularLanguage("(a+b(ab*a)*b)*")) << std::endl; // wypisuje "1"
    return 0;
}


