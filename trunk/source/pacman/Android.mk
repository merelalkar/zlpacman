#pacman game file

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LS_CPP=$(subst $(1)/,,$(wildcard $(1)/*.cpp))

LOCAL_MODULE := pacman
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../engine
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../platform_android
LOCAL_SRC_FILES := $(call LS_CPP,$(LOCAL_PATH))
LOCAL_STATIC_LIBRARIES := engine platform_android 
LOCAL_LDLIBS := -landroid -llog -lEGL -lGLESv1_CM -lOpenSLES


include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,libpng)
