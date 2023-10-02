@echo off 
ECHO Starting CMAKE
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build build --config Release --target cppn_compiler
cmake --build build --config Debug --target cppn_compiler
cmake --build build --config RelWithDebInfo --target cppn_compiler
ECHO CMAKE Done.  Run build/CppNext.sln