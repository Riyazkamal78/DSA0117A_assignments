#include "../Common/metro.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <optional>
std::optional<int> readHour() {
    for (;;) {
        std::cout << "Enter hour (0-23):\n";
        int hour{};
        if (!(std::cin >> hour)) {
            if (std::cin.eof()) return std::nullopt;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid number; try again.\n";
            continue;
        }
        std::string tail;
        std::getline(std::cin, tail);
        bool clean = std::all_of(tail.begin(), tail.end(),
            [](unsigned char c) { return std::isspace(c) != 0; });
        if (!clean || hour < 0 || hour > 23) {
            std::cout << "Invalid hour; try again.\n";
            continue;
        }
        return hour;
    }
}
int main() {
    auto h = readHour();
    if (!h) { std::cout << "Input ended.\n"; return 0; }
    metro::FareTable table;
    metro::SmartCard card{10000};
    auto fare = table.quote({metro::Zone::Middle, *h, {}});
    bool ok = card.deduct(fare);
    std::cout << "ZONE    HOUR   FARE       BALANCE\n"
              << "Middle  " << *h << "     " << metro::money(fare)
              << "   " << metro::money(card.balance()) << '\n'
              << "Allowed: " << std::boolalpha << ok << '\n';
    std::cout << std::fixed << std::setprecision(2)
              << "Display-only double: " << static_cast<double>(fare) / 100.0
              << '\n';
}
