#include "../Common/metro.hpp"
#include <iostream>
int main() {
    static_assert(metro::BASE.size() == 3);
    static_assert(metro::MIN_BALANCE == 5000);
    for (std::size_t i = 0; i < metro::BASE.size(); ++i)
        std::cout << "Zone " << i << ": "
                  << metro::money(metro::BASE[i]) << '\n';
    std::cout << "Peak surcharge: " << metro::PEAK_PERCENT << "%\n"
              << "Minimum remaining: "
              << metro::money(metro::MIN_BALANCE) << '\n';
}
