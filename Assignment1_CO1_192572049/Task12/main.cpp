#include "../Common/metro.hpp"
#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
namespace revised {
using metro::Money;
enum class Zone { Inner, Middle, Outer, Airport };
constexpr std::array<Money, 4> FARES{1000, 2000, 3000, 6000};
constexpr int WEEKEND_DISCOUNT{10};
int index(Zone z) {
    int i = static_cast<int>(z);
    if (i < 0 || i >= static_cast<int>(FARES.size()))
        throw std::invalid_argument("unknown revised zone");
    return i;
}
struct Trip {
    Zone from, to;
    int hour;
    bool weekend;
    int month; // YYYYMM, 1..12 months
    metro::Category category{metro::Category::Adult};
};
void validMonth(int m) {
    if (m < 200001 || m > 999912 || m % 100 < 1 || m % 100 > 12)
        throw std::invalid_argument("invalid month");
}
class Pass {
    Zone first_, second_;
    int month_;
public:
    Pass(Zone a, Zone b, int m) : first_{a}, second_{b}, month_{m} {
        index(a); index(b); validMonth(m);
        if (a == b) throw std::invalid_argument("choose distinct zones");
    }
    bool covers(const Trip& t) const {
        if (month_ != t.month) return false;
        int lo = std::min(index(t.from), index(t.to));
        int hi = std::max(index(t.from), index(t.to));
        // Every traversed zone must be one of the two pass zones.
        for (int z = lo; z <= hi; ++z)
            if (z != index(first_) && z != index(second_)) return false;
        return true;
    }
};
class FarePolicy {
public:
    Money quote(const Trip& t, const std::optional<Pass>& pass) const {
        int from = index(t.from), to = index(t.to);
        bool peak = metro::isPeak(t.hour);
        validMonth(t.month);
        int concession = metro::concession(t.category);
        if (pass && pass->covers(t)) return 0;
        Money base = FARES.at(static_cast<std::size_t>(std::max(from, to)));
        int multiplier = t.weekend ? 100 - WEEKEND_DISCOUNT :
                         (peak ? 100 + metro::PEAK_PERCENT : 100);
        Money gross = metro::roundPercent(base, multiplier);
        return metro::roundPercent(gross, 100 - concession);
    }
};
} // namespace revised
int main() {
    using namespace revised;
    FarePolicy policy;
    std::optional<Pass> pass{Pass{Zone::Inner, Zone::Middle, 202611}};
    struct Test { const char* name; Trip trip; bool usePass; Money expected; };
    const Test tests[]{
      {"airport weekday peak", {Zone::Inner,Zone::Airport,8,false,202611},false,7200},
      {"airport weekend peak", {Zone::Inner,Zone::Airport,8,true,202611},false,5400},
      {"airport weekend off", {Zone::Inner,Zone::Airport,10,true,202611},false,5400},
      {"covered monthly pass", {Zone::Inner,Zone::Middle,8,false,202611},true,0},
      {"outside pass", {Zone::Middle,Zone::Outer,8,false,202611},true,3600},
      {"expired pass", {Zone::Inner,Zone::Middle,10,false,202612},true,2000},
      {"student weekend", {Zone::Inner,Zone::Airport,8,true,202611,
                             metro::Category::Student},false,4860},
      {"senior weekday peak", {Zone::Inner,Zone::Airport,8,false,202611,
                                 metro::Category::Senior},false,5760}};
    int failures = 0;
    for (const auto& t : tests) {
        Money actual = policy.quote(t.trip, t.usePass ? pass : std::nullopt);
        bool ok = actual == t.expected;
        failures += !ok;
        std::cout << t.name << " expected=" << t.expected << " actual="
                  << actual << (ok ? " PASS\n" : " FAIL\n");
    }
    metro::SmartCard low{4999}, exact{5000};
    bool reserveOK = !low.deduct(0) && exact.deduct(0);
    failures += !reserveOK;
    std::cout << "Pass reserve check: " << (reserveOK ? "PASS" : "FAIL") << '\n';
    Pass separated{Zone::Inner, Zone::Outer, 202611};
    bool gapOK = !separated.covers({Zone::Inner,Zone::Outer,8,false,202611});
    failures += !gapOK;
    std::cout << "Intervening zone check: " << (gapOK ? "PASS" : "FAIL") << '\n';
    bool invalidOK = false;
    try { policy.quote({static_cast<Zone>(9),Zone::Inner,8,false,202611},pass); }
    catch (const std::invalid_argument&) { invalidOK = true; }
    failures += !invalidOK;
    std::cout << "Invalid revised zone: " << (invalidOK ? "PASS" : "FAIL") << '\n';
    std::cout << "Checks: 11 Failures: " << failures << '\n';
    return failures == 0 ? 0 : 1;
}
