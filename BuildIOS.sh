mkdir -p ./build/ios

`xcrun --sdk iphoneos --find clang` -Os  -isysroot `xcrun --sdk iphoneos \
  --show-sdk-path` -F`xcrun --sdk iphoneos --show-sdk-path`/System/Library/Frameworks \
  -arch armv7 -arch armv7s -arch arm64 -static -D UNIXLIKE -o ./build/ios/libDotty.a ./src/DottyBind.cpp

cp ./build/ios/libDotty.a ./unity/Assets/Dotty/Plugins/IOS