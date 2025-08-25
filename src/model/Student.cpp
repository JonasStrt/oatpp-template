#include "Student.hpp"
#include <algorithm>
#include <iterator>

namespace model {

// Default Konstruktor
Student::Student() 
    : id(0), firstName(""), lastName(""), age(0), gpa(0.0),
      courses(std::make_unique<std::vector<std::string>>()),
      universityName(nullptr) {
    std::cout << "Student Default Konstruktor aufgerufen für ID: " << id << std::endl;
}

// Parameter Konstruktor
Student::Student(int id, const std::string& firstName, const std::string& lastName, 
                int age, double gpa)
    : id(id), firstName(firstName), lastName(lastName), age(age), gpa(gpa),
      courses(std::make_unique<std::vector<std::string>>()),
      universityName(nullptr) {
    std::cout << "Student Parameter Konstruktor aufgerufen für: " << getFullName() << std::endl;
}

// Copy Konstruktor (Deep Copy)
Student::Student(const Student& other)
    : id(other.id), firstName(other.firstName), lastName(other.lastName),
      age(other.age), gpa(other.gpa),
      courses(std::make_unique<std::vector<std::string>>(*other.courses)),
      universityName(other.universityName) { // Shared pointer wird geteilt
    std::cout << "Student Copy Konstruktor aufgerufen für: " << getFullName() << std::endl;
}

// Move Konstruktor
Student::Student(Student&& other) noexcept
    : id(other.id), firstName(std::move(other.firstName)), lastName(std::move(other.lastName)),
      age(other.age), gpa(other.gpa),
      courses(std::move(other.courses)),
      universityName(std::move(other.universityName)) {
    
    // Other objekt in validen aber undefinierten Zustand versetzen
    other.id = 0;
    other.age = 0;
    other.gpa = 0.0;
    
    std::cout << "Student Move Konstruktor aufgerufen für: " << getFullName() << std::endl;
}

// Destruktor
Student::~Student() {
    std::cout << "Student Destruktor aufgerufen für: " << getFullName() << std::endl;
    // Smart Pointers räumen automatisch auf!
}

// Copy Assignment Operator
Student& Student::operator=(const Student& other) {
    if (this != &other) { // Self-assignment check
        id = other.id;
        firstName = other.firstName;
        lastName = other.lastName;
        age = other.age;
        gpa = other.gpa;
        
        // Deep copy für courses
        courses = std::make_unique<std::vector<std::string>>(*other.courses);
        
        // Shared pointer wird geteilt
        universityName = other.universityName;
        
        std::cout << "Student Copy Assignment für: " << getFullName() << std::endl;
    }
    return *this;
}

// Move Assignment Operator
Student& Student::operator=(Student&& other) noexcept {
    if (this != &other) { // Self-assignment check
        id = other.id;
        firstName = std::move(other.firstName);
        lastName = std::move(other.lastName);
        age = other.age;
        gpa = other.gpa;
        
        courses = std::move(other.courses);
        universityName = std::move(other.universityName);
        
        // Other objekt zurücksetzen
        other.id = 0;
        other.age = 0;
        other.gpa = 0.0;
        
        std::cout << "Student Move Assignment für: " << getFullName() << std::endl;
    }
    return *this;
}

// Course Management Methoden
void Student::addCourse(const std::string& courseName) {
    if (courses) {
        // Prüfen ob Kurs bereits existiert
        auto it = std::find(courses->begin(), courses->end(), courseName);
        if (it == courses->end()) {
            courses->push_back(courseName);
            std::cout << "Kurs '" << courseName << "' hinzugefügt für " << getFullName() << std::endl;
        } else {
            std::cout << "Kurs '" << courseName << "' bereits vorhanden für " << getFullName() << std::endl;
        }
    }
}

void Student::removeCourse(const std::string& courseName) {
    if (courses) {
        auto it = std::find(courses->begin(), courses->end(), courseName);
        if (it != courses->end()) {
            courses->erase(it);
            std::cout << "Kurs '" << courseName << "' entfernt für " << getFullName() << std::endl;
        } else {
            std::cout << "Kurs '" << courseName << "' nicht gefunden für " << getFullName() << std::endl;
        }
    }
}

std::vector<std::string> Student::getCourses() const {
    if (courses) {
        return *courses; // Copy der Kurse zurückgeben
    }
    return std::vector<std::string>(); // Leeren Vector zurückgeben
}

size_t Student::getCourseCount() const {
    return courses ? courses->size() : 0;
}

// University Management
void Student::setUniversity(std::shared_ptr<std::string> university) {
    universityName = university;
    std::cout << "Universität gesetzt für " << getFullName();
    if (university) {
        std::cout << ": " << *university;
    }
    std::cout << std::endl;
}

std::string Student::getUniversity() const {
    return universityName ? *universityName : "Keine Universität";
}

// Utility Methoden
std::string Student::getFullName() const {
    return firstName + " " + lastName;
}

void Student::printStudentInfo() const {
    std::cout << "\n=== Student Information ===" << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "Name: " << getFullName() << std::endl;
    std::cout << "Alter: " << age << std::endl;
    std::cout << "GPA: " << gpa << std::endl;
    std::cout << "Universität: " << getUniversity() << std::endl;
    std::cout << "Kurse (" << getCourseCount() << "):" << std::endl;
    
    if (courses && !courses->empty()) {
        for (const auto& course : *courses) {
            std::cout << "  - " << course << std::endl;
        }
    } else {
        std::cout << "  Keine Kurse eingetragen" << std::endl;
    }
    std::cout << "=========================" << std::endl;
}

// Operator Overloading
bool Student::operator==(const Student& other) const {
    return id == other.id; // Studenten sind gleich wenn IDs gleich sind
}

bool Student::operator<(const Student& other) const {
    // Sortierung nach GPA (absteigend), dann nach Name
    if (gpa != other.gpa) {
        return gpa > other.gpa; // Höhere GPA zuerst
    }
    return getFullName() < other.getFullName();
}

// Stream Output Operator
std::ostream& operator<<(std::ostream& os, const Student& student) {
    os << "Student[ID:" << student.id 
       << ", Name:" << student.getFullName()
       << ", Age:" << student.age
       << ", GPA:" << student.gpa
       << ", Courses:" << student.getCourseCount() << "]";
    return os;
}

} // namespace model
