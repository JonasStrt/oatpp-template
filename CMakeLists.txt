cmake_minimum_required(VERSION 3.20)

set(project_name my-project) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 17)

add_library(${project_name}-lib
        src/AppComponent.hpp
        src/controller/MyController.cpp
        src/controller/MyController.hpp
        src/controller/MyAuthController.cpp
        src/controller/MyAuthController.hpp
        src/auth/AuthConfig.hpp
        src/auth/AuthInterceptor.hpp
        src/auth/JwksCache.hpp
        src/auth/JwtVerifier.hpp
        src/dto/DTOs.hpp
        src/model/Student.cpp
        src/model/Student.hpp
        src/model/TestCode.cpp
        src/model/TestCode.hpp
)

## link libs

find_package(oatpp 1.4.0 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(jwt-cpp REQUIRED)

target_link_libraries(${project_name}-lib
        PUBLIC oatpp::oatpp
        PUBLIC oatpp::oatpp-test
        PUBLIC OpenSSL::SSL
        PUBLIC OpenSSL::Crypto
        PUBLIC CURL::libcurl
        PUBLIC nlohmann_json::nlohmann_json
        PUBLIC jwt-cpp::jwt-cpp
)

target_include_directories(${project_name}-lib PUBLIC src)

## add executables

add_executable(${project_name}-exe
        src/App.cpp
        test/app/MyApiTestClient.hpp)
target_link_libraries(${project_name}-exe ${project_name}-lib)
add_dependencies(${project_name}-exe ${project_name}-lib)

add_executable(${project_name}-test
        test/tests.cpp
        test/app/TestComponent.hpp
        test/app/MyApiTestClient.hpp
        test/MyControllerTest.cpp
        test/MyControllerTest.hpp
        test/StudentTest.cpp
        test/StudentTest.hpp
        test/TestCodeTest.cpp
        test/TestCodeTest.hpp
)

target_link_libraries(${project_name}-test ${project_name}-lib)
target_include_directories(${project_name}-test PRIVATE test)
add_dependencies(${project_name}-test ${project_name}-lib)

set_target_properties(${project_name}-lib ${project_name}-exe ${project_name}-test PROPERTIES
        CXX_STANDARD 17
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

enable_testing()
add_test(project-tests ${project_name}-test)
