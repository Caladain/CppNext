file (GLOB_RECURSE SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
    src/*.h
    src/*.cpp
)

add_library(cppn_compiler_lib STATIC ${SRC_FILES})

target_include_directories(cppn_compiler_lib PUBLIC src)

target_link_libraries(cppn_compiler_lib
PUBLIC
fmt::fmt
Tracy::TracyClient
Threads::Threads #needed by tracy
cxxopts::cxxopts
magic_enum::magic_enum
ctre::ctre
)

add_executable(cppn_compiler main.cpp)
target_link_libraries(cppn_compiler PUBLIC cppn_compiler_lib)


file (GLOB_RECURSE TEST_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
    tests/*.h
    tests/*.cpp
)

add_executable(cppn_compiler_tests ${TEST_FILES})
target_link_libraries(cppn_compiler_tests 
PUBLIC
cppn_compiler_lib
GTest::gtest_main
)

gtest_discover_tests(cppn_compiler_tests)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC_FILES})
