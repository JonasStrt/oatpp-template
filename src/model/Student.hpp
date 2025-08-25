#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
#include <memory>
#include <vector>
#include <iostream>

namespace model {

/**
 * Student-Klasse - Lektion 1: Objektorientierung und Pointer Management
 * 
 * Diese Klasse demonstriert:
 * - Konstruktoren (Default, Parameter, Copy, Move)
 * - Destruktor
 * - Getter/Setter
 * - Rule of Five
 * - Smart Pointers
 */
class Student {
private:
    // Basis-Attribute
    int id;
    std::string firstName;
    std::string lastName;
    int age;
    double gpa; // Grade Point Average
    
    // Zeiger auf Zusatzdaten (demonstriert Pointer Management)
    std::unique_ptr<std::vector<std::string>> courses;
    std::shared_ptr<std::string> universityName;

public:
    // Konstruktoren
    
    /**
     * Default Konstruktor
     */
    Student();
    
    /**
     * Parameter Konstruktor
     */
    Student(int id, const std::string& firstName, const std::string& lastName, 
            int age, double gpa);
    
    /**
     * Copy Konstruktor (Rule of Five)
     */
    Student(const Student& other);
    
    /**
     * Move Konstruktor (Rule of Five)
     */
    Student(Student&& other) noexcept;
    
    /**
     * Destruktor (Rule of Five)
     */
    ~Student();
    
    /**
     * Copy Assignment Operator (Rule of Five)
     */
    Student& operator=(const Student& other);
    
    /**
     * Move Assignment Operator (Rule of Five)
     */
    Student& operator=(Student&& other) noexcept;
    
    // Getter-Methoden
    int getId() const { return id; }
    const std::string& getFirstName() const { return firstName; }
    const std::string& getLastName() const { return lastName; }
    int getAge() const { return age; }
    double getGpa() const { return gpa; }
    
    // Setter-Methoden
    void setId(int newId) { id = newId; }
    void setFirstName(const std::string& name) { firstName = name; }
    void setLastName(const std::string& name) { lastName = name; }
    void setAge(int newAge) { age = newAge; }
    void setGpa(double newGpa) { gpa = newGpa; }
    
    // Methoden für Course Management (Smart Pointer Demonstration)
    void addCourse(const std::string& courseName);
    void removeCourse(const std::string& courseName);
    std::vector<std::string> getCourses() const;
    size_t getCourseCount() const;
    
    // University Management (Shared Pointer Demonstration)
    void setUniversity(std::shared_ptr<std::string> university);
    std::string getUniversity() const;
    
    // Utility-Methoden
    std::string getFullName() const;
    void printStudentInfo() const;
    
    // Operator Overloading für Vergleiche
    bool operator==(const Student& other) const;
    bool operator<(const Student& other) const; // Für Sortierung
    
    // Friend-Funktion für Stream-Output
    friend std::ostream& operator<<(std::ostream& os, const Student& student);
};

} // namespace model

#endif // STUDENT_HPP
