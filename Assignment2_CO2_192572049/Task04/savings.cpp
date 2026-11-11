#include <array>
#include <iostream>
#include <string>
class SavingsAccount{
 static constexpr int K=10;
 std::string number;long long balance;std::array<long long,K> transactions{};int count=0;
 void record(long long v){if(count==K){for(int i=1;i<K;++i)transactions[i-1]=transactions[i];--count;}transactions[count++]=v;}
public:
 SavingsAccount(std::string n,long long b):number(n),balance(b){}
 void deposit(long long amount){if(amount>0){balance+=amount;record(amount);}}
 bool withdraw(long long amount);
 void miniStatement()const;
};
bool SavingsAccount::withdraw(long long amount){if(amount<=0||amount>balance)return false;balance-=amount;record(-amount);return true;}
void SavingsAccount::miniStatement()const{std::cout<<"Account "<<number<<" Balance "<<balance<<"\nLast "<<count<<" transactions:";
 for(int i=0;i<count;++i){std::cout<<' '<<transactions[i];}std::cout<<'\n';}
int main(){SavingsAccount a("192572049",1000);for(int i=1;i<=11;++i)a.deposit(i*100);
 a.miniStatement();std::cout<<std::boolalpha<<"Withdraw 200: "<<a.withdraw(200)<<'\n';a.miniStatement();
 std::cout<<"Overdraft accepted: "<<a.withdraw(99999)<<'\n';}
