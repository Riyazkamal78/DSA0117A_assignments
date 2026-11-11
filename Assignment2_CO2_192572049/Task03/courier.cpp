#include <iostream>
#include <iomanip>
struct Parcel{double kg;bool express=false,insured=false;};
double cost(double w,bool e,bool i){return w*50+(e?100:0)+(i?25:0);}
double charge(int grams,bool e=false,bool i=false){std::cout<<"int grams: ";return cost(grams/1000.0,e,i);}
double charge(double kg,bool e=false,bool i=false){std::cout<<"double kg: ";return cost(kg,e,i);}
double charge(const Parcel& p){std::cout<<"Parcel: ";return cost(p.kg,p.express,p.insured);}
int main(){std::cout<<std::fixed<<std::setprecision(2);
 std::cout<<charge(2)<<'\n';std::cout<<charge(2.0f)<<'\n';
 std::cout<<charge(2000,true)<<'\n';std::cout<<charge(2.0,true,true)<<'\n';
 std::cout<<charge(Parcel{3.0,false,true})<<'\n';std::cout<<charge(1.5)<<'\n';}
