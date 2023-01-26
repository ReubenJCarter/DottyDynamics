LOCAL_PATH:= $(call my-dir)  # Get the local path of the project.
include $(CLEAR_VARS) # Clear all the variables with a prefix "LOCAL_"
LOCAL_CFLAGS:= -std=c++17 -fms-extensions -fexceptions -Wignored-attributes -ferror-limit=1000
LOCAL_MODULE:= Dotty #the name of the binary
LOCAL_SRC_FILES:= ../src/DottyBind.cpp
include $(BUILD_SHARED_LIBRARY)# builds a shared lib