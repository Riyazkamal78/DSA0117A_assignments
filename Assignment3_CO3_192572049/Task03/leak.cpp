#include <iostream>
#if defined(__GNUC__)
__attribute__((noinline))
#endif
void forget() {
    char* seatMap = new char[200];
    seatMap[0] = 'X';
    std::cout << "Allocated 200 bytes, first seat " << seatMap[0] << '\n';
#ifndef LEAK
    delete[] seatMap;
    std::cout << "Outstanding payload: 0 bytes\n";
#else
    std::cout << "Outstanding payload: 200 bytes (delete omitted)\n";
#endif
}
int main() { forget(); }
