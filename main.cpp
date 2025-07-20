// main.cpp

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <memory>
#include <limits>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

// ---------- [Futuristic Theme Setup] ----------
void SetupFuturisticTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 12.0f;
    style.FrameRounding = 8.0f;
    style.FramePadding = ImVec2(12, 8);
    style.ItemSpacing = ImVec2(10, 12);
    style.WindowPadding = ImVec2(20, 20);
    style.Colors[ImGuiCol_WindowBg]         = ImVec4(0.08f, 0.10f, 0.12f, 0.95f);
    style.Colors[ImGuiCol_TitleBg]          = ImVec4(0.10f, 0.12f, 0.15f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive]    = ImVec4(0.0f, 0.75f, 0.95f, 1.0f);
    style.Colors[ImGuiCol_Button]           = ImVec4(0.15f, 0.2f, 0.25f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered]    = ImVec4(0.0f, 0.75f, 0.95f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive]     = ImVec4(0.0f, 0.6f, 0.8f, 1.0f);
    style.Colors[ImGuiCol_FrameBg]          = ImVec4(0.15f, 0.18f, 0.22f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered]   = ImVec4(0.0f, 0.75f, 0.95f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive]    = ImVec4(0.0f, 0.6f, 0.8f, 1.0f);
    style.Colors[ImGuiCol_Separator]        = ImVec4(0.0f, 0.75f, 0.95f, 1.0f);
}

// ---------- [Core Classes: Person & Student] ----------
class Person {
protected:
    std::string name;
public:
    Person(std::string n) : name(n) {}
    virtual void printDetails() = 0;
    std::string getName() const { return name; }
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
        std::cout << "\n📊 GPA Report for " << name << " (" << studentID << "):\n";
        std::cout << "Courses: " << courses.size() << "\n";
        std::cout << "GPA: " << std::fixed << std::setprecision(2) << gpa << "\n";
        std::cout << "Classification: " << classify(gpa) << "\n";

        std::cout << "\nCourse Breakdown:\n";
        for (const auto& c : courses) {
            std::cout << " - " << c.title << " | Unit: " << c.unit
                      << " | Grade: " << c.grade << " (" << gradeToLetter(c.grade) << ")\n";
        }
    }
};

// ---------- [Global Variables] ----------
static std::vector<std::unique_ptr<Student>> studentList;
static char studentName[64] = "";
static char studentID[32] = "";
static char courseTitle[64] = "";
static int courseUnit = 2;
static int courseGrade = 5;

// ---------- [GUI Rendering Function] ----------
void RenderGPAApp() {
    ImGui::Begin("🔮 GPA Simulator - Futuristic Edition", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::SetWindowFontScale(1.2f);

    ImGui::TextColored(ImVec4(0.0f, 0.75f, 0.95f, 1.0f), "🚀 Student Information");
    ImGui::SeparatorText("Personal Data");
    ImGui::InputText("Full Name", studentName, IM_ARRAYSIZE(studentName));
    ImGui::InputText("Student ID", studentID, IM_ARRAYSIZE(studentID));

    ImGui::Spacing();
    ImGui::SeparatorText("📚 Add Course");
    ImGui::InputText("Course Title", courseTitle, IM_ARRAYSIZE(courseTitle));
    ImGui::SliderInt("Course Unit", &courseUnit, 1, 6);
    ImGui::SliderInt("Grade (0=F - 5=A)", &courseGrade, 0, 5);

    ImGui::Spacing();
    if (ImGui::Button("➕ Add Course", ImVec2(200, 40))) {
        if (courseUnit <= 0) courseUnit = 2;
        if (courseGrade < 0 || courseGrade > 5) courseGrade = 0;
        if (studentList.empty() || studentList.back()->getName() != studentName) {
            studentList.push_back(std::make_unique<Student>(studentName, studentID));
        }
        studentList.back()->addCourse(courseTitle, courseUnit, courseGrade);
        courseTitle[0] = '\0';
        courseUnit = 2;
        courseGrade = 5;
    }

    ImGui::Spacing();
    if (ImGui::Button("⚙️ Calculate GPA", ImVec2(200, 40))) {
        for (const auto& student : studentList) {
            student->printDetails();
        }
    }

    ImGui::Spacing();
    ImGui::SeparatorText("🗂️ Summary");

    if (!studentList.empty()) {
        const auto& student = studentList.back();
        double gpa = student->calculateGPA();
        ImGui::Text("Student: %s", student->getName().c_str());
        ImGui::Text("GPA: %.2f", gpa);
    } else {
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "No student data yet.");
    }

    ImGui::End();
}

// ---------- [Main Function & ImGui Setup] ----------
int main(int, char**) {
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* window = glfwCreateWindow(1280, 800, "🔮 GPA Simulator - Futuristic Edition", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    SetupFuturisticTheme();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        RenderGPAApp();

        ImGui::Render();
        glClearColor(0.08f, 0.10f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
