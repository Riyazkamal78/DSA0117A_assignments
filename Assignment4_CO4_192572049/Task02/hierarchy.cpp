#include <iostream>
#include <string>

class Person {
    std::string name_;
    int id_;
    std::string phone_;

public:
    Person(const std::string& name, int id, const std::string& phone)
        : name_(name), id_(id), phone_(phone) {}
    const std::string& name() const { return name_; }
    int id() const { return id_; }
    const std::string& phone() const { return phone_; }
};

class Student : virtual public Person {
    std::string programme_;
    double cgpa_;

public:
    Student(const Person& person, const std::string& programme, double cgpa)
        : Person(person), programme_(programme), cgpa_(cgpa) {}
    const std::string& programme() const { return programme_; }
    double cgpa() const { return cgpa_; }
};

class Staff : virtual public Person {
    std::string department_;
    double salary_;

public:
    Staff(const Person& person, const std::string& department, double salary)
        : Person(person), department_(department), salary_(salary) {}
    const std::string& department() const { return department_; }
    double salary() const { return salary_; }
};

class TeachingAssistant : public Student, public Staff {
public:
    TeachingAssistant(const Person& person, const std::string& programme, double cgpa,
                      const std::string& department, double salary)
        : Person(person), Student(person, programme, cgpa),
          Staff(person, department, salary) {}
};

struct Fellowship {
    std::string scheme;
    double amount;
};

class ResearchScholar : public Student {
    Fellowship fellowship_;

public:
    ResearchScholar(const Person& person, const std::string& programme, double cgpa,
                    const Fellowship& fellowship)
        : Person(person), Student(person, programme, cgpa), fellowship_(fellowship) {}
    const Fellowship& fellowship() const { return fellowship_; }
};

struct ComposedTA {
    Student student;
    Staff staff;
};

int main() {
    const Person profile("Anu", 201, "9000000201");
    TeachingAssistant ta(profile, "M.Tech", 9.1, "CSE", 12000);
    ResearchScholar scholar(profile, "PhD", 9.3, {"University Fellowship", 30000});
    ComposedTA composed{{profile, "M.Tech", 9.1}, {profile, "CSE", 12000}};

    const Person* studentIdentity = static_cast<const Student*>(&ta);
    const Person* staffIdentity = static_cast<const Staff*>(&ta);
    const Person* composedStudent = &composed.student;
    const Person* composedStaff = &composed.staff;

    std::cout << ta.name() << " (" << ta.id() << ", " << ta.phone() << ") | "
              << ta.programme() << ' ' << ta.cgpa() << " | "
              << ta.department() << ' ' << ta.salary() << '\n';
    std::cout << scholar.programme() << " | " << scholar.fellowship().scheme
              << ": " << scholar.fellowship().amount << '\n';
    std::cout << std::boolalpha
              << "Virtual diamond shares Person: " << (studentIdentity == staffIdentity) << '\n'
              << "Composition shares Person: " << (composedStudent == composedStaff) << '\n'
              << "sizeof(TeachingAssistant): " << sizeof(TeachingAssistant) << " bytes\n"
              << "sizeof(ComposedTA): " << sizeof(ComposedTA) << " bytes\n";
}
