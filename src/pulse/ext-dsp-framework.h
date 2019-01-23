#ifndef foopulseextdspframeworkhfoo
#define foopulseextdspframeworkhfoo

/***
  This file is part of PulseAudio.

  Copyright 2015 Harman COC_Media

  PulseAudio is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation; either version 2.1 of the License,
  or (at your option) any later version.

  PulseAudio is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with PulseAudio; if not, see <http://www.gnu.org/licenses/>.
***/

#include <pulse/cdecl.h>
#include <pulse/context.h>
#include <pulse/version.h>
#include <pulse/format.h>

/** \file
 *
 * Routines for controlling module-stream-restore
 */

PA_C_DECL_BEGIN


#define AM_PARAMS_DATA_LENGTH 256
#define PA_PROP_AUDIO_MANAGER "audiomanager.flag"
#define PA_PROP_DSP_EVENT "DSP.Tagged_MSG"
#define DSP_EVENT "dsp_event"

typedef enum
{
    CMD_SET,
    CMD_GET,
    CMD_SETGET,
    CMD_RESULT = 0x0C,
    CMD_ERROR  = 0x0F,
}CMD_TYPE_t;

typedef enum
{
    MSG_AM_CMD,
    MSG_DUMP_CMD,
    MSG_INVALID
}msg_type_t;


/**
 * @brief ASP module location
 *
 * Specifies the location of the ASP module.
 * This information is required for the Audio Realtime Controller.
 * The audio controller supports multiple remote instances, this is
 * indicated by the hopping message type. A HOPPING_1_MSG_E message
 * indicates a message for a remote system which is 1 hop away from
 * its destination.
 */
typedef enum
{
  LOCAL_AC_INSTANCE_E,
  HOPPING_1_MSG_E,
  HOPPING_2_MSG_E,
  HOPPING_3_MSG_E
}t_Asp_Module_Location;


/** @ingroup PrivateTypeDefs
 * The enum below is used to identify the correct destination so that the message can be put into the correct queue or
 * that the correct control instance of the AudioController is addressed.
 */
typedef enum
{
   /** message is intended for the AudioManagement transmit queue*/
   AUDIO_MANAGEMENT_E,
   /** message is intended for the Diagnostic transmit queue */
    DIAGNOSTIC_E,
    /** message is intended for the SyncRouting transmit queue */
    SYNC_ROUTING_E,
    /** message is intended for the Decoder transmit queue */
    DECODER_E,
    /** message is intended for the AudioController Transparent Mode state */
    ASP_MODULE_E,
    /** message is intended for the AudioCOntroller Crossbar Control module */
    CROSSBAR_E,
    /** message is intended for the AudioConroller Decoder Control module */
   DECODER_CTRL_E,
     /**  20130408 message is intended for the AudioConroller SRC Control module */
   SRC_CTRL_E
}t_Message_Destinations;


/**
 * @brief Available ASP Modules
 *
 * List of all available ASP modules. These defines
 * have to be used the specify the ASP module within
 * the t_Module structure.
 */
typedef enum
{
   ASP_AEQUALIZER_E,
   ASP_ANTI_ALIAS_FILTER_E,
   ASP_CHIME_GEN_E,
   ASP_DELAY_E,
   ASP_LIMITER_E,
   ASP_MIXER_E,
   ASP_MUTE_CHANNEL_E,
   ASP_POWER_CALCULATION_E,
   ASP_RINGTONE_GEN_E,
   ASP_SIGNAL_GEN_E,
   ASP_SK_COMPRESOR_E,
   ASP_TONE_CONTROL_E,
   ASP_TONE_CONTROL_WDF_E,
   ASP_TRUE_LOUDNESS_E,
   ASP_DTCP_CIPHER_E,
   ASP_DECODER_E,
   ASP_REFCHANMIXER_E,
   ASP_GAIN_E,
   ASP_SUMMING_E,
   ASP_TLAM_SPEECH_E,
   ASP_ICC_WRAPPER_E,
   ASP_MEGICOL_E,
   ASP_SPHEARICOL_E,
   ASP_LOGIC7_E,
   ASP_TLAM_VOLUME_E,
   ASP_TELEPHONEMIXER_E,
   ASP_SIGNALANALYSIS_E,
   ASP_GALAF_E,
   ASP_HD_BLENDFILTER_E,
   ASP_DYNAMIC_COMPRESSOR_E,
   ASP_ADAPTIVEMAPPING_E,
   ASP_USER_EQUALIZER_E,
   ASP_CLIPPING_DETECTION_E,
   ASP_CLIPPING_COUNTER_E,
   ASP_AEQUALIZER2_E,
   ASP_ASPSLAVE_E,
   ASP_ANTIALIASFILTER_E,
   ASP_SEAMLESSLINKING_E,
   ASP_SINKDELAY_E,
   ASP_CMT_E,
   ASP_USER_EQUALIZER2_E,
   ASP_NUM_MODULES_E,
   ASP_INVALID_E
}t_Asp_Module_Types;

