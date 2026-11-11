#include <iostream>
#include <string>
class BusTicket{
 static int issued;static const int CAPACITY=3;int seat;std::string name;
public:
 explicit BusTicket(const std::string& n):seat(0),name(n){if(issued<CAPACITY)seat=++issued;}
 static int count(){return issued;}
 void print()const{std::cout<<name<<": "<<(seat?"Seat "+std::to_string(seat):std::string("Waiting"))<<'\n';}
};
int BusTicket::issued=0;

void issue(){BusTicket trip[2]={BusTicket("Asha"),BusTicket("Bala")};
 for(const auto& t:trip){t.print();}std::cout<<"Issued: "<<BusTicket::count()<<'\n';}
int main(){issue();issue();}
