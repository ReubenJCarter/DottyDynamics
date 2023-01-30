mkdir -p ./build/macos
g++ -shared -fPIC -D UNIXLIKE -Ofast -march=native -o ./build/macos/libDotty.so ./src/DottyBind.cpp
cp ./build/macos/libDotty.so ./unity/Assets/Dotty/Plugins/MacOS