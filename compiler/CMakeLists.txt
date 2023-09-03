file (GLOB_RECURSE SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS
    src/*.h
    src/*.cpp 
)

add_executable(cppn_compiler ${SRC_FILES})

target_include_directories(cppn_compiler PUBLIC src)

target_link_libraries(cppn_compiler
PRIVATE
fmt::fmt
Tracy::TracyClient
Threads::Threads #needed by tracy
cxxopts::cxxopts
magic_enum::magic_enum
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC_FILES})
