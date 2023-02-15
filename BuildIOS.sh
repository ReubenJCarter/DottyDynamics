mkdir -p ./build/ios

`xcrun --sdk iphoneos --find clang` -Os  -isysroot `xcrun --sdk iphoneos \
  --show-sdk-path` -F`xcrun --sdk iphoneos --show-sdk-path`/System/Library/Frameworks \
  -arch arm64 -static -D UNIXLIKE -Ofast -std=c++17 -v -o ./build/ios/libDotty.a ./src/DottyBind.cpp

cp ./build/ios/libDotty.a ./unity/Assets/Dotty/Plugins/IOS