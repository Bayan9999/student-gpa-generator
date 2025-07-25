// trying with multiple students
// the highest gpa will bw 5 while the lowest will be 0
// 5 will be A wile 0 will be F
// im using 4 prnciples of oop(Encapsulation, Abstraction, Inheritance, Polymorphism)
 

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <memory>
#include <limits>

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

    // Private method to get grade points based on the grade
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
            default: return "Invalid"; // Return "Invalid" for out-of-range grades
        }
    }

    // Method to add a course to the student's list
    void addCourse(std::string title, int unit, int grade) {
        courses.push_back({title, unit, grade}); // Add course to the vector
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

    // Override the printDetails method to display student information
    void printDetails() override {
        double gpa = calculateGPA(); // Calculate GPA
        std::cout << "\nGPA Report for " << name << " (" << studentID << "):\n";
        std::cout << "Level: " << level << "\n"; // Display level
        std::cout << "Semester: " << semester << "\n"; // Display semester
        std::cout << "Department: " << department << "\n"; // Display department
        std::cout << "Courses: " << courses.size() << "\n"; // Display number of courses
        std::cout << "GPA: " << std::fixed << std::setprecision(2) << gpa << "\n"; // Display GPA
        std::cout << "Classification: " << classify(gpa) << "\n"; // Display classification

        // Write GPA report to a file
        std::ofstream out("gpa_report.txt", std::ios::app);
        out << "Student: " << name << " (" << studentID << ")\n";
        out << "Level: " << level << "\n";
        out << "Semester: " << semester << "\n";
        out << "Department: " << department << "\n";
        for (const auto& c : courses) {
            out << " - " << c.title << " | Unit: " << c.unit << " Grade: " 
                << c.grade << " (" << gradeToLetter(c.grade) << ")\n";
        }
        out << "GPA: " << std::fixed << std::setprecision(2) << gpa
            << " - " << classify(gpa) << "\n\n";
    }
};

int main() {
    int n; // Number of students
    std::cout << "GPA Simulation with OOP\n"; // Program title
    std::cout << "Enter number of students: ";
    std::cin >> n; // Input number of students
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

    std::vector<std::unique_ptr<Student>> studentList; // Vector to hold student objects

    // Loop to input details for each student
    for (int i = 0; i < n; ++i) {
        std::string name, id, level, semester, department; // Variables to hold student details
        std::cout << "\nEnter student name: "; // Prompt for student name
        std::getline(std::cin, name); // Read student name
        std::cout << "Enter student ID: "; // Prompt for student ID
        std::getline(std::cin, id); // Read student ID
        
        // Prompt for additional student details
        std::cout << "Enter student level (e.g., 100, 200, 300, 400): ";
        std::getline(std::cin, level);
        
        std::cout << "Enter student semester (e.g., 1st, 2nd, etc.): ";
        std::getline(std::cin, semester);
        
        std::cout << "Enter student department (e.g., Computer Science, Mathematics): ";
        std::getline(std::cin, department);

        // Create a new Student object and add it to the list
        auto student = std::make_unique<Student>(name, id, level, semester, department);

        int courseCount; // Number of courses
        std::cout << "Enter number of courses: "; // Prompt for number of courses
        std::cin >> courseCount; // Read number of courses
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
        
        // Loop to input details for each course
        for (int j = 0; j < courseCount; ++j) {
            std::string title; // Course title
            int unit; // Course unit
            int grade; // Course grade

            std::cout << "\nCourse name: "; // Prompt for course name
            std::getline(std::cin, title); // Read course name

            std::cout << "Unit: "; // Prompt for course unit
            std::cin >> unit; // Read course unit
            if (unit <= 0) {
                std::cout << "Invalid unit entered. Counting as 2.\n";
                unit = 2; // Default to 2 if invalid
            }

            std::cout << "Enter grade (0 = F, ....., 5 = A): "; // Prompt for grade
            std::cin >> grade; // Read grade

            if (grade < 0 || grade > 5) {
                std::cout << "Invalid grade entered. Counting as 0.\n";
                grade = 0; // Default to 0 if invalid
            }
            std::cout << "You entered: " << grade << " (" << student->gradeToLetter(grade) << ")\n";

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

            student->addCourse(title, unit, grade); // Add course to student
        }

        studentList.push_back(std::move(student)); // Add student to the list
    }

    // Loop to print details for each student
    for (const auto& s : studentList) {
        s->printDetails(); // Print student details
        std::cout << "----------------------\n";
    }
    std::cout << "\nGPA simulation complete.\n";
    std::cout << "GPA report saved to gpa_report.txt\n";

    std::cout << "\nPress Enter to exit...";
    std::cin.ignore(); // Wait for user input before exiting

    return 0; // End of program
}