/** @ingroup PublicTypedDefs
 * structure used as message structure for data exchange between the AudioController and
 * the AudioManagement
 */
typedef struct
{
  /** not important for AC, used by AM */
  uint16_t Session_ID;
  /** identifies the command */
  uint16_t Cmd_ID;
  /** the type of command ( set, get, setget...)*/
  uint16_t Cmd_Type;
  /** in case of AM command it defines the audio zone, in case of decoder command it
   *  defines the decoder instance */
  uint16_t InstID;
  /** the number of used data fields in the data array */
  uint16_t Length;
  /** the array carrying additional command data */
  uint8_t Data[AM_PARAMS_DATA_LENGTH];
}t_AM_Params;



/** @ingroup PrivateTypeDefs
 * Description:
 * The structure below is used to tag messages coming from the higher layers with additional internal information
 * for routing purposes. A tagged message is only routed between AudioController objects.
 */
typedef struct
{
   /** The location of the destination. Defines how many "hops" the message must be routed */
   t_Asp_Module_Location  e_AC_Destination;
   /** The destination of the message */
   t_Message_Destinations e_Msg_Destination;
   /** The module type of the message */
   t_Asp_Module_Types     e_Asp_Module;
   /* The message structure coming from the AudioManagement. It contains the protocol information*/
   t_AM_Params            x_AM_Params;
}t_Tagged_MSG_Params;


typedef int32_t PAF_AudioData;

/// PAF_AudioFrameData is a fixed structure which defines the possible
/// data-carrying capacity of the audio frame.
typedef struct sPAF_AudioFrameData
{
   int16_t           bfpExp;     ///< block floating point exponent
   uint32_t          nChannels;  ///< max. number of channels
   PAF_AudioData    **sample;     ///< sample[nChannels][sampleCount]
} PAF_AudioFrameData;

/// Forward declaration of buffer struct from BufferManager to avoid dependency
struct BMGR_Buffer;

typedef struct MetaDataBuffer
{
   uint32_t nChannels;                ///< max. number of channels
   struct BMGR_Buffer **sample;     ///< Pointers to buffer structs of BufferManager. This does also contain the possibility to directly access the meta data of the buffers.
} tMetaDataBuffer;


/// PAF_AudioFrame is the audio frame as a whole.
typedef struct sPAF_AudioFrame
{
   uint32_t     sampleCount;  ///< actual number of samples
   PAF_AudioFrameData  data;         ///< audio frame data
   tMetaDataBuffer     meta;         ///< meta data buffer
} PAF_AudioFrame;

typedef void (*event_cb_t)(t_Tagged_MSG_Params *msg);
typedef void (*cmd_status_cb_t)(uint8_t result);
typedef void (*dump_status_cb_t)(uint8_t result);

pa_operation* pa_context_ext_audio_framework_msg_send(
	pa_context *c,
	msg_type_t msg_type,
	void *data,
	cmd_status_cb_t cb,
	void *userdata,
	const char *name);

typedef struct
{
   /** dump time in seconds */
   unsigned int time;
   /** dump file path */
   char path[256];
}t_dump_params;


typedef enum
{
    PA_DUMP_OK = 0,
    PA_DUMP_ERR_ACCESS,
    PA_DUMP_ERR_BUSY,
    PA_DUMP_ERR_UNKNOWN
}pa_dump_error_code_t;

PA_C_DECL_END

#endif
