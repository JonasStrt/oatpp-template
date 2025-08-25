#ifndef StudentTest_hpp
#define StudentTest_hpp

#include "oatpp-test/UnitTest.hpp"

/**
 * Student Unit Test - Professionelle Test-Architektur
 * 
 * Dieser Test folgt der gleichen Struktur wie MyControllerTest:
 * - Erbt von oatpp::test::UnitTest
 * - Separate Header- und Implementation-Dateien
 * - Verwendet das OATPP_RUN_TEST Makro
 * - Saubere Trennung von Test-Definition und -Implementation
 */
class StudentTest : public oatpp::test::UnitTest {
public:
    StudentTest() : UnitTest("TEST[StudentTest]") {}
    
    void onRun() override;

private:
    // Private Test-Methoden für bessere Organisation
    void testBasicConstructors();
    void testCopyAndMoveSemantics();
    void testSmartPointerManagement();
    void testCourseManagement();
    void testUniversitySharedPointer();
    void testOperatorOverloading();
    void testContainerOperations();
};

#endif // StudentTest_hpp
