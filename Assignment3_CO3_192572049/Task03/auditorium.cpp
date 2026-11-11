#include <iostream>
#include <stdexcept>
#include <string>
class Auditorium {
    std::string name;
    int rows, seats, booked = 0;
    char** map;
public:
    Auditorium(std::string n, int r, int s)
        : name(n), rows(r), seats(s), map(nullptr) {
        if (r < 0 || s < 0) throw std::invalid_argument("negative dimensions");
        if (rows > 0) map = new char*[rows]{};
        try {
            for (int i = 0; i < rows; ++i) map[i] = new char[seats]{};
        } catch (...) {
            for (int i = 0; i < rows; ++i) delete[] map[i];
            delete[] map; throw;
        }
        std::cout << '+' << name << " rows=" << rows << " seats=" << seats
                  << " payload=" << bytes() << " bytes\n";
    }
    Auditorium(const Auditorium&) = delete;
    Auditorium& operator=(const Auditorium&) = delete;
    ~Auditorium() {
        for (int i = 0; i < rows; ++i) delete[] map[i];
        delete[] map;
        std::cout << '-' << name << " closing booked=" << booked << '\n';
    }
    bool book(int r, int s) {
        if (r < 0 || r >= rows || s < 0 || s >= seats || map[r][s]) return false;
        map[r][s] = 1; ++booked; return true;
    }
    bool cancel(int r, int s) {
        if (r < 0 || r >= rows || s < 0 || s >= seats || !map[r][s]) return false;
        map[r][s] = 0; --booked; return true;
    }
    std::size_t bytes() const {
        return static_cast<std::size_t>(rows) * sizeof(char*)
             + static_cast<std::size_t>(rows) * seats * sizeof(char);
    }
};
int main() {
    std::cout << std::boolalpha;
    Auditorium local("Stack", 2, 3);
    {
        Auditorium block("Block", 1, 2);
        std::cout << "Book " << block.book(0, 1) << " duplicate " << block.book(0, 1)
                  << " cancel " << block.cancel(0, 1) << '\n';
    }
    Auditorium* heap = new Auditorium("Heap", 2, 2);
    heap->book(1, 1); delete heap;
    Auditorium zero("Zero", 0, 20);
    std::cout << "Zero booking " << zero.book(0, 0) << '\n';
    for (int r : {10, 100, 1000}) {
        Auditorium measurement("Measure", r, 2*r);
    }
}
