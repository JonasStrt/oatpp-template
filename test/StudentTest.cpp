#include "StudentTest.hpp"
#include "model/Student.hpp"
#include <memory>
#include <vector>
#include <algorithm>

void StudentTest::onRun() {
    testBasicConstructors();
    testCopyAndMoveSemantics();
    testSmartPointerManagement();
    testCourseManagement();
    testUniversitySharedPointer();
    testOperatorOverloading();
    testContainerOperations();
}

/**
 * Test 1: Basis Konstruktoren und Getter/Setter
 */
void StudentTest::testBasicConstructors() {
    std::cout << "\n=== Test 1: Basic Constructors ===" << std::endl;
    
    // Default Konstruktor
    model::Student student1;
    OATPP_ASSERT(student1.getId() == 0);
    OATPP_ASSERT(student1.getFirstName() == "");
    OATPP_ASSERT(student1.getAge() == 0);
    
    // Parameter Konstruktor
    model::Student student2(1, "Max", "Mustermann", 22, 3.7);
    OATPP_ASSERT(student2.getId() == 1);
    OATPP_ASSERT(student2.getFirstName() == "Max");
    OATPP_ASSERT(student2.getLastName() == "Mustermann");
    OATPP_ASSERT(student2.getAge() == 22);
    OATPP_ASSERT(student2.getGpa() == 3.7);
    OATPP_ASSERT(student2.getFullName() == "Max Mustermann");
    
    // Setter testen
    student1.setId(999);
    student1.setFirstName("Anna");
    student1.setLastName("Schmidt");
    student1.setAge(20);
    student1.setGpa(3.9);
    
    OATPP_ASSERT(student1.getId() == 999);
    OATPP_ASSERT(student1.getFullName() == "Anna Schmidt");
    
    student2.printStudentInfo();
}

/**
 * Test 2: Copy und Move Semantics (Rule of Five)
 */
void StudentTest::testCopyAndMoveSemantics() {
    std::cout << "\n=== Test 2: Copy and Move Semantics ===" << std::endl;
    
    // Original Student erstellen
    model::Student original(1, "John", "Doe", 25, 3.5);
    original.addCourse("Mathematics");
    original.addCourse("Physics");
    
    // Copy Konstruktor testen
    model::Student copied(original);
    OATPP_ASSERT(copied.getId() == original.getId());
    OATPP_ASSERT(copied.getFullName() == original.getFullName());
    OATPP_ASSERT(copied.getCourseCount() == original.getCourseCount());
    
    // Unabhängigkeit der Kopie testen
    copied.addCourse("Chemistry");
    OATPP_ASSERT(copied.getCourseCount() == 3);
    OATPP_ASSERT(original.getCourseCount() == 2); // Original unverändert
    
    // Move Konstruktor testen
    model::Student moved(std::move(copied));
    OATPP_ASSERT(moved.getCourseCount() == 3);
    OATPP_ASSERT(moved.getFullName() == "John Doe");
    
    // Copy Assignment testen
    model::Student assigned;
    assigned = original;
    OATPP_ASSERT(assigned.getFullName() == "John Doe");
    OATPP_ASSERT(assigned.getCourseCount() == 2);
    
    // Move Assignment testen
    model::Student moveAssigned;
    model::Student temp(2, "Jane", "Smith", 23, 3.8);
    temp.addCourse("Biology");
    moveAssigned = std::move(temp);
    OATPP_ASSERT(moveAssigned.getFullName() == "Jane Smith");
    OATPP_ASSERT(moveAssigned.getCourseCount() == 1);
}

/**
 * Test 3: Smart Pointer Management
 */
void StudentTest::testSmartPointerManagement() {
    std::cout << "\n=== Test 3: Smart Pointer Management ===" << std::endl;
    
    // Unique Pointer wird intern für courses verwendet
    model::Student student(1, "Alice", "Wonder", 21, 3.6);
    
    // Courses hinzufügen (demonstriert unique_ptr usage)
    student.addCourse("Data Structures");
    student.addCourse("Algorithms");
    student.addCourse("Computer Networks");
    
    OATPP_ASSERT(student.getCourseCount() == 3);
    
    auto courses = student.getCourses();
    OATPP_ASSERT(courses.size() == 3);
    OATPP_ASSERT(std::find(courses.begin(), courses.end(), "Algorithms") != courses.end());
    
    // Course entfernen
    student.removeCourse("Data Structures");
    OATPP_ASSERT(student.getCourseCount() == 2);
    
    // Nicht existierenden Course entfernen
    student.removeCourse("NonExistent");
    OATPP_ASSERT(student.getCourseCount() == 2); // Unverändert
}

/**
 * Test 4: Course Management Details
 */
void StudentTest::testCourseManagement() {
    std::cout << "\n=== Test 4: Course Management ===" << std::endl;
    
    model::Student student(1, "Bob", "Builder", 24, 3.2);
    
    // Mehrere Courses hinzufügen
    std::vector<std::string> coursesToAdd = {
        "Object Oriented Programming",
        "Database Systems", 
        "Software Engineering",
        "Machine Learning"
    };
    
    for (const auto& course : coursesToAdd) {
        student.addCourse(course);
    }
    
    OATPP_ASSERT(student.getCourseCount() == 4);
    
    // Duplikat hinzufügen (sollte ignoriert werden)
    student.addCourse("Database Systems");
    OATPP_ASSERT(student.getCourseCount() == 4); // Unverändert
    
    // Alle Courses abrufen und prüfen
    auto courses = student.getCourses();
    for (const auto& expectedCourse : coursesToAdd) {
        OATPP_ASSERT(std::find(courses.begin(), courses.end(), expectedCourse) != courses.end());
    }
    
    student.printStudentInfo();
}

