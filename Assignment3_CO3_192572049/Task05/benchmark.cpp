#include <chrono>
#include <iostream>
#include <stdexcept>
struct Time {
    long long seconds=0;
    Time& operator++() { ++seconds; return *this; }
    Time operator++(int) { Time old=*this; ++*this; return old; }
};
template<class F> void timed(const char* label,F f) {
    auto start=std::chrono::steady_clock::now();
    long long checksum=f();
    double ms=std::chrono::duration<double,std::milli>(
        std::chrono::steady_clock::now()-start).count();
    std::cout << label << ": " << ms << " ms checksum " << checksum << '\n';
}
struct Playlist {
    int values[16]{};
    int checked(int i) const {
        if (i<0 || i>=16) throw std::out_of_range("index");
        return values[i];
    }
    int unchecked(int i) const { return values[i]; }
};
int main() {
    constexpr int N=10000000;
    // Volatile object access prevents replacing all increments with one addition.
    volatile long long observed=0;
    timed("prefix",[&] {
        Time t;
        for (int i=0;i<N;++i) { t.seconds=observed; ++t; observed=t.seconds; }
        return t.seconds;
    });
    observed=0;
    timed("postfix",[&] {
        Time t;
        for (int i=0;i<N;++i) { t.seconds=observed; t++; observed=t.seconds; }
        return t.seconds;
    });
    Playlist p; for(int i=0;i<16;++i) p.values[i]=i;
    volatile int input=3;
    timed("checked access",[&] {
        long long sum=0;
        for(int i=0;i<N;++i) sum+=p.checked((input+i)%16);
        return sum;
    });
    timed("unchecked access",[&] {
        long long sum=0;
        for(int i=0;i<N;++i) sum+=p.unchecked((input+i)%16);
        return sum;
    });
}
