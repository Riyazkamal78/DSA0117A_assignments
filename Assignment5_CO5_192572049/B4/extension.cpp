#include "../B1/validate.hpp"
#include <optional>

using namespace hospital;

class TelehealthBill final : public Bill {
    double consultation_, platform_;
public:
    TelehealthBill(std::string patient, double consultation, double platform)
        : Bill(std::move(patient)), consultation_(charge(consultation)), platform_(charge(platform))
    { charge(total()); }
    double total() const noexcept override { return consultation_ + platform_; }
    std::string_view type() const noexcept override { return "TEL"; }
};

class CorporatePayer final : public Payer {
    double cap_;
public:
    explicit CorporatePayer(double cap) : cap_(charge(cap)) {}
    double amountCovered(const Bill& bill) const noexcept override
    {
        return std::min(cap_, bill.total() * 0.75);
    }
};

double audited_claim(const Bill& bill, const Payer& payer)
{
    try {
        return payer.amountCovered(bill);
    } catch (const ClaimError& error) {
        std::cout << "Audit: " << error.what() << '\n';
        throw;
    }
}

int main()
{
    check_baseline(std::cout);
    auto bill = std::make_unique<TelehealthBill>("Kiran", 900, 100);
    CorporatePayer corporate(700);
    double covered = audited_claim(*bill, corporate);
    if (bill->total() != 1000 || covered != 700)
        throw std::logic_error("extension mismatch");
    bill->print(std::cout);
    std::cout << " | Corporate cover " << covered << " | Due " << bill->total() - covered << '\n';

    std::optional<double> settlement;
    try {
        settlement = audited_claim(*bill, GovernmentScheme{});
        throw std::logic_error("expected unknown package");
    } catch (const UnknownPackage& error) {
        std::cout << "Claim pending: " << error.what() << '\n';
    }
    if (settlement || bill->total() != 1000)
        throw std::logic_error("failed claim changed the stored bill");
    settlement = audited_claim(*bill, SelfPay{});
    std::cout << "Retried using self pay. Stored gross = " << bill->total()
              << " | Patient due = " << bill->total() - *settlement << '\n';
}
