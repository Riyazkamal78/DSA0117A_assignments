#include "../Common/metro.hpp"
#include <array>
#include <iostream>
int main() {
    constexpr std::array<bool, 24> peak{
        false,false,false,false,false,false,false,false,
        true,true,false,false,false,false,false,false,
        false,true,true,true,false,false,false,false};
    int mismatches = 0;
    for (int h = 0; h < 24; ++h)
        if (peak.at(static_cast<std::size_t>(h)) != metro::isPeak(h))
            ++mismatches;
    std::cout << "Hours tested: 24\nMismatches: " << mismatches
              << "\nTable bytes: " << sizeof(peak) << '\n';
    return mismatches == 0 ? 0 : 1;
}
