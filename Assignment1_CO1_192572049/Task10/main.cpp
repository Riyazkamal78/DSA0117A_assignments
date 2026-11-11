#include <chrono>
#include <climits>
#include <iomanip>
#include <iostream>
int main() {
    std::cout << std::fixed << std::setprecision(9);
    for (long long n : {20000LL, 500000LL, 3000000LL}) {
        double actualFareTotal = 0.0, stressTotal = 0.0;
        long long paiseTotal = 0;
        auto start = std::chrono::steady_clock::now();
        for (long long i = 0; i < n; ++i) {
            actualFareTotal += 36.0;
            stressTotal += 10.01; // Precision stress, not a base fare
            paiseTotal += 1001;
        }
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::steady_clock::now() - start).count();
        long long realPaise = n * 3600LL;
        std::cout << "n=" << n << " actual Rs=" << actualFareTotal << '\n'
          << "actual difference Rs="
          << actualFareTotal - static_cast<double>(realPaise) / 100.0 << '\n'
          << "stress double Rs=" << stressTotal
          << " integer paise=" << paiseTotal << '\n'
          << "stress difference Rs="
          << stressTotal - static_cast<double>(paiseTotal) / 100.0 << '\n'
          << "actual paise=" << realPaise << " int fits="
          << std::boolalpha << (realPaise <= INT_MAX)
          << " loop microseconds=" << us << '\n';
    }
    std::cout << "Max int journeys at 3600 paise: " << INT_MAX / 3600 << '\n'
      << "First unsafe journey: " << INT_MAX / 3600 + 1 << '\n';
    int sum = 0;
    long long accepted = 0;
    while (sum <= INT_MAX - 3600) { sum += 3600; ++accepted; }
    std::cout << "Guarded int stopped at " << accepted << " journeys; "
              << sum << " paise\n";
}
