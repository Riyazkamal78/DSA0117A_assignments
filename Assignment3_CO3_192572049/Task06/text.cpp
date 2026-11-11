#include <chrono>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
class Text {
    std::size_t n;
    char* data;
    explicit Text(std::size_t size) : n(size),data(new char[n+1]{}) {}
public:
    static int copies;
    Text(const char* s="") : n(std::strlen(s)),data(new char[n+1]) {
        std::memcpy(data,s,n+1);
    }
    Text(const Text& t) : n(t.n),data(new char[n+1]) {
        std::memcpy(data,t.data,n+1); ++copies;
    }
    Text& operator=(const Text& t) {
        if(this==&t) return *this;
        std::unique_ptr<char[]> fresh(new char[t.n+1]);
        std::memcpy(fresh.get(),t.data,t.n+1);
        delete[] data; data=fresh.release(); n=t.n; return *this;
    }
    ~Text() { delete[] data; }
    std::size_t size() const { return n; }
    friend Text operator+(const Text& a,const Text& b) {
        Text result(a.n+b.n);
        std::memcpy(result.data,a.data,a.n);
        std::memcpy(result.data+a.n,b.data,b.n+1);
        return result;
    }
    friend bool operator==(const Text& a,const Text& b) {
        return a.n==b.n && std::memcmp(a.data,b.data,a.n)==0;
    }
    friend bool operator!=(const Text& a,const Text& b) { return !(a==b); }
    friend std::ostream& operator<<(std::ostream& out,const Text& t) { return out<<t.data; }
};
int Text::copies=0;
template<class F>void timed(const char* name,F f) {
    auto start=std::chrono::steady_clock::now(); auto sum=f();
    std::cout<<name<<": "<<std::chrono::duration<double,std::milli>(
        std::chrono::steady_clock::now()-start).count()<<" ms checksum "<<sum<<'\n';
}
int main() {
    Text a("Hello"),b(" "),c("SMS"); Text::copies=0;
    Text joined=a+b+c;
    std::cout<<joined<<" copies="<<Text::copies<<'\n';
    joined=joined;
    Text empty; Text copy(joined); copy=empty;
    std::cout<<std::boolalpha<<"Self assignment "<<(joined==Text("Hello SMS"))
             <<" empty "<<(copy==empty)<<" different "<<(joined!=empty)<<'\n';
    std::string longInput(100000,'x'); Text longText(longInput.c_str());
    Text longCopy=longText; longText=empty;
    std::cout<<"Long independent copy size "<<longCopy.size()<<'\n';
    volatile int seed=7;
    timed("Text 100000 concatenations",[&] {
        std::size_t sum=0;
        for(int i=0;i<100000;++i) {
            Text x((i+seed)%2?"alpha":"omega"), y("beta");
            Text z=x+y; sum+=z.size();
        }
        return sum;
    });
    timed("std::string 100000 concatenations",[&] {
        std::size_t sum=0;
        for(int i=0;i<100000;++i) {
            std::string x=(i+seed)%2?"alpha":"omega",y="beta";
            std::string z=x+y; sum+=z.size();
        }
        return sum;
    });
}
