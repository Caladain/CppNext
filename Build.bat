@echo off 
ECHO Starting CMAKE
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build build --config Release --target cppn_compiler
cmake --build build --config Debug --target cppn_compiler
cmake --build build --config RelWithDebInfo --target cppn_compiler
ECHO .
ECHO Running Release Tests
cmake --build build --config Release --target cppn_compiler_tests
ctest --test-dir build
ECHO .
ECHO Running Debug Tests
cmake --build build --config Debug --target cppn_compiler_tests
ctest --test-dir build
ECHO .
ECHO Running RelWithDebInfo Tests
cmake --build build --config RelWithDebInfo --target cppn_compiler_tests
ctest --test-dir build
ECHO .
ECHO CMAKE Done.  Feel free to run build/CppNext.sln