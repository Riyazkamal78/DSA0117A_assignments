#include <array>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

class Account {
public:
    virtual double rate() const { return 3.5; }
    virtual ~Account() { std::cout << "~Account\n"; }
};

class SeniorAccount final : public Account {
    std::array<double, 100> history_{};

public:
    double rate() const override { return 4.0; }
    ~SeniorAccount() override { std::cout << "~SeniorAccount (releases history)\n"; }
};

void printRate(const Account& account) {
    std::cout << account.rate() << "%\n";
}

int main() {
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Base pointers to live objects\n";
    {
        Account standard;
        SeniorAccount senior;
        Account* accounts[] = {&standard, &senior};
        for (const Account* account : accounts) {
            printRate(*account);
        }
    }

    std::cout << "\nOwning vector of unique_ptr<Account>\n";
    std::vector<std::unique_ptr<Account>> accounts;
    accounts.push_back(std::make_unique<Account>());
    accounts.push_back(std::make_unique<SeniorAccount>());
    for (const auto& account : accounts) {
        printRate(*account);
    }
}
