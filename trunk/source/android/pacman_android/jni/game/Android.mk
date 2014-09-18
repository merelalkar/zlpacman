LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := pacman_game
LOCAL_SRC_FILES := editor.cpp game_ai.cpp game_app.cpp game_events.cpp game_objects.cpp game_screen.cpp game_world.cpp pacman_game_screens.cpp tile_grid.cpp
LOCAL_STATIC_LIBRARIES := pacman_engine pacman_platform
 
include $(BUILD_SHARED_LIBRARY)