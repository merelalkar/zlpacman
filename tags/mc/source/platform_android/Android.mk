#----------------------------------------------
#platform android file
#---------------------------------------------

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LS_CPP=$(subst $(1)/,,$(wildcard $(1)/*.cpp))

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../3dparty/rapidxml
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../3dparty/libttf

LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES
LOCAL_CFLAGS += -DRAPIDXML_NO_EXCEPTIONS

LOCAL_MODULE := platform_android
LOCAL_SRC_FILES := $(call LS_CPP,$(LOCAL_PATH))
LOCAL_STATIC_LIBRARIES := engine android_native_app_glue png ttf
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)
LOCAL_EXPORT_LDLIBS := -landroid -llog -lEGL -lGLESv1_CM -lOpenSLES

include $(BUILD_STATIC_LIBRARY)

