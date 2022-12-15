if not defined DevEnvDir (
    call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x64
)

echo "Building..." 

if not exist "build" mkdir "build"
if not exist "build\obj" mkdir "build\obj"

cl /std:c++17 /LD /O2 /Febuild\Dotty.dll /Fobuild\obj\Dotty.o src/DottyBind.cpp 