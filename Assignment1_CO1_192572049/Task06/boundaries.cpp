#include "../Common/metro.hpp"
#include <iostream>
int main() {
    metro::FareTable f;
    for (int h : {8, 10, 17, 20})
        std::cout << "Hour " << h << " peak=" << std::boolalpha
                  << metro::isPeak(h) << " fare="
                  << metro::money(f.quote({metro::Zone::Outer, h, {}}))
                  << '\n';
}
