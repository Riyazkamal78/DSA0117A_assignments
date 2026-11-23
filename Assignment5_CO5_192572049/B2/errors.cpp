#include "../B1/billing.hpp"
#include <iostream>
#include <memory>
#include <new>

using namespace hospital;

struct UnknownBill final : Bill {
    UnknownBill() : Bill("Test patient") {}
    double total() const noexcept override { return 500; }
    std::string_view type() const noexcept override { return "UNKNOWN"; }
};

struct Guard {
    const char* name;
    ~Guard() noexcept { std::cout << "Destroyed " << name << '\n'; }
};

void invalid_action()
{
    Guard inner{"inner guard"};
    throw 42;
}

template<class Action>
void run(const char* label, Action action)
{
    std::cout << label << '\n';
    try {
        Guard outer{"case guard"};
        action();
    } catch (const BillingError& error) {
        std::cout << "Billing error: " << error.what() << '\n';
    } catch (const std::invalid_argument&) {
        std::cout << "Caught std::invalid_argument from std::stod\n";
    } catch (const std::bad_alloc&) {
        std::cout << "Caught std::bad_alloc\n";
    } catch (...) {
        std::cout << "Catch-all handled unknown exception\n";
    }
}

int main()
{
    run("Negative charge", [] { OutPatientBill bill("Ravi", -1, 20); });
    run("Above cover", [] {
        OutPatientBill bill("Ravi", 60'000, 0);
        Insurance plan(50'000, 0);
        (void)plan.amountCovered(bill);
    });
    run("Unknown package", [] {
        GovernmentScheme scheme;
        (void)scheme.amountCovered(UnknownBill{});
    });
    run("Invalid number", [] { (void)std::stod("abc"); });
    run("Missing patient", [] { EmergencyBill bill("", 1000, 2, 300); });
    run("Allocation failure", [] {
        // Deliberate fault injection keeps the test deterministic without consuming RAM.
        auto failing_allocator = []() -> std::unique_ptr<int[]> { throw std::bad_alloc{}; };
        auto storage = failing_allocator();
    });
    run("Unknown exception", invalid_action);
    run("Subsequent valid case", [] {
        OutPatientBill bill("Ravi", 500, 1200);
        std::cout << "Still running. Total = " << bill.total() << '\n';
    });
}
