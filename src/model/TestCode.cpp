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

    void TestCode::floatToBinary(float* f) {
        char* b = reinterpret_cast<char*>(f);
        for(int i=0;i<4;i++) {
            char value = b[3-i];
            for(int i = 0; i< 8; i++) {
                std::cout << ((value >> (7-i)) & 1);
            }
        }
    }




}