#include "TestCode.hpp"
#include <algorithm>
#include <iterator>
#include <iostream>

namespace model {
    void TestCode::addToArray(int*& array, size_t& length, int value) {
        // Neues Array mit erhöhter Größe erstellen
        int* newArray = new int[length + 1];
        
        // Bestehende Elemente kopieren
        for (size_t i = 0; i < length; ++i) {
            newArray[i] = array[i];
        }
        
        // Neues Element hinzufügen
        newArray[length] = value;
        
        // Altes Array löschen (falls vorhanden)
        delete[] array;
        
        // Pointer und Länge aktualisieren
        array = newArray;
        length = length + 1;
        
        std::cout << "Wert " << value << " zum Array hinzugefügt. Neue Länge: " << length << std::endl;
    }
}