LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CFLAGS += -D GL_GLEXT_PROTOTYPES
LOCAL_MODULE    := droidblaster
LOCAL_SRC_FILES := Main.cpp EventLoop.cpp Log.cpp TimeService.cpp GraphicsService.cpp GraphicsTexture.cpp GraphicsSprite.cpp Resource.cpp DroidBlaster.cpp Ship.cpp
#LOCAL_SRC_FILES := $(call LS_CPP,$(LOCAL_PATH))
LOCAL_LDLIBS := -landroid -llog -lEGL -lGLESv1_CM
LOCAL_STATIC_LIBRARIES := android_native_app_glue png

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,libpng)