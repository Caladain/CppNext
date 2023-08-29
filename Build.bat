@echo off 
ECHO Starting CMAKE
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=vcpkg\scripts\buildsystems\vcpkg.cmake
ECHO CMAKE Done.  Run build/CppNext.sln