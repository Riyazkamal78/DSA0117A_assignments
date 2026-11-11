#pragma once
#include <array>
#include <climits>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
namespace metro {
using Money = long long;
enum class Zone { Inner, Middle, Outer };
enum class Category { Adult, Student, Senior };
inline constexpr std::array<Money, 3> BASE{1000, 2000, 3000};
inline constexpr Money MIN_BALANCE{5000};
inline constexpr int PEAK_PERCENT{20};
inline constexpr int STUDENT_PERCENT{10}; // Assumed concession
inline constexpr int SENIOR_PERCENT{20};  // Assumed concession
inline std::size_t index(Zone z) {
    int n = static_cast<int>(z);
    if (n < 0 || n >= static_cast<int>(BASE.size()))
        throw std::invalid_argument("unknown zone");
    return static_cast<std::size_t>(n);
}
inline bool isPeak(int hour) {
    if (hour < 0 || hour > 23)
        throw std::invalid_argument("invalid hour");
    return (hour >= 8 && hour < 10) ||
           (hour >= 17 && hour < 20);
}
inline int concession(Category c) {
    switch (c) {
    case Category::Adult: return 0;
    case Category::Student: return STUDENT_PERCENT;
    case Category::Senior: return SENIOR_PERCENT;
    }
    throw std::invalid_argument("unknown category");
}
inline Money roundPercent(Money amount, int percent) {
    if (amount < 0 || percent < 0 || percent > 200 ||
        amount > (LLONG_MAX - 50) / 200)
        throw std::invalid_argument("percentage out of range");
    return (amount * percent + 50) / 100; // half up
}
struct Passenger { Category category{Category::Adult}; };
struct Journey { Zone zone; int hour; Passenger passenger{}; };
class FareTable {
public:
    Money quote(const Journey& j) const {
        Money base = BASE.at(index(j.zone));
        bool peak = isPeak(j.hour);
        int discount = concession(j.passenger.category);
        Money gross = roundPercent(base, peak ? 100 + PEAK_PERCENT : 100);
        return roundPercent(gross, 100 - discount);
    }
};
class SmartCard {
    Money balance_;
public:
    explicit SmartCard(Money balance) : balance_{balance} {
        if (balance < 0) throw std::invalid_argument("negative balance");
    }
    Money balance() const { return balance_; }
    bool canTravel(Money f) const {
        return f >= 0 && balance_ >= MIN_BALANCE &&
               f <= balance_ - MIN_BALANCE;
    }
    bool deduct(Money f) {
        if (!canTravel(f)) return false;
        balance_ -= f;
        return true;
    }
    bool recharge(Money value) {
        if (value <= 0 || value > LLONG_MAX - balance_) return false;
        balance_ += value;
        return true;
    }
};
inline std::string money(Money p) {
    if (p < 0) throw std::invalid_argument("negative money");
    std::ostringstream out;
    out << "Rs " << p / 100 << '.' << std::setw(2)
        << std::setfill('0') << p % 100;
    return out.str();
}
} // namespace metro
