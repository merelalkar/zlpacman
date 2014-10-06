#-------------------------------------------------------
#engine file
#-------------------------------------------------------

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LS_CPP=$(subst $(1)/,,$(wildcard $(1)/*.cpp))

LOCAL_MODULE := engine
LOCAL_SRC_FILES := $(call LS_CPP,$(LOCAL_PATH))
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(BUILD_STATIC_LIBRARY)
