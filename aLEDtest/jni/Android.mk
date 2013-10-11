LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS) 
# give module name
LOCAL_MODULE    := blinky  
# list your C files to compile
LOCAL_SRC_FILES := cloverLEDBlinker.cpp
# this option will build executables instead of building library for android application.
include $(BUILD_EXECUTABLE)
