#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <memory>
#include <limits>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions

class Person {
protected:
    std::string name;
public:
    Person(std::string n) : name(n) {}
    virtual void printDetails() = 0; // Polymorphism
    std::string getName() const { return name; } // <-- Add this
};

struct Course {
    std::string title;
    int unit;
    int grade;
};

class Student : public Person {
private:
    std::string studentID;
    std::vector<Course> courses;

    int getGradePoint(int grade) {
        if (grade < 0 || grade > 5) {
            std::cout << "Invalid grade entered. Counting as 0.\n";
            return 0;
        }
        return grade;
    }

    std::string classify(double gpa) {
        if (gpa >= 4.5) return "First Class";
        else if (gpa >= 3.5) return "Second Class Upper";
        else if (gpa >= 2.5) return "Second Class Lower";
        else if (gpa >= 1.5) return "Third Class";
        else return "Pass";
    }

public:
    Student(std::string n, std::string id) : Person(n), studentID(id) {}

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

    void addCourse(std::string title, int unit, int grade) {
        courses.push_back({title, unit, grade});
    }

    double calculateGPA() {
        int totalPoints = 0, totalUnits = 0;
        for (const auto& c : courses) {
            totalPoints += getGradePoint(c.grade) * c.unit;
            totalUnits += c.unit;
        }
        return totalUnits ? (double)totalPoints / totalUnits : 0.0;
    }

    void printDetails() override {
        double gpa = calculateGPA();
        std::cout << "\nGPA Report for " << name << " (" << studentID << "):\n";
        std::cout << "Courses: " << courses.size() << "\n";
        std::cout << "GPA: " << std::fixed << std::setprecision(2) << gpa << "\n";
        std::cout << "Classification: " << classify(gpa) << "\n";

        std::cout << "\nCourse breakdown:\n";
        for (const auto& c : courses) {
            std::cout << " - " << c.title << " | Unit: " << c.unit
                      << " | Grade: " << c.grade << " (" << gradeToLetter(c.grade) << ")\n";
        }
    }
};

static std::vector<std::unique_ptr<Student>> studentList;
static char studentName[64] = "";
static char studentID[32] = "";
static char courseTitle[64] = "";
static int courseUnit = 2;
static int courseGrade = 5;

void RenderGPAApp() {
    ImGui::Begin("GPA Simulator");

    ImGui::InputText("Student Name", studentName, IM_ARRAYSIZE(studentName));
    ImGui::InputText("Student ID", studentID, IM_ARRAYSIZE(studentID));

    ImGui::Separator();
    ImGui::InputText("Course Title", courseTitle, IM_ARRAYSIZE(courseTitle));
    ImGui::InputInt("Unit", &courseUnit);
    ImGui::SliderInt("Grade (0 = F, ..., 5 = A)", &courseGrade, 0, 5);

    if (ImGui::Button("Add Course")) {
        if (courseUnit <= 0) {
            std::cout << "Invalid unit entered. Counting as 2.\n";
            courseUnit = 2;
        }
        if (courseGrade < 0 || courseGrade > 5) {
            std::cout << "Invalid grade entered. Counting as 0.\n";
            courseGrade = 0;
        }
        if (studentList.empty() || studentList.back()-> getName() != studentName) {
            studentList.push_back(std::make_unique<Student>(studentName, studentID));
        }
        studentList.back()->addCourse(courseTitle, courseUnit, courseGrade);
        // Clear input fields after adding
        courseTitle[0] = '\0';
        courseUnit = 2;
        courseGrade = 5;
    }

    if (ImGui::Button("Calculate GPA")) {
        for (const auto& student : studentList) {
            student->printDetails();
        }
    }

    ImGui::End();
}

int main(int, char**) {
    // Setup GLFW
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(1280, 800, "GPA Simulator", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        RenderGPAApp();

        // Rendering
        ImGui::Render();
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
