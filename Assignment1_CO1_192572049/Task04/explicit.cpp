#include <iostream>
int main() {
    int a{7}, b{2};
    double c{static_cast<double>(a / b)};
    double d{static_cast<double>(a) / static_cast<double>(b)};
    char ch{static_cast<char>(static_cast<int>('A') + 2)};
    bool flag{static_cast<bool>(5)};
    unsigned int u{1};
    int s{-1};
    std::cout << c << ' ' << d << '\n'
              << ch << ' ' << static_cast<int>(ch) + 1 << '\n'
              << flag << ' ' << std::boolalpha << flag << '\n'
              << (static_cast<unsigned int>(s) < u ? "s < u" : "s >= u")
              << '\n';
    // The cast above preserves the original unsigned comparison.
    // For intended mathematical comparison, widen both operands:
    std::cout << "Corrected comparison: "
      << (static_cast<long long>(s) < static_cast<long long>(u)) << '\n';
}
