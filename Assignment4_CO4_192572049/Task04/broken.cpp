#include <iostream>

class Account {
public:
    double rate() const { return 3.5; }
    ~Account() { std::cout << "~Account\n"; }
};

class SeniorAccount : public Account {
    static inline int liveHistories_ = 0;
    double* history = new double[100];

public:
    SeniorAccount() { ++liveHistories_; }
    static int liveHistories() { return liveHistories_; }
    double rate() const { return 4.0; }
    ~SeniorAccount() {
        std::cout << "~SeniorAccount (releases history)\n";
        delete[] history;
        --liveHistories_;
    }
};

int main() {
    std::cout << std::unitbuf;
    Account* accounts[2] = {new Account, new SeniorAccount};
    for (Account* account : accounts) {
        std::cout << "Rate through Account*: " << account->rate() << "%\n";
    }
    {
        SeniorAccount original;
        Account copy = original;
        std::cout << "Senior rate before slicing: " << original.rate() << "%\n";
        std::cout << "Sliced copy rate: " << copy.rate() << "%\n";
    }
    std::cout << "Deleting through Account* (undefined for SeniorAccount):\n";
    for (Account* account : accounts) {
        delete account;
    }
    std::cout << "Outstanding history allocations: " << SeniorAccount::liveHistories()
              << " (" << SeniorAccount::liveHistories() * 100 * sizeof(double)
              << " payload bytes)\n";
}
