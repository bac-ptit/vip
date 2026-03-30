cmake_minimum_required(VERSION 3.10)
project(vip_test CXX)

find_package(Drogon CONFIG REQUIRED)
find_package(glaze CONFIG REQUIRED)

# Basic test
add_executable(vip_test test_main.cc)
target_link_libraries(vip_test PRIVATE Drogon::Drogon)
ParseAndAddDrogonTests(vip_test)

# Auth test (Client only)
add_executable(test_auth test_auth.cc)
target_link_libraries(test_auth PRIVATE 
    Drogon::Drogon
    glaze::glaze
)
ParseAndAddDrogonTests(test_auth)
