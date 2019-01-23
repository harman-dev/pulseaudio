LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE       := default.pa
LOCAL_MODULE_TAGS  := optional eng
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES    := default.pa
LOCAL_MODULE_PATH  := $(TARGET_OUT_ETC)/pulse
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE       := default.withoutAVB.pa
LOCAL_MODULE_TAGS  := optional eng
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES    := default.withoutAVB.pa
LOCAL_MODULE_PATH  := $(TARGET_OUT_ETC)/pulse
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE       := daemon.conf
LOCAL_MODULE_TAGS  := optional eng
LOCAL_MODULE_CLASS := ETC
LOCAL_SRC_FILES    := daemon.conf
LOCAL_MODULE_PATH  := $(TARGET_OUT_ETC)/pulse
include $(BUILD_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE := init_pulseaudio.sh
LOCAL_MODULE_TAGS := optional eng
LOCAL_MODULE_CLASS := SYSTEM
LOCAL_MODULE_PATH := $(TARGET_OUT_EXECUTABLES)
LOCAL_SRC_FILES := $(LOCAL_MODULE)
include $(BUILD_PREBUILT)

# PA_BUILD_32BIT
# true : build to 32 bit  
# false: build to 64 bit 
PA_BUILD_32BIT := true

PA_DEFINES := \
  -DANDROID_PULSE_HOME=\"/cache/pulseaudio\" \
  -DPA_BINARY=\"/system/bin/pulseaudio\" \
  -DPA_DEFAULT_CONFIG_DIR=\"/system/etc/pulse\" \
  -DDUMP_HOME=\"/data/pulseaudio\" \
  -DHAVE_GETADDRINFO \
  -DHAVE_NANOSLEEP \
  -DHAVE_SIGACTION \
  -DHAVE_ARPA_INET_H \
  -DHAVE_ALSA \
  -DHAVE_CLOCK_GETTIME \
  -DHAVE_CTIME_R \
  -DHAVE_FCHMOD \
  -DHAVE_FCHOWN \
  -DHAVE_FSTAT \
  -DHAVE_GETPWNAM_R \
  -DHAVE_GETPWUID_R \
  -DHAVE_GETTIMEOFDAY \
  -DHAVE_GRP_H \
  -DHAVE_LSTAT \
  -DHAVE_MLOCK \
  -DHAVE_NETDB_H \
  -DHAVE_NETINET_IN_H \
  -DHAVE_NETINET_IN_SYSTM_H \
  -DHAVE_NETINET_IP_H \
  -DHAVE_NETINET_TCP_H \
  -DHAVE_PIPE \
  -DHAVE_PTHREAD \
  -DHAVE_PWD_H \
  -DHAVE_REGEX_H \
  -DHAVE_SCHED_H \
  -DHAVE_SETRESGID \
  -DHAVE_SETRESUID \
  -DHAVE_SETSID \
  -DHAVE_STD_BOOL \
  -DHAVE_SYMLINK \
  -DHAVE_SYS_EVENTFD_H \
  -DHAVE_SYS_IOCTL_H \
  -DHAVE_SYS_PRCTL_H \
  -DHAVE_SYS_SELECT_H \
  -DHAVE_SYS_SOCKET_H \
  -DHAVE_SYS_SYSCALL_H \
  -DHAVE_SYS_UN_H \
  -DHAVE_SYS_WAIT_H \
  -DHAVE_UNAME \
  -DHAVE_STRERROR_R \
  -DHAVE_POLL_H \
  -DHAVE_PPOLL \
  -DPAGE_SIZE=4096 \
  -DGETGROUPS_T=gid_t \
  -DPA_MACHINE_ID=\"\" \
  -DPA_MACHINE_ID_FALLBACK=\"\" \
  -DPA_SYSTEM_RUNTIME_PATH=\"\" \
  -DPA_BUILDDIR=\"\" \
  -DPA_SYSTEM_USER=\"system\" \
  -DPA_SYSTEM_GROUP=\"system\" \
  -DPA_SYSTEM_STATE_PATH=\"\" \
  -DPA_SYSTEM_CONFIG_PATH=\"/system/etc/pulse\" \
  -DDISABLE_ORC \
  -DPACKAGE \
  -DPACKAGE_NAME=\"pulseaudio\" \
  -DPACKAGE_VERSION=\"6.0\" \
  -DCANONICAL_HOST=\"\" \
  -DPA_CFLAGS=\"\" \
  -DPA_ALSA_PROFILE_SETS_DIR=\"\" \
  -DPA_ALSA_PATHS_DIR=\"\" \
  -DPA_ACCESS_GROUP=\"audio\" \
  -UNDEBUG \
  -DHAVE_ATOMIC_BUILTINS \
  -DPA_SOEXT=\".so\" \
  -DHAVE_ALSA_UCM \
  -DHAVE_SIMPLEDB \
  -DHAVE_SYS_MMAN_H \
  -D_POSIX_PRIORITY_SCHEDULING \
  -Wno-unused-parameter \
  -std=gnu99

ifeq ($(PA_BUILD_32BIT), true)
PA_DEFINES += \
    -DPA_DLSEARCHPATH=\"/system/lib/pulse\" \
    -DAUDIOPROCESSING_LIBPATH=\"/system/lib/libaudioprocessing.so\"
else
PA_DEFINES += \
    -DPA_DLSEARCHPATH=\"/system/lib64/pulse\" \
    -DAUDIOPROCESSING_LIBPATH=\"/system/lib64/libaudioprocessing.so\"
endif

PA_C_INCLUDE_PATH := \
    $(INCLUDE_PATH_OF_JSON-C) \
    $(INCLUDE_PATH_OF_SNDFILE) \
    $(INCLUDE_PATH_OF_ALSA)

PA_EXT_SHARED_LIBS := \
    libsndfile \
    libjson-c \
    liblog \
    libasound \
    libcutils \
    libutils

PA_SHARED_LIBS := \
    libpulsecommon \
    libpulse \
    libpulsecore

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
		pulse/client-conf.c \
		pulse/fork-detect.c \
		pulse/format.c \
		pulse/xmalloc.c \
		pulse/proplist.c \
		pulse/utf8.c \
		pulse/channelmap.c \
		pulse/sample.c \
		pulse/util.c \
		pulse/timeval.c \
		pulse/rtclock.c \
		pulse/volume.c  \
		pulsecore/authkey.c \
		pulsecore/conf-parser.c \
		pulsecore/core-error.c \
		pulsecore/core-format.c \
		pulsecore/core-rtclock.c \
		pulsecore/core-util.c \
		pulsecore/dynarray.c \
		pulsecore/fdsem.c \
		pulsecore/flist.c \
		pulsecore/g711.c \
		pulsecore/hashmap.c \
		pulsecore/i18n.c \
		pulsecore/idxset.c \
		pulsecore/arpa-inet.c \
		pulsecore/iochannel.c \
		pulsecore/ioline.c \
		pulsecore/ipacl.c \
		pulsecore/lock-autospawn.c \
		pulsecore/log.c \
		pulsecore/ratelimit.c \
		pulsecore/mcalign.c \
		pulsecore/memblock.c \
		pulsecore/memblockq.c \
		pulsecore/memchunk.c \
		pulsecore/once.c \
		pulsecore/packet.c \
		pulsecore/parseaddr.c \
		pulsecore/pdispatch.c \
		pulsecore/pid.c \
		pulsecore/pipe.c \
		pulsecore/memtrap.c \
		pulsecore/aupdate.c \
		pulsecore/proplist-util.c \
		pulsecore/pstream-util.c \
		pulsecore/pstream.c \
		pulsecore/queue.c \
		pulsecore/random.c \
		pulsecore/srbchannel.c \
		pulsecore/sample-util.c \
		pulsecore/shm.c \
		pulsecore/bitset.c \
		pulsecore/socket-client.c \
		pulsecore/socket-server.c \
		pulsecore/socket-util.c \
		pulsecore/strbuf.c \
		pulsecore/strlist.c \
		pulsecore/svolume_c.c \
		pulsecore/svolume_arm.c \
		pulsecore/svolume_mmx.c \
		pulsecore/svolume_sse.c \
		pulsecore/tagstruct.c \
		pulsecore/time-smoother.c \
		pulsecore/tokenizer.c \
		pulsecore/usergroup.c \
		pulsecore/sndfile-util.c \
		pulsecore/mutex-posix.c \
		pulsecore/thread-posix.c \
		pulsecore/semaphore-posix.c

LOCAL_CFLAGS := $(PA_DEFINES)
LOCAL_C_INCLUDES := $(PA_C_INCLUDE_PATH)
LOCAL_MODULE := libpulsecommon
LOCAL_SHARED_LIBRARIES:= $(PA_EXT_SHARED_LIBS)
LOCAL_MODULE_TAGS := optional eng
LOCAL_MULTILIB := both
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
		pulse/channelmap.c \
		pulse/context.c \
		pulse/direction.c \
		pulse/error.c\
		pulse/ext-device-manager.c \
		pulse/ext-device-restore.c \
		pulse/ext-stream-restore.c \
		pulse/ext-dsp-framework.c \
		pulse/format.c \
		pulse/introspect.c \
		pulse/mainloop-api.c \
		pulse/mainloop-signal.c \
		pulse/mainloop.c \
		pulse/operation.c \
		pulse/proplist.c \
		pulse/rtclock.c \
		pulse/sample.c \
		pulse/scache.c \
		pulse/stream.c \
		pulse/subscribe.c \
		pulse/thread-mainloop.c \
		pulse/timeval.c \
		pulse/utf8.c \
		pulse/util.c \
		pulse/volume.c \
		pulse/xmalloc.c \
		pulse/audiocontrol.c

LOCAL_CFLAGS := $(PA_DEFINES)
LOCAL_C_INCLUDES := $(PA_C_INCLUDE_PATH)
LOCAL_MODULE := libpulse
LOCAL_SHARED_LIBRARIES:= $(PA_EXT_SHARED_LIBS) libpulsecommon
LOCAL_MODULE_TAGS := optional eng
LOCAL_MULTILIB := both
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
		pulsecore/ffmpeg/resample2.c \
		pulsecore/asyncmsgq.c \
		pulsecore/asyncq.c \
		pulsecore/auth-cookie.c \
		pulsecore/cli-command.c \
		pulsecore/cli-text.c \
		pulsecore/client.c \
		pulsecore/card.c \
		pulsecore/core-scache.c \
		pulsecore/core-subscribe.c \
		pulsecore/core.c \
		pulsecore/hook-list.c \
		pulsecore/ltdl-helper.c \
		pulsecore/modargs.c \
		pulsecore/modinfo.c  \
		pulsecore/module.c \
		pulsecore/msgobject.c \
		pulsecore/namereg.c \
		pulsecore/object.c \
		pulsecore/play-memblockq.c \
		pulsecore/play-memchunk.c \
		pulsecore/remap.c \
		pulsecore/remap_mmx.c \
		pulsecore/remap_sse.c \
		pulsecore/resampler.c \
		pulsecore/resampler/ffmpeg.c \
		pulsecore/resampler/peaks.c \
		pulsecore/resampler/trivial.c \
		pulsecore/rtpoll.c \
		pulsecore/stream-util.c \
		pulsecore/mix.c \
		pulsecore/cpu.c \
		pulsecore/cpu-arm.c \
		pulsecore/cpu-x86.c \
		pulsecore/cpu-orc.c \
		pulsecore/sconv-s16be.c \
		pulsecore/sconv-s16le.c \
		pulsecore/sconv_sse.c \
		pulsecore/sconv.c \
		pulsecore/shared.c \
		pulsecore/sink-input.c \
		pulsecore/sink.c \
		pulsecore/device-port.c \
		pulsecore/sioman.c \
		pulsecore/sound-file-stream.c \
		pulsecore/sound-file.c \
		pulsecore/source-output.c \
		pulsecore/source.c \
		pulsecore/start-child.c \
		pulsecore/thread-mq.c \
		pulsecore/database-simple.c \
		pulsecore/protocol-native.c \
		pulsecore/cli.c \
		pulsecore/protocol-cli.c

LOCAL_CFLAGS := $(PA_DEFINES) -D__INCLUDED_FROM_PULSE_AUDIO
LOCAL_C_INCLUDES := $(PA_C_INCLUDE_PATH)
LOCAL_MODULE := libpulsecore
LOCAL_SHARED_LIBRARIES:= $(PA_EXT_SHARED_LIBS) libpulsecommon libpulse
LOCAL_MODULE_TAGS := optional eng
LOCAL_MULTILIB := both
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
    modules/alsa/alsa-ucm.c \
    modules/alsa/alsa-util.c \
    modules/alsa/alsa-mixer.c \
    modules/alsa/alsa-sink.c \
    modules/alsa/alsa-source.c \
    modules/reserve-wrap.c

LOCAL_C_INCLUDES := $(PA_C_INCLUDE_PATH)
LOCAL_CFLAGS :=  $(PA_DEFINES) -D__INCLUDED_FROM_PULSE_AUDIO -DPA_SRCDIR=""
LOCAL_MODULE := libalsa-util
LOCAL_SHARED_LIBRARIES:= $(PA_EXT_SHARED_LIBS) $(PA_SHARED_LIBS)
LOCAL_MODULE_TAGS := optional eng
LOCAL_MULTILIB := both
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
    daemon/caps.c \
    daemon/cmdline.c \
    daemon/cpulimit.c \
    daemon/daemon-conf.c \
    daemon/dumpmodules.c \
    daemon/ltdl-bind-now.c \
    daemon/main.c

LOCAL_C_INCLUDES := $(PA_C_INCLUDE_PATH)
LOCAL_CFLAGS := $(PA_DEFINES)
LOCAL_MODULE := pulseaudio
LOCAL_SHARED_LIBRARIES:= $(PA_EXT_SHARED_LIBS) $(PA_SHARED_LIBS)
LOCAL_MODULE_TAGS := optional eng
LOCAL_32_BIT_ONLY := $(PA_BUILD_32BIT)
include $(BUILD_EXECUTABLE)

define PA_MODULE_TEMPLATE
  include $$(CLEAR_VARS)
  LOCAL_MODULE := $(1)
  $$(LOCAL_PATH)/$(2)/$$(LOCAL_MODULE).c: $$(LOCAL_PATH)/$(2)/$$(LOCAL_MODULE)-symdef.h
  $$(LOCAL_PATH)/$(2)/$$(LOCAL_MODULE)-symdef.h: $$(LOCAL_PATH)/modules/module-defs.h.m4
	m4 -Dfname="$$@" $$< > $$@
  LOCAL_C_INCLUDES := $(PA_C_INCLUDE_PATH)
  LOCAL_SRC_FILES := $(2)/$$(LOCAL_MODULE).c
  LOCAL_CFLAGS := $$(PA_DEFINES) -D__INCLUDED_FROM_PULSE_AUDIO
  LOCAL_SHARED_LIBRARIES := $(PA_SHARED_LIBS) $(3)
  LOCAL_MODULE_TAGS := optional eng
  LOCAL_MODULE_RELATIVE_PATH := pulse
  LOCAL_MULTILIB := both
  include $$(BUILD_SHARED_LIBRARY)
endef

PA_MODULES := \
  module-loopback \
  module-combine-sink \
  module-virtual-sink \
  module-suspend-on-idle

PA_ALSA_MODULES := \
  module-alsa-sink \
  module-alsa-source \
  module-alsa-card

$(foreach pamod,$(PA_MODULES),$(eval $(call PA_MODULE_TEMPLATE,$(pamod),modules,)))
$(foreach pamod,$(PA_ALSA_MODULES),$(eval $(call PA_MODULE_TEMPLATE,$(pamod),modules/alsa,libalsa-util libasound)))

include $(CLEAR_VARS)
LOCAL_MODULE := module-native-protocol-unix
$(LOCAL_PATH)/modules/module-protocol-stub.c: $(LOCAL_PATH)/modules/$(LOCAL_MODULE)-symdef.h
$(LOCAL_PATH)/modules/$(LOCAL_MODULE)-symdef.h: $(LOCAL_PATH)/modules/module-defs.h.m4
	m4 -Dfname="$@" $< > $@
LOCAL_SRC_FILES := modules/module-protocol-stub.c
LOCAL_CFLAGS := $(PA_DEFINES) -D__INCLUDED_FROM_PULSE_AUDIO -DUSE_PROTOCOL_NATIVE
LOCAL_SHARED_LIBRARIES := $(PA_SHARED_LIBS)
LOCAL_MODULE_TAGS := optional eng
LOCAL_MULTILIB := both
LOCAL_MODULE_RELATIVE_PATH := pulse
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := module-cli-protocol-unix
$(LOCAL_PATH)/modules/module-protocol-stub.c: $(LOCAL_PATH)/modules/$(LOCAL_MODULE)-symdef.h
$(LOCAL_PATH)/modules/$(LOCAL_MODULE)-symdef.h: $(LOCAL_PATH)/modules/module-defs.h.m4
	m4 -Dfname="$@" $< > $@
LOCAL_SRC_FILES := modules/module-protocol-stub.c
LOCAL_CFLAGS := $(PA_DEFINES) -D__INCLUDED_FROM_PULSE_AUDIO  -DUSE_PROTOCOL_CLI
LOCAL_SHARED_LIBRARIES := $(PA_SHARED_LIBS)
LOCAL_MODULE_TAGS := optional eng
LOCAL_MULTILIB := both
LOCAL_MODULE_RELATIVE_PATH := pulse
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES:= $(PA_C_INCLUDE_PATH)
LOCAL_SRC_FILES:= utils/pacmd.c
LOCAL_CFLAGS := $(PA_DEFINES)
LOCAL_MODULE := pacmd
LOCAL_SHARED_LIBRARIES := $(PA_EXT_SHARED_LIBS) $(PA_SHARED_LIBS)
LOCAL_MODULE_TAGS := optional eng
LOCAL_32_BIT_ONLY := $(PA_BUILD_32BIT)
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(PA_C_INCLUDE_PATH)
LOCAL_SRC_FILES:= utils/pactl.c
LOCAL_CFLAGS := $(PA_DEFINES)
LOCAL_MODULE := pactl
LOCAL_SHARED_LIBRARIES := $(PA_EXT_SHARED_LIBS) $(PA_SHARED_LIBS)
LOCAL_MODULE_TAGS := optional eng
LOCAL_32_BIT_ONLY := $(PA_BUILD_32BIT)
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES:= $(PA_C_INCLUDE_PATH)
LOCAL_SRC_FILES:= utils/pacat.c
LOCAL_CFLAGS := $(PA_DEFINES)
LOCAL_MODULE := pacat
LOCAL_SHARED_LIBRARIES:= $(PA_EXT_SHARED_LIBS) $(PA_SHARED_LIBS)
LOCAL_MODULE_TAGS := optional eng
LOCAL_32_BIT_ONLY := $(PA_BUILD_32BIT)
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES:= $(PA_C_INCLUDE_PATH)
LOCAL_SRC_FILES:= utils/dummy_audiomanagement.c
LOCAL_CFLAGS := $(PA_DEFINES)
LOCAL_MODULE := dummy_audiomanagement
LOCAL_SHARED_LIBRARIES:= $(PA_EXT_SHARED_LIBS) $(PA_SHARED_LIBS)
LOCAL_MODULE_TAGS := optional eng
LOCAL_32_BIT_ONLY := $(PA_BUILD_32BIT)
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES:= $(PA_C_INCLUDE_PATH)
LOCAL_SRC_FILES:= utils/padump2file.c
LOCAL_CFLAGS := $(PA_DEFINES)
LOCAL_MODULE := padump2file
LOCAL_SHARED_LIBRARIES:= $(PA_EXT_SHARED_LIBS) $(PA_SHARED_LIBS)
LOCAL_MODULE_TAGS := optional eng
LOCAL_32_BIT_ONLY := $(PA_BUILD_32BIT)
include $(BUILD_EXECUTABLE)





