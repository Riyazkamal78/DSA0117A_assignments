#include "../Common/metro.hpp"
#include <iostream>
int main() {
    metro::Passenger student{metro::Category::Student};
    metro::Journey j{metro::Zone::Outer, 9, student};
    metro::FareTable table;
    metro::SmartCard card{10000};
    auto f = table.quote(j);
    bool ok = card.deduct(f);
    std::cout << "Student outer peak fare: " << metro::money(f)
              << "\nJourney allowed: " << std::boolalpha << ok
              << "\nBalance: " << metro::money(card.balance()) << '\n';
}
