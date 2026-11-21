#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class Staff {
    std::string name_;
    int id_;

protected:
    Staff(std::string name, int id) : name_(std::move(name)), id_(id) {}

public:
    virtual ~Staff() = default;
    const std::string& name() const { return name_; }
    int id() const { return id_; }
    virtual double pay() const = 0;
};

class SalariedStaff : public Staff {
    static constexpr double daRate = 0.42;
    double basic_;

protected:
    SalariedStaff(std::string name, int id, double basic)
        : Staff(std::move(name), id), basic_(basic) {}
    double payWithDA() const { return basic_ * (1 + daRate); }
};

class Professor final : public SalariedStaff {
    double research_;

public:
    Professor(std::string name, int id, double basic, double research)
        : SalariedStaff(std::move(name), id, basic), research_(research) {}
    double pay() const override { return payWithDA() + research_; }
};

class LabAssistant final : public SalariedStaff {
public:
    LabAssistant(std::string name, int id, double basic)
        : SalariedStaff(std::move(name), id, basic) {}
    double pay() const override { return payWithDA(); }
};

class GuestFaculty final : public Staff {
    double hours_;
    double rate_;

public:
    GuestFaculty(std::string name, int id, double hours, double rate)
        : Staff(std::move(name), id), hours_(hours), rate_(rate) {}
    double pay() const override { return hours_ * rate_; }
};

int main() {
    std::vector<std::unique_ptr<Staff>> payroll;
    payroll.push_back(std::make_unique<Professor>("Meera", 101, 50000, 5000));
    payroll.push_back(std::make_unique<LabAssistant>("Arun", 102, 25000));
    payroll.push_back(std::make_unique<GuestFaculty>("Isha", 103, 20, 800));
    std::cout << std::fixed << std::setprecision(2);
    for (const auto& staff : payroll) {
        std::cout << staff->id() << ' ' << staff->name() << ": "
                  << staff->pay() << '\n';
    }
}
