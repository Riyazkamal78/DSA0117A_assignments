#pragma once
#include <algorithm>
#include <cmath>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace hospital {

struct BillingError : std::runtime_error { using std::runtime_error::runtime_error; };
struct InvalidCharge : BillingError { using BillingError::BillingError; };
struct MissingPatient : BillingError { using BillingError::BillingError; };
struct ClaimError : BillingError { using BillingError::BillingError; };
struct AboveCover : ClaimError { using ClaimError::ClaimError; };
struct UnknownPackage : ClaimError { using ClaimError::ClaimError; };

inline double charge(double value)
{
    if (!std::isfinite(value) || value < 0) throw InvalidCharge("invalid charge");
    return value;
}

class Bill {
    std::string patient_;

protected:
    explicit Bill(std::string patient) : patient_(std::move(patient))
    {
        if (patient_.empty()) throw MissingPatient("missing patient record");
    }

public:
    virtual ~Bill() noexcept = default;
    virtual double total() const noexcept = 0;
    virtual std::string_view type() const noexcept = 0;
    const std::string& patient() const noexcept { return patient_; }

    void print(std::ostream& out) const
    {
        out << type() << ' ' << patient_ << ": " << total();
    }
};

class OutPatientBill final : public Bill {
    double consultation_, tests_;
public:
    OutPatientBill(std::string patient, double consultation, double tests)
        : Bill(std::move(patient)), consultation_(charge(consultation)), tests_(charge(tests))
    { charge(total()); }
    double total() const noexcept override { return consultation_ + tests_; }
    std::string_view type() const noexcept override { return "OP"; }
};

class InPatientBill final : public Bill {
    int days_;
    double rate_, procedures_, medicines_;
public:
    InPatientBill(std::string patient, int days, double rate, double procedures, double medicines)
        : Bill(std::move(patient)), days_(days), rate_(charge(rate)),
          procedures_(charge(procedures)), medicines_(charge(medicines))
    {
        if (days_ < 0) throw InvalidCharge("invalid stay length");
        charge(total());
    }
    double total() const noexcept override { return days_ * rate_ + procedures_ + medicines_; }
    std::string_view type() const noexcept override { return "IP"; }
};

class DayCareBill final : public Bill {
    double package_;
public:
    DayCareBill(std::string patient, double package)
        : Bill(std::move(patient)), package_(charge(package)) {}
    double total() const noexcept override { return package_; }
    std::string_view type() const noexcept override { return "DAY"; }
};

class EmergencyBill final : public Bill {
    double base_, hours_, rate_;
public:
    EmergencyBill(std::string patient, double base, double hours, double rate)
        : Bill(std::move(patient)), base_(charge(base)), hours_(charge(hours)), rate_(charge(rate))
    { charge(total()); }
    double total() const noexcept override { return base_ + hours_ * rate_; }
    std::string_view type() const noexcept override { return "ER"; }
};

class Payer {
public:
    virtual ~Payer() noexcept = default;
    virtual double amountCovered(const Bill& bill) const = 0;
};

class SelfPay final : public Payer {
public:
    double amountCovered(const Bill&) const noexcept override { return 0; }
};

class Insurance final : public Payer {
    double limit_, copay_;
public:
    Insurance(double limit, double copay) : limit_(charge(limit)), copay_(charge(copay))
    {
        if (copay_ > 1) throw ClaimError("invalid co-payment fraction");
    }

    double amountCovered(const Bill& bill) const override
    {
        double requested = bill.total() * (1 - copay_);
        if (requested > limit_) throw AboveCover("claim exceeds policy cover");
        return requested;
    }
};

class GovernmentScheme final : public Payer {
    std::unordered_map<std::string_view, double> rates_{
        {"OP", 1000}, {"IP", 30000}, {"DAY", 15000}, {"ER", 2000}};
public:
    double amountCovered(const Bill& bill) const override
    {
        auto rate = rates_.find(bill.type());
        if (rate == rates_.end()) throw UnknownPackage("unknown package");
        return std::min(rate->second, bill.total());
    }
};

} // namespace hospital
