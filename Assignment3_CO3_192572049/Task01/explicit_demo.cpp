#include <iostream>
#include <string>
#ifndef EXPLICIT
#define EXPLICIT
#endif
class Book {
    std::string title;
public:
    EXPLICIT Book(std::string t) : title(t) {}
    void print() const { std::cout << title << '\n'; }
};
int main() { Book b = std::string("Gift"); b.print(); }
