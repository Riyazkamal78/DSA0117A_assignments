#include <iomanip>
#include <iostream>
class Money {
    long long paise;
public:
    Money(long long rupees = 0, int p = 0) : paise(rupees * 100 + p) {}
    Money operator+(const Money& b) const {
        Money result; result.paise = paise + b.paise; return result;
    }
    bool operator==(const Money& b) const { return paise == b.paise; }
    friend std::ostream& operator<<(std::ostream& out, const Money& m) {
        return out << "Rs " << m.paise/100 << '.'
                   << std::setfill('0') << std::setw(2) << m.paise%100 << std::setfill(' ');
    }
};
int main() {
    Money a(120, 75), b(79, 25);
    Money c = a + b;
    std::cout << c << '\n' << std::boolalpha << (c == Money(200)) << '\n';
}
