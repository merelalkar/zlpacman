LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := pacman_engine
LOCAL_SRC_FILES := animation.cpp collision_checker.cpp common_events.cpp default_game_state.cpp event_system.cpp fader_layer.cpp game_objects.cpp graf_manager.cpp gui_layer.cpp gui_manager.cpp interfaces.cpp math_utils.cpp processes.cpp standart_widgets.cpp types.cpp

include $(BUILD_STATIC_LIBRARY)