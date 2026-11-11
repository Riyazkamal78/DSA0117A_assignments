#include <iostream>
class Time{int h,m;public:Time(int hh,int mm):h(hh),m(mm){}
 friend int minutesBetween(const Time& a,const Time& b);
 friend bool earlier(const Time& a,const Time& b);
};
int minutesBetween(const Time& a,const Time& b){int d=(a.h*60+a.m)-(b.h*60+b.m);return d<0?-d:d;}
bool earlier(const Time& a,const Time& b){return a.h*60+a.m<b.h*60+b.m;}

int main(){Time a(11,50),b(12,10),c(9,45);
 std::cout<<std::boolalpha<<"Across noon: "<<minutesBetween(a,b)<<' '<<earlier(a,b)<<'\n';
 std::cout<<"Same time: "<<minutesBetween(c,c)<<' '<<earlier(c,c)<<'\n';}
