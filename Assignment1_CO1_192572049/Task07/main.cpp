#include "../Common/metro.hpp"
#include <iostream>
// Procedural version, with a caller-owned balance.
bool canTravel(long long balance, long long fare) {
    return fare >= 0 && balance >= metro::MIN_BALANCE &&
           fare <= balance - metro::MIN_BALANCE;
}
bool deduct(long long& balance, long long fare) {
    if (!canTravel(balance, fare)) return false;
    balance -= fare;
    return true;
}
int main() {
    long long balance = 10000;
    metro::SmartCard card{10000};
    for (long long fare : {1000LL, 3600LL, 2000LL, -1LL}) {
        bool p = deduct(balance, fare);
        bool o = card.deduct(fare);
        std::cout << "fare=" << fare << " procedural=" << p
                  << " oop=" << o << " balances=" << balance
                  << ',' << card.balance() << '\n';
        if (p != o || balance != card.balance()) return 1;
    }
    struct PublicCard { long long balance; };
    PublicCard unsafe{10000};
    unsafe.balance = -1;
    std::cout << "Public member can break invariant: "
              << unsafe.balance << '\n';
}
