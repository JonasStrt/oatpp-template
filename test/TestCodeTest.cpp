#include "TestCodeTest.hpp"
#include "model/TestCode.hpp"
#include <memory>
#include <vector>
#include <algorithm>

void TestCodeTest::onRun() {
    testAddToArray();
    // Hier kannst du weitere Test-Aufrufe hinzufügen
}

/**
 * Test 1: addToArray Funktion testen
 */
void TestCodeTest::testAddToArray() {
    std::cout << "\n=== Test 1: Test function - addToArray ===" << std::endl;
    
    // TestCode Instanz erstellen
    model::TestCode testcode;
    
    // Test mit leerem Array
    int* array = nullptr;
    size_t length = 0;
    
    // Werte hinzufügen
    testcode.addToArray(array, length, 10);
    OATPP_ASSERT(length == 1);
    OATPP_ASSERT(array != nullptr);
    OATPP_ASSERT(array[0] == 10);
    
    std::cout << "✓ Erstes Element erfolgreich hinzugefügt" << std::endl;
    
    // Zweites Element hinzufügen
    testcode.addToArray(array, length, 20);
    OATPP_ASSERT(length == 2);
    OATPP_ASSERT(array[0] == 10);
    OATPP_ASSERT(array[1] == 20);
    
    std::cout << "✓ Zweites Element erfolgreich hinzugefügt" << std::endl;
    
    // Drittes Element hinzufügen
    testcode.addToArray(array, length, 30);
    OATPP_ASSERT(length == 3);
    OATPP_ASSERT(array[0] == 10);
    OATPP_ASSERT(array[1] == 20);
    OATPP_ASSERT(array[2] == 30);
    
    std::cout << "✓ Drittes Element erfolgreich hinzugefügt" << std::endl;
    
    // Array-Inhalt ausgeben
    std::cout << "Array Inhalt: ";
    for (size_t i = 0; i < length; ++i) {
        std::cout << array[i];
        if (i < length - 1) std::cout << ", ";
    }
    std::cout << std::endl;
    
    // Memory cleanup
    delete[] array;
    
    std::cout << "✓ addToArray Test erfolgreich abgeschlossen!" << std::endl;
}
