#include <cmath>
#include <complex>
#include <iomanip>
#include <iostream>
#include <stdexcept>
class Complex {
    double re,im;
public:
    Complex() : re(0),im(0) {}
    Complex(double r) : re(r),im(0) {}
    Complex(double r,double i) : re(r),im(i) {}
    double real() const { return re; }
    double imag() const { return im; }
    double magnitude() const { return std::hypot(re,im); }
    friend Complex operator+(const Complex&a,const Complex&b) { return {a.re+b.re,a.im+b.im}; }
    friend Complex operator-(const Complex&a,const Complex&b) { return {a.re-b.re,a.im-b.im}; }
    friend Complex operator*(const Complex&a,const Complex&b) {
        return {a.re*b.re-a.im*b.im,a.re*b.im+a.im*b.re};
    }
    friend Complex operator/(const Complex&a,const Complex&b) {
        if(b.re==0 && b.im==0) throw std::domain_error("zero impedance divisor");
        // Scaled ratio form avoids squaring the denominator components.
        if(std::abs(b.re)>=std::abs(b.im)) {
            double r=b.im/b.re,den=b.re+b.im*r;
            return {(a.re+a.im*r)/den,(a.im-a.re*r)/den};
        }
        double r=b.re/b.im,den=b.im+b.re*r;
        return {(a.re*r+a.im)/den,(a.im*r-a.re)/den};
    }
    friend bool operator==(const Complex&a,const Complex&b) { return a.re==b.re && a.im==b.im; }
    friend std::ostream& operator<<(std::ostream&out,const Complex&z) {
        return out<<z.re<<(z.im<0?" - j":" + j")<<std::abs(z.im);
    }
};
bool close(const Complex&a,const std::complex<double>&b) {
    return std::hypot(a.real()-b.real(),a.imag()-b.imag())<1e-9;
}
int main() {
    // omega=1000 rad/s, R=100 ohm, L=0.2 H, C=10 microfarad.
    const double omega=1000,R=100,L=0.2,C=10e-6;
    Complex resistor(R),inductor(0,omega*L),capacitor(0,-1/(omega*C));
    Complex series=resistor+inductor+capacitor;
    Complex parallel=(resistor*inductor)/(resistor+inductor);
    parallel=(parallel*capacitor)/(parallel+capacitor);
    std::cout<<std::fixed<<std::setprecision(6);
    std::cout<<"Series: "<<series<<" |Z|="<<series.magnitude()<<'\n';
    std::cout<<"Parallel: "<<parallel<<" |Z|="<<parallel.magnitude()<<'\n';
    std::complex<double> zr(R,0),zl(0,omega*L),zc(0,-1/(omega*C));
    auto expected=1.0/(1.0/zr+1.0/zl+1.0/zc);
    std::cout<<std::boolalpha<<"std::complex agreement: "<<close(series,zr+zl+zc)
             <<' '<<close(parallel,expected)<<'\n';
    std::cout<<"Basic operators: "<<Complex(3,4)-Complex(1,2)<<" | "
             <<Complex(1,2)*Complex(3,-4)<<" equality "<<(Complex()==Complex(0))<<'\n';
    try { auto invalid=resistor/Complex(); std::cout<<invalid; }
    catch(const std::domain_error&e) { std::cout<<"Rejected: "<<e.what()<<'\n'; }
    return !(close(series,zr+zl+zc) && close(parallel,expected));
}
