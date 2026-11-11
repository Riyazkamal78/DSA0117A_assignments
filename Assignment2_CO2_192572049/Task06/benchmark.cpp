#include <chrono>
#include <iostream>
class Time{
 int h,m;
public:
 Time(int hh,int mm):h(hh),m(mm){}
 int total()const{return h*60+m;}
 int minutesTo(const Time& b)const{int d=total()-b.total();return d<0?-d:d;}
 friend int regular(const Time& a,const Time& b);
 friend int inlined(const Time& a,const Time& b);
};
int regular(const Time& a,const Time& b){int d=(a.h*60+a.m)-(b.h*60+b.m);return d<0?-d:d;}
inline int inlined(const Time& a,const Time& b){int d=(a.h*60+a.m)-(b.h*60+b.m);return d<0?-d:d;}
int getters(const Time& a,const Time& b){int d=a.total()-b.total();return d<0?-d:d;}
template<class F>void measure(const char* name,F f){volatile int seed=7;long long sum=0;
 auto start=std::chrono::steady_clock::now();for(int i=0;i<10000000;++i){int x=(i+seed)%1440;Time a(x/60,x%60),b(13,10);sum+=f(a,b);}
 auto end=std::chrono::steady_clock::now();std::cout<<name<<": "<<std::chrono::duration<double,std::milli>(end-start).count()<<" ms checksum "<<sum<<'\n';}
int main(){measure("regular friend",[](const Time&a,const Time&b){return regular(a,b);});
 measure("inline friend",[](const Time&a,const Time&b){return inlined(a,b);});
 measure("member",[](const Time&a,const Time&b){return a.minutesTo(b);});
 measure("getters",[](const Time&a,const Time&b){return getters(a,b);});}
