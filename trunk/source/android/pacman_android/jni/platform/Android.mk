LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := pacman_platform
LOCAL_SRC_FILES := gles_graph_manager.cpp
LOCAL_LDLIBS    := -lGLESv2
 
include $(BUILD_STATIC_LIBRARY)
