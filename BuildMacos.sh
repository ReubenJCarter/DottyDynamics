mkdir -p ./build/macos
g++ -shared -fPIC -D UNIXLIKE -Ofast -std=c++17 -o ./build/macos/libDotty.so ./src/DottyBind.cpp
cp ./build/macos/libDotty.so ./unity/Assets/Dotty/Plugins/MacOS