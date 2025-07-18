// i'll be including comments in the code to explain each part
// project.cpp - GPA Simulation Project using C++ OOP
// so as to make reading the code easier
// we r using four functions of oop
// 1. Inheritance
// 2. Polymorphism
// 3. Encapsulation
// 4. Abstraction   





#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <memory>
#include <limits>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

// Base class representing a person
class Person {
protected:
    std::string name; // Protected member variable to store the name of the person
public:
    // Constructor to initialize the name
    Person(std::string n) : name(n) {}

    // Pure virtual function for printing details, enabling polymorphism
    virtual void printDetails() = 0; 
};

// Structure to represent a course
struct Course {
    std::string title; // Title of the course
    int unit;         // Number of units for the course
    int grade;        // Grade received in the course
};

// Student class inheriting from Person
class Student : public Person {
private:
    std::string studentID; // Student ID
    std::string level;     // Level of study (e.g., 100, 200)
    std::string semester;  // Semester (e.g., 1st, 2nd)
    std::string department; // Department (e.g., Computer Science)
    std::vector<Course> courses; // List of courses taken by the student

    int getGradePoint(int grade) {
        if (grade < 0 || grade > 5) {
            std::cout << "Invalid grade entered. Counting as 0.\n";          
            return 0; // Return 0 for invalid grades
        }
        return grade; // Return the valid grade
    }

    // Private method to classify GPA into categories
    std::string classify(double gpa) {
        if (gpa >= 4.5) return "First Class";
        else if (gpa >= 3.5) return "Second Class Upper";
        else if (gpa >= 2.5) return "Second Class Lower";
        else if (gpa >= 1.5) return "Third Class";
        else return "Pass";        
    }

    // NEW: Automatically calculate performance factor (0.7-1.3)
    double getPerformanceFactor() {
        double gpa = calculateGPA();
        
        if (gpa >= 4.5) return 0.92; // Small dip for top students
        else if (gpa >= 3.5) return 1.05; // Slight improvement
        else if (gpa >= 2.0) return 1.12; // Moderate improvement
        else return 1.25; // Big improvement for struggling students
    }

    // NEW: Simulate Grade Change (+/- 20% variation)
    int simulateGradeChange(int currentGrade) {
        int newGrade = currentGrade;
        
        int roll = rand() % 100;
        if (roll < 15 && newGrade < 5) newGrade++;
        else if (roll > 85 && newGrade > 0) newGrade--;
        
        return newGrade;
    }

public: 
    // Constructor to initialize student details
    Student(std::string n, std::string id, std::string lvl, std::string sem, std::string dept) 
        : Person(n), studentID(id), level(lvl), semester(sem), department(dept) {}

