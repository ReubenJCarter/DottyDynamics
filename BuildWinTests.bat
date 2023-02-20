if not defined DevEnvDir (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
)

echo "Building..." 

if not exist "build" mkdir "build"
if not exist "build\win" mkdir "build\win"
if not exist "build\win\obj" mkdir "build\win\obj"

cl /std:c++17 /O2 /Febuild\win\WorldTest.exe /Fobuild\win\obj\WorldTest.o test/WorldTest.cpp 
cl /std:c++17 /O2 /Febuild\win\RodsTest.exe /Fobuild\win\obj\WorldTest.o test/RodsTest.cpp
