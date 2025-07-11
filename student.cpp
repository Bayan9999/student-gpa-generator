// trying with multiple students
// the highest gpa will bw 5 while the lowest will be 0
// 5 will be A wile 0 will be F
// im using 4 prnciples of oop(Encapsulation, Abstraction, Ingeritance, Polymorphism)
 
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iomanip>
#include <fstream>
#include <memory>
#include <cstdlib> // for rand()
#include <limits>

class Person {
protected:
    std::string name;
public:
    Person(std::string n) : name(n){}
 
    virtual void printDetails() = 0; //polymorphism
};
struct Course {
    std::string title;
    int unit;
    int grade;
};
class Student : public Person{  // im declaring a class to get the student grade and return 
    private:
        std::string studentID;
        std::vector<Course> courses;
        int getGradePoint(int grade) {
            if (grade < 0 || grade > 5 ){
                std::cout << "Invalid grade entered. Counting as 0.\n";
                return 0;
            }
            return grade;
        }
        std::string classify(double gpa){
            if (gpa >= 4.5) return "First Class";
            else if (gpa >= 3.5) return "Second Class Upper";
            else if (gpa >= 2.5) return "Second Class Lower";
            else if (gpa >= 1.5) return "Third Class";
            else return "Pass";
        }
    public:
        Student(std::string n, std::string id) : Person(n), studentID(id) {}

        std::string gradeToLetter(int grade){
            switch (grade) {
                case 5: return "A";
                case 4: return "B";
                case 3: return "C";
                case 2: return "D";
                case 1: return "E";
                case 0: return "F";
                default: return "Invalid";
            }

        }

        void addCourse(std::string title, int unit, int grade){
            courses.push_back({title, unit, grade});
        }

        double calculateGPA(){
            int totalPoints = 0, totalUnits =0;
            for (const auto& c: courses) {
                totalPoints += getGradePoint(c.grade) * c.unit;
                totalUnits += c.unit;
            }
            return totalUnits ? (double)totalPoints / totalUnits : 0.0;
        }

        void printDetails() override {
            double gpa = calculateGPA();
            std::cout << "\n GPA Report for " << name << " (" << studentID << "):\n";
            std::cout << "Courses :" << courses.size() << "\n";
            std::cout << "GPA: " << std::fixed << std::setprecision(2) << gpa << "\n";
            std::cout << "Classification: " << classify(gpa) << "\n";

            std::ofstream out("gpa_report.txt", std::ios::app);
            out << name << "(" << studentID << ") - GPA: " << std::fixed << std::setprecision(2)
               << gpa << "-"  << classify(gpa) << "\n";
            
        }
};



int main() {
    int n;
    std::cout << "GPA Simulation with OOP\n"; // This is a simple program to simulate the GPA calculation for students using OOP principles
    std::cout << "Enter number of students: ";
    std::cin >> n;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<std::unique_ptr<Student>> studentList;

    for (int i = 0; i < n; ++i) {
        std::string name, id;
        std::cout << "\nEnter student name: "; // let the user to enter the name of the student
        std::getline(std::cin, name);
        std::cout << "Enter student ID: ";// let the user to enter the id of the student
        std::getline(std::cin, id);
        auto student = std::make_unique<Student>(name, id);

        int courseCount;
        std::cout << "Enter number of courses: "; // let the user to enter the number of courses the student is taking
        std::cin >> courseCount;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        for (int j = 0; j < courseCount; ++j) {
            std::string title;
            int unit;
            int grade;

            std::cout << "\nCourse name: "; // let the user to enter the name of the course
            std::getline(std::cin, title);
            std::cout << "Unit: "; // let the user to enter the unit of the course(it can be 2 or 3 ion really care)
            std::cin >> unit;

            std::cout << "Enter grade(0 = F, ....., 5 = A): ";
            std::cin >> grade;

            if (grade < 0 || grade > 5) {
                std::cout << "Invalid grade entered. Counting as 0.\n";
                grade = 0;
            }
              std::cout << "You entered: " << grade << "(" << student ->gradeToLetter(grade) << ")\n";


            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            student->addCourse(title, unit, grade);
        }

    

        studentList.push_back(std::move(student));
    }

    for ( const auto& s : studentList) {
        s->printDetails();
        std::cout << "----------------------\n";
    }
    std::cout << "\n GPA simulation complete.\n";
    std::cout << "GPA report saved to gpa_report.txt\n";

    std::cout << "\nPress Enter to exit...";
    std::cin.ignore();


    return 0;
}