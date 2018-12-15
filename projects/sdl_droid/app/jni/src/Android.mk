LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL2
SDL_MIXER_PATH := ../SDL2_mixer

LOCAL_CFLAGS += -DANDROID

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/$(SDL_MIXER_PATH)/src \
    $(LOCAL_PATH)/$(SDL_PATH)/include \
	$(LOCAL_PATH)/framework/core \
	$(LOCAL_PATH)/framework/tools \
	$(LOCAL_PATH)/sources

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

# Add your application source files here...
#LOCAL_SRC_FILES := $(LOCAL_PATH)/sources/main.cpp

LOCAL_SRC_FILES := \
    $(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/framework/core/*.c) \
	$(wildcard $(LOCAL_PATH)/framework/tools/*.c) \
	$(wildcard $(LOCAL_PATH)/sources/*.c \
    $(LOCAL_PATH)/bridge.c \
    $(LOCAL_PATH)/sources/main.cpp ))

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_mixer

LOCAL_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -llog -landroid

include $(BUILD_SHARED_LIBRARY)
