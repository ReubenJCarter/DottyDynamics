
echo "Building..." 

if not exist "build" mkdir "build"
if not exist "build\obj" mkdir "build\obj"

call ndk-build.cmd NDK_APP_LIBS_OUT=build/ NDK_OUT=build/
copy "build\Dotty.so" "unity\Assets\Dotty\"

