#-----------------------------------------------
#game file
#-----------------------------------------------

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LS_CPP=$(subst $(1)/,,$(wildcard $(1)/*.cpp))

LOCAL_MODULE := game

LOCAL_SRC_FILES += game_ai.cpp
LOCAL_SRC_FILES += game_events.cpp
LOCAL_SRC_FILES += game_objects.cpp
LOCAL_SRC_FILES += game_screen.cpp
LOCAL_SRC_FILES += game_world.cpp
LOCAL_SRC_FILES += pacman_game_screens.cpp
LOCAL_SRC_FILES += tile_grid.cpp

LOCAL_STATIC_LIBRARIES := engine platform_android

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path,$(LOCAL_PATH)/..)
$(call import-module,android/native_app_glue)
$(call import-module,libpng)
$(call import-module,engine)
$(call import-module,platform_android)