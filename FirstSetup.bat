@echo off 
ECHO Setting up VCPKG
rmdir /s /q vcpkg
call Clean.bat
git clone https://github.com/microsoft/vcpkg
cd vcpkg
call bootstrap-vcpkg.bat
cd ..
call Build.bat