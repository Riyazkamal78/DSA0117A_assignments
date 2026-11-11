#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
class Money {
    long long paise = 0;
    static long long sum(long long a, long long b) {
        if ((b > 0 && a > std::numeric_limits<long long>::max()-b) ||
            (b < 0 && a < std::numeric_limits<long long>::min()-b))
            throw std::overflow_error("money overflow");
        return a+b;
    }
public:
    Money(long long rupees = 0) {
        if (rupees > std::numeric_limits<long long>::max()/100 ||
            rupees < std::numeric_limits<long long>::min()/100)
            throw std::overflow_error("rupees overflow");
        paise = rupees*100;
    }
    static Money fromPaise(long long p) { Money m; m.paise=p; return m; }
    Money& operator+=(const Money& b) { paise=sum(paise,b.paise); return *this; }
    friend Money operator+(Money a, const Money& b) { return a+=b; }
    friend Money operator-(const Money& a, const Money& b) {
        long long x=a.paise, y=b.paise;
        if ((y>0 && x<std::numeric_limits<long long>::min()+y) ||
            (y<0 && x>std::numeric_limits<long long>::max()+y))
            throw std::overflow_error("money overflow");
        return fromPaise(x-y);
    }
    friend bool operator==(const Money& a,const Money& b) { return a.paise==b.paise; }
    friend bool operator<(const Money& a,const Money& b) { return a.paise<b.paise; }
    friend std::ostream& operator<<(std::ostream& out,const Money& m) {
        unsigned long long mag=m.paise<0 ? 0ULL-static_cast<unsigned long long>(m.paise)
                                        : static_cast<unsigned long long>(m.paise);
        char fill=out.fill();
        out << "Rs " << (m.paise<0?"-":"") << mag/100 << '.'
            << std::setfill('0') << std::setw(2) << mag%100;
        out.fill(fill); return out;
    }
    friend std::istream& operator>>(std::istream& in,Money& m) {
        std::string token; if (!(in>>token)) return in;
        try {
            bool negative=token[0]=='-';
            std::size_t begin=(negative || token[0]=='+')?1:0;
            std::size_t dot=token.find('.',begin);
            std::string whole=token.substr(begin,dot==std::string::npos?dot:dot-begin);
            std::string fraction=dot==std::string::npos?"":token.substr(dot+1);
            auto digits=[](const std::string& s) {
                return s.find_first_not_of("0123456789")==std::string::npos;
            };
            if (whole.empty() || !digits(whole) || !digits(fraction) || fraction.size()>2)
                throw std::invalid_argument("amount");
            while (fraction.size()<2) fraction+='0';
            unsigned long long w=std::stoull(whole), f=std::stoull(fraction);
            unsigned long long max=static_cast<unsigned long long>(std::numeric_limits<long long>::max());
            if (w>max/100 || w*100>max-f) throw std::out_of_range("amount");
            long long p=static_cast<long long>(w*100+f);
            m=fromPaise(negative?-p:p);
        } catch (...) { in.setstate(std::ios::failbit); }
        return in;
    }
};
int main() {
    Money a=Money::fromPaise(75), b=Money::fromPaise(50);
    std::cout << "Carry: " << a+b << "\nNegative: " << b-a << '\n';
    Money bill=Money(120)+Money::fromPaise(7975); bill+=Money::fromPaise(25);
    std::cout << "Bill: " << bill << "\nMixed: " << 100+a << " | " << a+100 << '\n';
    std::cout << std::boolalpha << "Compare: " << (a==b) << ' ' << (b<a) << '\n';
    std::istringstream input("12.34 -0.25 bad"); Money value;
    while (input>>value) std::cout << "Read: " << value << '\n';
    std::cout << "Invalid rejected: " << input.fail() << '\n';
    Money exact; double approximate=0;
    for (int i=0;i<1000000;++i) { exact+=Money::fromPaise(1); approximate+=0.01; }
    std::cout << "Integer million: " << exact << '\n' << std::setprecision(17)
              << "Double million: " << approximate << " error " << approximate-10000.0 << '\n';
}
