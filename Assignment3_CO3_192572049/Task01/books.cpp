#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
class Book {
    std::string title, author;
    int copies;
public:
    Book() : Book("", "Unknown", 0) {}
    explicit Book(std::string t) : Book(std::move(t), "Donated", 1) {}
    Book(std::string t, std::string a, int c)
        : title(std::move(t)), author(std::move(a)), copies(c) {
        if (copies < 0) throw std::invalid_argument("negative copies");
    }
    void print() const {
        std::cout << '[' << title << "] [" << author << "] " << copies << '\n';
    }
};
int main() {
    Book empty, donated("C++ Primer"), full("Algorithms", "Sedgewick", 3);
    empty.print(); donated.print(); full.print();
    Book records[2];
    std::cout << "Default array record: "; records[0].print();
    try { Book invalid("Invalid", "Test", -1); }
    catch (const std::exception& e) { std::cout << "Rejected: " << e.what() << '\n'; }
}
