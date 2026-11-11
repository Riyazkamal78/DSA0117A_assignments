#include "../Common/metro.hpp"
#include <iostream>
#include <sstream>
#include <vector>
using namespace metro;
int main() {
    FareTable table;
    struct Test { const char* name; Journey j; Money expected; };
    std::vector<Test> tests{
      {"inner off", {Zone::Inner, 11, {}}, 1000},
      {"middle off", {Zone::Middle, 11, {}}, 2000},
      {"outer off", {Zone::Outer, 11, {}}, 3000},
      {"inner peak", {Zone::Inner, 9, {}}, 1200},
      {"middle peak", {Zone::Middle, 9, {}}, 2400},
      {"outer peak", {Zone::Outer, 9, {}}, 3600},
      {"08 boundary", {Zone::Outer, 8, {}}, 3600},
      {"10 boundary", {Zone::Outer, 10, {}}, 3000},
      {"17 boundary", {Zone::Outer, 17, {}}, 3600},
      {"20 boundary", {Zone::Outer, 20, {}}, 3000},
      {"student peak", {Zone::Outer, 9, {Category::Student}}, 3240},
      {"senior peak", {Zone::Outer, 9, {Category::Senior}}, 2880}};
    int failures = 0, checks = 0;
    auto check = [&](const std::string& name, Money expected, Money actual) {
        ++checks;
        if (expected != actual) ++failures;
        std::cout << name << " expected=" << expected << " actual=" << actual
                  << (expected == actual ? " PASS\n" : " FAIL\n");
    };
    for (const auto& t : tests)
        check(t.name, t.expected, table.quote(t.j));
    SmartCard exact{6000}, low{5999};
    check("exact Rs50", 1, exact.deduct(1000));
    check("exact balance", 5000, exact.balance());
    check("one paisa short", 0, low.deduct(1000));
    check("denial unchanged", 5999, low.balance());
    check("negative recharge", 0, low.recharge(-100));
    check("negative fare", 0, low.deduct(-1));
    check("overflow recharge", 0, low.recharge(LLONG_MAX));
    bool rejected = false;
    try { table.quote({static_cast<Zone>(99), 10, {}}); }
    catch (const std::invalid_argument&) { rejected = true; }
    check("unknown zone", 1, rejected);
    rejected = false;
    try { table.quote({Zone::Inner, 24, {}}); }
    catch (const std::invalid_argument&) { rejected = true; }
    check("hour 24", 1, rejected);
    std::istringstream bad{"letters"};
    int hour{};
    bad >> hour;
    check("letters hour fail", 1, bad.fail());
    SmartCard batch{20000};
    const Journey trips[]{{Zone::Inner, 11, {}}, {Zone::Middle, 8, {}},
      {Zone::Outer, 10, {}}, {Zone::Inner, 17, {Category::Student}},
      {Zone::Outer, 20, {Category::Senior}}};
    Money total = 0;
    for (const auto& j : trips) {
        Money f = table.quote(j);
        if (!batch.deduct(f)) return 1;
        total += f;
    }
    check("five trip total", 9880, total);
    check("five trip balance", 10120, batch.balance());
    std::cout << "Checks: " << checks << " Failures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
