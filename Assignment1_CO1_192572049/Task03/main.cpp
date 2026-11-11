#include <climits>
#include <iomanip>
#include <iostream>
int main() {
    std::cout << "Type sizes in bytes\n"
      << "bool " << sizeof(bool) << " char " << sizeof(char)
      << " int " << sizeof(int) << " long long " << sizeof(long long)
      << "\nunsigned " << sizeof(unsigned) << " float " << sizeof(float)
      << " double " << sizeof(double) << '\n';
    auto count = 3000000LL;
    std::cout << "auto count " << sizeof(count) << '\n'
      << "CHAR_BIT " << CHAR_BIT << " CHAR " << CHAR_MIN << ".." << CHAR_MAX
      << "\nINT " << INT_MIN << ".." << INT_MAX
      << "\nUINT 0.." << UINT_MAX
      << "\nLLONG " << LLONG_MIN << ".." << LLONG_MAX << '\n';
    std::cout << std::setprecision(17)
      << "0.1 + 0.2 = " << 0.1 + 0.2 << '\n'
      << "10 + 20 paise = " << 10LL + 20LL << '\n'
      << "Network peak total paise: " << count * 3600LL << '\n'
      << "Fits int: " << std::boolalpha
      << (count * 3600LL <= INT_MAX) << '\n';
}
