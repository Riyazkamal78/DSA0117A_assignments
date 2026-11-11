#include <chrono>
#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>
#include <algorithm>
class Prescription {

    int n;
    std::unique_ptr<std::string[]> medicines;
public:
    static int copies;
    explicit Prescription(int count) : n(count), medicines(allocate(count)) {
        for (int i = 0; i < n; ++i) medicines[i] = "Medicine";
    }
    static std::unique_ptr<std::string[]> allocate(int n) {
        if (n < 0) throw std::invalid_argument("negative count");
        return std::unique_ptr<std::string[]>(new std::string[n]);
    }
    Prescription(const Prescription& p) : n(p.n), medicines(allocate(p.n)) {
        std::copy_n(p.medicines.get(), n, medicines.get());
        ++copies;
        if (n <= 2) std::cout << "Copy constructor\n";
    }
    Prescription& operator=(const Prescription& p) {
        if (this != &p) {
            auto fresh = allocate(p.n);
            std::copy_n(p.medicines.get(), p.n, fresh.get());
            medicines.swap(fresh); n = p.n;
        }
        return *this;
    }
    ~Prescription() = default;
    void set(int i, const std::string& s) {
        if (i < 0 || i >= n) throw std::out_of_range("medicine index");
        medicines[i] = s;
    }
    const std::string& get(int i) const {
        if (i < 0 || i >= n) throw std::out_of_range("medicine index");
        return medicines[i];
    }
    std::size_t size() const { return static_cast<std::size_t>(n); }

};
int Prescription::copies = 0;
void byValue(Prescription p) { std::cout << "Value size " << p.size() << '\n'; }
void byReference(const Prescription& p) { std::cout << "Reference size " << p.size() << '\n'; }
int main() {
    Prescription p(2); p.set(0, "Original");
    Prescription q = p; q.set(0, "Follow-up");
    p = p;
    std::cout << p.get(0) << " | " << q.get(0) << '\n';
    Prescription::copies = 0;
    byValue(p); std::cout << "Copies by value: " << Prescription::copies << '\n';
    Prescription::copies = 0;
    byReference(p); std::cout << "Copies by const reference: " << Prescription::copies << '\n';
    for (int n : {10, 1000, 100000}) {
        Prescription source(n);
        auto start = std::chrono::steady_clock::now();
        std::size_t checksum = 0;
        for (int i = 0; i < 20; ++i) {
            Prescription copy(source); checksum += copy.size();
        }
        double ms = std::chrono::duration<double, std::milli>(
            std::chrono::steady_clock::now() - start).count();
        std::cout << "n=" << n << " 20 copies " << ms << " ms checksum " << checksum << '\n';
    }
}
