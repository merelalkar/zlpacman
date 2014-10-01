LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := pacman
LOCAL_SRC_FILES := pacman.cpp

include $(BUILD_SHARED_LIBRARY)
