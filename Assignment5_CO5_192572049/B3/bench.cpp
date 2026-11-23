#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <variant>
#include <vector>

using Amount = std::int64_t;

template<int Kind>
Amount calculate(int a, int b, int c)
{
    if constexpr (Kind == 0) return a + b;
    if constexpr (Kind == 1) return a * b + c;
    if constexpr (Kind == 2) return a;
    return a + b * c;
}

struct Bill {
    virtual ~Bill() = default;
    virtual Amount total() const = 0;
};

template<int Kind>
struct VirtualBill final : Bill {
    const int a, b, c;
    VirtualBill(int first, int second, int third) : a(first), b(second), c(third) {}
    Amount total() const override { return calculate<Kind>(a, b, c); }
};

struct TaggedBill {
    int kind, a, b, c;
    Amount total() const
    {
        switch (kind) {
        case 0: return calculate<0>(a, b, c);
        case 1: return calculate<1>(a, b, c);
        case 2: return calculate<2>(a, b, c);
        case 3: return calculate<3>(a, b, c);
        }
        throw std::logic_error("unknown bill kind");
    }
};

template<int Kind>
struct PlainBill {
    int a, b, c;
    Amount total() const { return calculate<Kind>(a, b, c); }
};

using Variant = std::variant<PlainBill<0>, PlainBill<1>, PlainBill<2>, PlainBill<3>>;

std::unique_ptr<Bill> make_virtual(TaggedBill row)
{
    switch (row.kind) {
    case 0: return std::make_unique<VirtualBill<0>>(row.a, row.b, row.c);
    case 1: return std::make_unique<VirtualBill<1>>(row.a, row.b, row.c);
    case 2: return std::make_unique<VirtualBill<2>>(row.a, row.b, row.c);
    default: return std::make_unique<VirtualBill<3>>(row.a, row.b, row.c);
    }
}

Variant make_variant(TaggedBill row)
{
    switch (row.kind) {
    case 0: return PlainBill<0>{row.a, row.b, row.c};
    case 1: return PlainBill<1>{row.a, row.b, row.c};
    case 2: return PlainBill<2>{row.a, row.b, row.c};
    default: return PlainBill<3>{row.a, row.b, row.c};
    }
}

template<class Operation>
double median_ms(Operation operation, Amount expected)
{
    using Clock = std::chrono::steady_clock;
    std::array<double, 5> samples{};
    for (double& sample : samples) {
        auto start = Clock::now();
        volatile Amount observed = operation();
        auto stop = Clock::now();
        if (observed != expected) throw std::logic_error("checksum mismatch");
        sample = std::chrono::duration<double, std::milli>(stop - start).count();
    }
    std::sort(samples.begin(), samples.end());
    return samples[2];
}

void compare_bills(std::size_t count)
{
    std::vector<std::unique_ptr<Bill>> virtual_bills;
    std::vector<TaggedBill> tagged_bills;
    std::vector<Variant> variants;
    virtual_bills.reserve(count);
    tagged_bills.reserve(count);
    variants.reserve(count);

    Amount expected = 0;
    std::uint32_t seed = 47;
    for (std::size_t i = 0; i < count; ++i) {
        seed = seed * 1664525u + 1013904223u;
        TaggedBill row{static_cast<int>((seed >> 16) & 3), 200 + static_cast<int>(i % 80),
                       3 + static_cast<int>(i % 6), 50};
        tagged_bills.push_back(row);
        virtual_bills.push_back(make_virtual(row));
        variants.push_back(make_variant(row));
        expected += row.total();
    }
    if (count == 10) {
        std::cout << "Row Kind Virtual Switch Variant\n";
        for (std::size_t i = 0; i < count; ++i) {
            Amount a = virtual_bills[i]->total();
            Amount b = tagged_bills[i].total();
            Amount c = std::visit([](const auto& bill) { return bill.total(); }, variants[i]);
            if (a != b || a != c) throw std::logic_error("per-bill result mismatch");
            std::cout << i + 1 << ' ' << tagged_bills[i].kind << ' ' << a << ' ' << b << ' ' << c << '\n';
        }
        std::cout << "Verified total for 10 bills: " << expected << "\n\n";
        return;
    }

    auto v = median_ms([&] {
        Amount sum = 0;
        for (const auto& bill : virtual_bills) sum += bill->total();
        return sum;
    }, expected);
    auto s = median_ms([&] {
        Amount sum = 0;
        for (const auto& bill : tagged_bills) sum += bill.total();
        return sum;
    }, expected);
    auto x = median_ms([&] {
        Amount sum = 0;
        for (const auto& bill : variants)
            sum += std::visit([](const auto& value) { return value.total(); }, bill);
        return sum;
    }, expected);
    std::cout << count << ' ' << v << ' ' << s << ' ' << x << ' ' << expected << '\n';
}

enum class Status { accepted, rejected };
struct Rejected {};

[[gnu::noinline]] Status validate_status(int amount)
{
    return amount < 0 ? Status::rejected : Status::accepted;
}
[[gnu::noinline]] int validate_throw(int amount)
{
    if (amount < 0) throw Rejected{};
    return amount;
}
[[gnu::noinline]] std::optional<double> validate_optional(double amount)
{
    if (amount < 0) return std::nullopt;
    return amount;
}

void compare_errors(int invalid_pct, int count)
{
    std::vector<int> amounts;
    for (int i = 0; i < count; ++i)
        amounts.push_back(i % 100 < invalid_pct ? -1 : 400);
    auto expected = static_cast<Amount>(count - count * invalid_pct / 100) * 400;
    auto status = median_ms([&] {
        Amount sum = 0;
        for (int amount : amounts)
            if (validate_status(amount) == Status::accepted) sum += amount;
        return sum;
    }, expected);
    auto exceptions = median_ms([&] {
        Amount sum = 0;
        for (int amount : amounts) {
            try { sum += validate_throw(amount); }
            catch (const Rejected&) {}
        }
        return sum;
    }, expected);
    auto optional = median_ms([&] {
        Amount sum = 0;
        for (int amount : amounts) sum += static_cast<Amount>(validate_optional(amount).value_or(0));
        return sum;
    }, expected);
    std::cout << invalid_pct << "% " << status << ' ' << exceptions << ' ' << optional << '\n';
}

int main(int argc, char**)
{
    std::cout << std::fixed << std::setprecision(3);
    compare_bills(10);
    std::cout << "Bills Virtual_ms Switch_ms Variant_ms Checksum\n";
    compare_bills(1000);
    if (argc == 1) {
        compare_bills(1'000'000);
        compare_bills(10'000'000);
    }
    std::cout << "\nInvalid Status_ms Exception_ms Optional_ms\n";
    for (int rate : {0, 1, 50}) compare_errors(rate, argc == 1 ? 1'000'000 : 1000);
    std::cout << "\nSize in bytes\n"
              << "Virtual object " << sizeof(VirtualBill<0>) << " + owner "
              << sizeof(std::unique_ptr<Bill>) << " (excludes allocator overhead)\n"
              << "Tagged " << sizeof(TaggedBill) << " | Variant " << sizeof(Variant)
              << " | Plain alternative " << sizeof(PlainBill<0>) << '\n'
              << "Status " << sizeof(Status) << " | int " << sizeof(int)
              << " | optional<int> " << sizeof(std::optional<int>)
              << " | optional<double> " << sizeof(std::optional<double>) << '\n';
}