/**
 * Test 5: Shared Pointer für University
 */
void StudentTest::testUniversitySharedPointer() {
    std::cout << "\n=== Test 5: Shared Pointer University ===" << std::endl;
    
    // Shared pointer für Universität erstellen
    auto university = std::make_shared<std::string>("Technische Universität München");
    
    model::Student student1(1, "Klaus", "Müller", 23, 3.4);
    model::Student student2(2, "Lisa", "Weber", 22, 3.8);
    
    // Beide Studenten teilen sich die gleiche Universität
    student1.setUniversity(university);
    student2.setUniversity(university);
    
    OATPP_ASSERT(student1.getUniversity() == "Technische Universität München");
    OATPP_ASSERT(student2.getUniversity() == "Technische Universität München");
    
    // Reference count sollte 3 sein (university + 2 studenten)
    OATPP_ASSERT(university.use_count() == 3);
    
    // Student kopieren - shared_ptr wird geteilt
    model::Student student3 = student1;
    OATPP_ASSERT(student3.getUniversity() == "Technische Universität München");
    OATPP_ASSERT(university.use_count() == 4);
    
    // Universität ändern
    *university = "Ludwig-Maximilians-Universität München";
    OATPP_ASSERT(student1.getUniversity() == "Ludwig-Maximilians-Universität München");
    OATPP_ASSERT(student2.getUniversity() == "Ludwig-Maximilians-Universität München");
    OATPP_ASSERT(student3.getUniversity() == "Ludwig-Maximilians-Universität München");
}

/**
 * Test 6: Operator Overloading
 */
void StudentTest::testOperatorOverloading() {
    std::cout << "\n=== Test 6: Operator Overloading ===" << std::endl;
    
    model::Student student1(1, "Alpha", "Test", 20, 3.8);
    model::Student student2(1, "Alpha", "Test", 20, 3.8); // Gleiche ID
    model::Student student3(2, "Beta", "Test", 21, 3.9);
    
    // Equality operator
    OATPP_ASSERT(student1 == student2); // Gleiche ID
    OATPP_ASSERT(!(student1 == student3)); // Verschiedene ID
    
    // Less than operator (für Sortierung)
    OATPP_ASSERT(!(student1 < student3)); // student3 hat höhere GPA, also student1 ist NICHT kleiner
    OATPP_ASSERT(student3 < student1); // student1 hat niedrigere GPA, also ist student3 kleiner in unserer Sortierung
    
    // Stream operator
    std::cout << "Student Output: " << student1 << std::endl;
    std::cout << "Student Output: " << student3 << std::endl;
}

/**
 * Test 7: Container Operations (Vector, Sorting, etc.)
 */
void StudentTest::testContainerOperations() {
    std::cout << "\n=== Test 7: Container Operations ===" << std::endl;
    
    // Vector von Studenten erstellen
    std::vector<model::Student> students;
    
    students.emplace_back(1, "Charlie", "Brown", 22, 3.2);
    students.emplace_back(2, "Alice", "Smith", 21, 3.9);
    students.emplace_back(3, "Bob", "Jones", 23, 3.5);
    students.emplace_back(4, "Diana", "Miller", 20, 3.7);
    
    OATPP_ASSERT(students.size() == 4);
    
    // Sortieren nach GPA (höchste zuerst)
    std::sort(students.begin(), students.end());
    
    // Prüfen der Sortierung
    OATPP_ASSERT(students[0].getFirstName() == "Alice"); // Höchste GPA (3.9)
    OATPP_ASSERT(students[1].getFirstName() == "Diana"); // 3.7
    OATPP_ASSERT(students[2].getFirstName() == "Bob");   // 3.5
    OATPP_ASSERT(students[3].getFirstName() == "Charlie"); // 3.2
    
    std::cout << "Sortierte Studenten:" << std::endl;
    for (const auto& student : students) {
        std::cout << "  " << student << std::endl;
    }
    
    // Student mit bestimmter ID finden
    auto it = std::find_if(students.begin(), students.end(), 
        [](const model::Student& s) { return s.getId() == 2; });
    
    OATPP_ASSERT(it != students.end());
    OATPP_ASSERT(it->getFirstName() == "Alice");
    
    // Studenten mit GPA > 3.5 filtern
    std::vector<model::Student> highPerformers;
    std::copy_if(students.begin(), students.end(), 
                std::back_inserter(highPerformers),
                [](const model::Student& s) { return s.getGpa() > 3.5; });
    
    OATPP_ASSERT(highPerformers.size() == 2); // Alice und Diana
    
    std::cout << "High Performers (GPA > 3.5):" << std::endl;
    for (const auto& student : highPerformers) {
        std::cout << "  " << student << std::endl;
    }
}
