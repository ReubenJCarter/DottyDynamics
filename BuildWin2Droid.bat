
echo "Building..." 

if not exist "build" mkdir "build"
if not exist "build\obj" mkdir "build\obj"

call ndk-build.cmd NDK_APPLICATION_MK=./jni/Application.mk NDK_APP_LIBS_OUT=build/ NDK_OUT=build/

copy "build\arm64-v8a\libDotty.so" "unity\Assets\Dotty\Plugins\Android\"

