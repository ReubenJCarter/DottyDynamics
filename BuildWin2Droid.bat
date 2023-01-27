
echo "Building..." 

if not exist "build" mkdir "build"
if not exist "build\android" mkdir "build\android"


call ndk-build.cmd NDK_APPLICATION_MK=./jni/Application.mk NDK_APP_LIBS_OUT=build/android NDK_OUT=build/android
call ndk-build.cmd NDK_APPLICATION_MK=./jni/Application32.mk NDK_APP_LIBS_OUT=build/android NDK_OUT=build/android

copy "build\android\arm64-v8a\libDotty.so" "unity\Assets\Dotty\Plugins\Android\arm64-v8a\"
copy "build\android\arm64-v8a\libc++_shared.so" "unity\Assets\Dotty\Plugins\Android\arm64-v8a\"

copy "build\android\armeabi-v7a\libDotty.so" "unity\Assets\Dotty\Plugins\Android\armeabi-v7a\"
copy "build\android\armeabi-v7a\libc++_shared.so" "unity\Assets\Dotty\Plugins\Android\armeabi-v7a\"
