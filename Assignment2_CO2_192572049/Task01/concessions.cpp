#include <iostream>
void byValue(long long fee,int p){fee-=fee*p/100;}
void byReference(long long& fee,int p){fee-=fee*p/100;}
void byPointer(long long* fee,int p){if(fee) *fee-=*fee*p/100;}
long long reduced(long long fee,int p){return fee-fee*p/100;}
int main(){long long a=45000,b=a,c=a;
 byValue(a,10);byReference(b,10);byPointer(&c,10);
 std::cout<<"Value: "<<a<<"\nReference: "<<b<<"\nPointer: "<<c<<'\n';
 std::cout<<"Merit then sports: "<<reduced(reduced(45000,10),15)<<'\n';
 std::cout<<"Single parent: "<<reduced(45000,25)<<'\n';}
