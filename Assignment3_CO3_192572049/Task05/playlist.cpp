#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
class Time {
    long long seconds;
public:
    explicit Time(long long s=0) : seconds(s) {
        if (s<0) throw std::invalid_argument("negative time");
    }
    Time& operator++() { ++seconds; return *this; }
    Time operator++(int) { Time old=*this; ++*this; return old; }
    bool operator==(const Time& b) const { return seconds==b.seconds; }
    bool operator<(const Time& b) const { return seconds<b.seconds; }
    friend std::ostream& operator<<(std::ostream& out,const Time& t) {
        char fill=out.fill();
        out << t.seconds/3600 << ':' << std::setfill('0') << std::setw(2)
            << t.seconds/60%60 << ':' << std::setw(2) << t.seconds%60;
        out.fill(fill); return out;
    }
};
class Playlist {
    std::vector<std::string> tracks;
public:
    Playlist() : tracks{"Intro","Song"} {}
    std::size_t size() const { return tracks.size(); }
    std::string& operator[](int i) {
        if (i<0 || static_cast<std::size_t>(i)>=size()) throw std::out_of_range("track index");
        return tracks[i];
    }
    const std::string& operator[](int i) const {
        if (i<0 || static_cast<std::size_t>(i)>=size()) throw std::out_of_range("track index");
        return tracks[i];
    }
};
int main() {
    Time t(3599); Time a=t++; Time b=++t;
    std::cout << "a=" << a << " b=" << b << " t=" << t << '\n';
    Time minute(59); std::cout << "Minute rollover " << ++minute << '\n';
    std::cout << std::boolalpha << "Comparisons " << (b==t) << ' ' << (a<t) << '\n';
    Playlist list; list[1]="Edited song"; const Playlist& view=list;
    std::cout << "Playing " << view[1] << " at " << t << '\n';
    for (int i : {-1,static_cast<int>(list.size())}) {
        try { std::cout << list[i] << '\n'; }
        catch (const std::out_of_range& e) { std::cout << "Rejected " << i << ": " << e.what() << '\n'; }
    }
}
