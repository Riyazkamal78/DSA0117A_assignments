#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>
class History{
 std::vector<int> a;int count=0,start=0;bool ring;
public:
 long long moves=0;
 History(int k,bool r):a(k),ring(r){}
 void add(int v){int k=static_cast<int>(a.size());
 if(count<k){a[count++]=v;++moves;}
 else if(ring){a[start]=v;start=(start+1)%k;++moves;}
 else{for(int i=1;i<k;++i){a[i-1]=a[i];++moves;}a[k-1]=v;++moves;}}
 std::vector<int> values()const{std::vector<int> v;for(int i=0;i<count;++i)v.push_back(a[(start+i)%a.size()]);return v;}
};
int main(){constexpr int N=200000;
 for(int k:{10,50,1000}){History shift(k,false),ring(k,true);
 auto run=[&](History& h){auto t=std::chrono::steady_clock::now();for(int i=1;i<=N;++i)h.add(i);
 return std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-t).count();};
 double s=run(shift),r=run(ring);auto a=shift.values(),b=ring.values();
 std::cout<<"k="<<k<<" N="<<N<<" equal="<<std::boolalpha<<(a==b)<<'\n';
 std::cout<<"shift "<<s<<" ms writes "<<shift.moves<<"\nring  "<<r<<" ms writes "<<ring.moves<<'\n';
 std::cout<<"retained "<<b.front()<<".."<<b.back()<<'\n';if(a!=b)return 1;}
 History h(10,true);for(int i=1;i<=11;++i)h.add(i);std::cout<<"11th transaction:";for(int v:h.values())std::cout<<' '<<v;std::cout<<'\n';}
