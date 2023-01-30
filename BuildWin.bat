if not defined DevEnvDir (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
)

echo "Building..." 

if not exist "build" mkdir "build"
if not exist "build\win" mkdir "build\win"
if not exist "build\win\obj" mkdir "build\win\obj"

cl /std:c++17 /LD /O2 /Febuild\win\Dotty.dll /Fobuild\win\obj\Dotty.o src/DottyBind.cpp 
cl /std:c++17 /O2 /Febuild\win\WorldTest.exe /Fobuild\win\obj\WorldTest.o test/WorldTest.cpp 

copy "build\win\Dotty.dll" "unity\Assets\Dotty\Plugins\Windows\"

