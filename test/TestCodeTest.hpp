#ifndef TestCodeTest_hpp
#define TestCodeTest_hpp

#include "oatpp-test/UnitTest.hpp"

/**
 * TestCode Unit Test - Professionelle Test-Architektur
 * 
 * Dieser Test folgt der gleichen Struktur wie MyControllerTest und StudentTest:
 * - Erbt von oatpp::test::UnitTest
 * - Separate Header- und Implementation-Dateien
 * - Verwendet das OATPP_RUN_TEST Makro
 * - Saubere Trennung von Test-Definition und -Implementation
 * - Tests für deine bestehende model::TestCode Klasse
 */
class TestCodeTest : public oatpp::test::UnitTest {
public:
    TestCodeTest() : UnitTest("TEST[TestCodeTest]") {}
    
    void onRun() override;

private:
    // Private Test-Methoden für bessere Organisation
    void testAddToArray();
    void testFloatToBinary();
    // Hier kannst du weitere Test-Methoden hinzufügen
};

#endif // TestCodeTest_hpp
