#include <iostream>
#include <stdexcept>
int highestIndex(const int* a,int n){if(n<=0)throw std::invalid_argument("empty");
 int best=0;for(int i=1;i<n;++i)if(a[i]>a[best])best=i;return best;}
int& highest(int* a,int n){return a[highestIndex(a,n)];}
const int& highestConst(const int* a,int n){return a[highestIndex(a,n)];}
int highestCopy(const int* a,int n){return highestConst(a,n);}
int* highestPointer(int* a,int n){return &highest(a,n);}
int main(){int a[]={72,91,84};highest(a,3)+=2;
 std::cout<<"After moderation: "<<a[1]<<'\n';highest(a,3)=95;
 std::cout<<"After assignment: "<<a[1]<<'\n';int copy=highestCopy(a,3);copy=0;
 std::cout<<"Copy: "<<copy<<", stored: "<<highestConst(a,3)<<'\n';
 std::cout<<"Index: "<<highestIndex(a,3)<<", pointer value: "<<*highestPointer(a,3)<<'\n';}
