mkdir -p ./build/macos
g++ -dynamiclib -fPIC -D UNIXLIKE -Ofast -std=c++17 -o ./build/macos/libDotty.dylib ./src/DottyBind.cpp

mkdir -p ./unity/Assets/Dotty/Plugins/MacOS
cp ./build/macos/libDotty.dylib ./unity/Assets/Dotty/Plugins/MacOS