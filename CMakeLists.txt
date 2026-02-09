cmake_minimum_required(VERSION 3.14)

project(Checksum VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ==========================================
# 1. THE CORE LIBRARY
# ==========================================
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# Exclude main.cpp from the library
foreach(f ${SRC_FILES})
    if(f MATCHES "main.cpp$")
        list(REMOVE_ITEM SRC_FILES ${f})
    endif()
endforeach()

find_package(OpenSSL REQUIRED)

add_library(core_lib STATIC ${SRC_FILES})
target_include_directories(core_lib PUBLIC headers)

target_link_libraries(core_lib PUBLIC OpenSSL::SSL OpenSSL::Crypto)

# ==========================================
# 2. THE MAIN APP
# ==========================================
if(EXISTS "${CMAKE_SOURCE_DIR}/src/main.cpp")
    add_executable(my_app src/main.cpp)
    target_link_libraries(my_app PRIVATE core_lib)
endif()

# ==========================================
# 3. THE TESTS (Catch2 v3 Setup)
# ==========================================
enable_testing()


add_subdirectory(lib/Catch2)

file(GLOB_RECURSE TEST_FILES "test/*.cpp")

add_executable(run_tests ${TEST_FILES})

# Link your core library AND Catch2::Catch2WithMain
# Catch2WithMain provides the main() function automatically.
target_link_libraries(run_tests PRIVATE core_lib Catch2::Catch2WithMain)

# This defines a macro "TEST_DATA_DIR" that holds the absolute path to your folder
target_compile_definitions(run_tests PRIVATE 
    TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/test/testResources/"
)

# Use the target file location for CTest
add_test(NAME AllTests COMMAND $<TARGET_FILE:run_tests>)