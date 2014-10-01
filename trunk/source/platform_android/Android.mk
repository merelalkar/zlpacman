LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LS_CPP=$(subst $(1)/,,$(wildcard $(1)/*.cpp))

LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES
LOCAL_CFLAGS += -DRAPIDXML_NO_EXCEPTIONS

# !!!! ENGINE MODULE !!!!
LOCAL_MODULE := platform_android
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../3dparty/rapidxml
LOCAL_SRC_FILES := $(call LS_CPP,$(LOCAL_PATH))
LOCAL_STATIC_LIBRARIES := android_native_app_glue png

include $(BUILD_STATIC_LIBRARY)