    // Method to convert numeric grade to letter grade
    std::string gradeToLetter(int grade) {
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

    // Method to add a course to the student's list
    void addCourse(std::string title, int unit, int grade) {
        courses.push_back({title, unit, grade}); 
    }

    // Method to calculate the GPA based on courses taken
    double calculateGPA() {
        int totalPoints = 0, totalUnits = 0; // Initialize total points and units
        for (const auto& c : courses) {
            totalPoints += getGradePoint(c.grade) * c.unit; // Calculate total points
            totalUnits += c.unit; // Calculate total units
        }
        return totalUnits ? (double)totalPoints / totalUnits : 0.0; // Return GPA
    }

    // NEW: Automatic GPA Projection
    void projectNextSemester() {
        double currentGPA = calculateGPA();
        double performanceFactor = getPerformanceFactor();
        double projectedGPA = currentGPA * performanceFactor;

        // Ensure GPA stays within bounds (0.0-5.0)
        if (projectedGPA > 5.0) projectedGPA = 5.0;
        if (projectedGPA < 0.0) projectedGPA = 0.0;

        // Detailed simulation log
        std::cout << "\n--- Next Semester Projection ---" << std::endl;
        std::cout << "Current GPA: " << std::fixed << std::setprecision(2) << currentGPA << std::endl;
        std::cout << "Performance Factor: " << performanceFactor << std::endl;
        std::cout << "Projected GPA: " << projectedGPA << std::endl;
        
        // Course-by-course simulation
        std::cout << "\nCourse Adjustments:" << std::endl;
        for (auto& course : courses) {
            int oldGrade = course.grade;
            int newGrade = simulateGradeChange(oldGrade);
            std::cout << course.title << ": " << gradeToLetter(oldGrade) 
                      << " -> " << gradeToLetter(newGrade) << std::endl;
        }

        // Write to simulation file
        std::ofstream simFile("gpa_projection.txt", std::ios::app);
        simFile << name << "'s Projection:\n";
        simFile << "Current: " << currentGPA << " -> Projected: " << projectedGPA << "\n";
    }

    // Override the printDetails method to display student information
    void printDetails() override {
        double gpa = calculateGPA(); // Calculate GPA
        std::cout << "\nGPA Report for " << name << " (" << studentID << "):\n";
        std::cout << "Level: " << level << "\n";
        std::cout << "Semester: " << semester << "\n";
        std::cout << "Department: " << department << "\n";
        
        std::cout << "\nCourses:\n";
        for (const auto& c : courses) {
            std::cout << " - " << c.title << " | Unit: " << c.unit << " | Grade: " 
                     << c.grade << " (" << gradeToLetter(c.grade) << ")\n";
        }
        
        std::cout << "\nGPA: " << std::fixed << std::setprecision(2) << gpa << "\n";
        std::cout << "Classification: " << classify(gpa) << "\n";

        // Write GPA report to a file
        std::ofstream out("gpa_report.txt", std::ios::app);
        out << "\nStudent: " << name << " (" << studentID << ")\n";
        out << "Level: " << level << "\n";
        out << "Semester: " << semester << "\n";
        out << "Department: " << department << "\n";
        for (const auto& c : courses) {
            out << " - " << c.title << " | Unit: " << c.unit << " Grade: " 
                << c.grade << " (" << gradeToLetter(c.grade) << ")\n";
        }
        out << "GPA: " << std::fixed << std::setprecision(2) << gpa
            << " - " << classify(gpa) << "\n";
    }
};

int main() {
    srand(time(0)); // NEW: Seed random number generator

    int n; // Number of students
    std::cout << "GPA Simulation with OOP\n"; // Program title
    std::cout << "Enter number of students: ";
    std::cin >> n; // Input number of students
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

    std::vector<std::unique_ptr<Student>> studentList; // Vector to hold student objects

    // Loop to input details for each student
    for (int i = 0; i < n; ++i) {
        std::string name, id, level, semester, department;
        std::cout << "\nEnter student name: ";
        std::getline(std::cin, name);
        std::cout << "Enter student ID: ";
        std::getline(std::cin, id);
        
        std::cout << "Enter student level (e.g., 100, 200, 300, 400): ";
        std::getline(std::cin, level);
        
        std::cout << "Enter student semester (e.g., 1st, 2nd, etc.): ";
        std::getline(std::cin, semester);
        
        std::cout << "Enter student department (e.g., Computer Science, Mathematics): ";
        std::getline(std::cin, department);

        // Create a new Student object and add it to the list
        auto student = std::make_unique<Student>(name, id, level, semester, department);

        int courseCount;
        std::cout << "Enter number of courses: ";
        std::cin >> courseCount;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        for (int j = 0; j < courseCount; ++j) {
            std::string title;
            int unit, grade;

            std::cout << "\nCourse " << (j + 1) << " name: ";
            std::getline(std::cin, title);

            std::cout << "Unit: ";
            std::cin >> unit;
            if (unit <= 0) {
                std::cout << "Invalid unit entered. Counting as 2.\n";
                unit = 2;
            }

            std::cout << "Enter grade (0 = F, ....., 5 = A): ";
            std::cin >> grade;

            if (grade < 0 || grade > 5) {
                std::cout << "Invalid grade entered. Counting as 0.\n";
                grade = 0;
            }
            std::cout << "You entered: " << grade << " (" << student->gradeToLetter(grade) << ")\n";

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            student->addCourse(title, unit, grade);
        }

        studentList.push_back(std::move(student));
    }

    // Loop to print details and simulate future GPA for each student
    for (const auto& s : studentList) {
        s->printDetails();
        s->projectNextSemester(); // NEW: Call the projection function
        std::cout << "\n----------------------\n";
    }

    std::cout << "\nGPA simulation complete.\n";
    std::cout << "Current GPA report saved to gpa_report.txt\n";
    std::cout << "Future GPA projections saved to gpa_projection.txt\n";

    std::cout << "\nPress Enter to exit...";
    std::cin.ignore();

    return 0;
}
