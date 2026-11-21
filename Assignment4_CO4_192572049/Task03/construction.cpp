#include <iostream>
#include <string>

struct Address {
    std::string city;
    int pin;

    Address(const std::string& c, int p) : city(c), pin(p) { std::cout << "Address\n"; }
    ~Address() { std::cout << "~Address\n"; }
};

struct Course {
    std::string name;

    explicit Course(const std::string& n) : name(n) { std::cout << "Course\n"; }
    ~Course() { std::cout << "~Course\n"; }
};

class Person {
    std::string name;
    Address home;

public:
    Person(const std::string& n, const std::string& city, int pin)
        : name(n), home(city, pin) { std::cout << "Person\n"; }
    ~Person() { std::cout << "~Person\n"; }
};

class Student : virtual public Person {
    Course course;

public:
    Student(const std::string& n, const std::string& city, int pin,
            const std::string& courseName)
        : Person(n, city, pin), course(courseName) { std::cout << "Student\n"; }
    ~Student() { std::cout << "~Student\n"; }
};

class Staff : virtual public Person {
    std::string department;

public:
    Staff(const std::string& n, const std::string& city, int pin,
          const std::string& dept)
        : Person(n, city, pin), department(dept) { std::cout << "Staff\n"; }
    ~Staff() { std::cout << "~Staff\n"; }
};

class TeachingAssistant : public Student, public Staff {
public:
    TeachingAssistant(const std::string& n, const std::string& city, int pin,
                      const std::string& course, const std::string& dept)
        : Person(n, city, pin), Student(n, city, pin, course),
          Staff(n, city, pin, dept) { std::cout << "TeachingAssistant\n"; }
    ~TeachingAssistant() { std::cout << "~TeachingAssistant\n"; }
};

int main()
{
    std::cout << "[Student]\n";
    { Student student("Kavya", "Salem", 636001, "C++"); }
    std::cout << "\n[Staff]\n";
    { Staff staff("Arun", "Chennai", 600001, "CSE"); }
    std::cout << "\n[TeachingAssistant]\n";
    { TeachingAssistant ta("Anu", "Madurai", 625001, "C++", "CSE"); }
    std::cout << "\nsizeof(TeachingAssistant): " << sizeof(TeachingAssistant) << " bytes\n";
}
