#include <iostream>
#include <string>
class Prescription {
    std::string* medicines;
public:
    Prescription() : medicines(new std::string[1]{"Original"}) {}
    ~Prescription() { delete[] medicines; }
    void set(const std::string& s) { medicines[0] = s; }
    const std::string& get() const { return medicines[0]; }
    // No copy constructor: compiler copies the pointer value.
};
int main() {
    Prescription p;
    Prescription q = p;
    q.set("Changed");
    std::cout << "Original changed to: " << p.get() << std::endl;
    // q frees the shared array first; p then uses the freed allocation.
}
