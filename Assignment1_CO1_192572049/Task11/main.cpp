#include "../Common/metro.hpp"
#include <fstream>
#include <iostream>
#include <vector>
int main() {
    std::ifstream in{"fares.txt"};
    int zones{};
    if (!(in >> zones) || zones < 1 || zones > 100) return 1;
    std::vector<metro::Money> fares(static_cast<std::size_t>(zones));
    for (auto& f : fares)
        if (!(in >> f) || f <= 0 || f > 1000000) return 1;
    std::string extra;
    if (in >> extra) return 1;
    if (!in.eof()) return 1;
    std::cout << "Validated configuration zones: " << fares.size() << '\n';
    for (std::size_t i = 0; i < fares.size(); ++i)
        std::cout << "Zone " << i << ": " << metro::money(fares[i]) << '\n';
    std::cout << "Startup O(z), quote O(1), table space O(z)\n";
}
