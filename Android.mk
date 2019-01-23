
LOCAL_PATH:= $(call my-dir)

INCLUDE_PATH_OF_JSON-C := $(LOCAL_PATH)/../json-c
INCLUDE_PATH_OF_SNDFILE := $(LOCAL_PATH)/../libsndfile/src
INCLUDE_PATH_OF_ALSA := vendor/intel/external/alsa-lib/android/include/alsa \
                        vendor/intel/external/alsa-lib/include

include $(call all-subdir-makefiles)
