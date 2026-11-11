#include <iostream>
struct Example {
    int first, second;
    Example() : second(2), first(1) {}
};
int main() { Example e; std::cout << e.first << ' ' << e.second << '\n'; }
