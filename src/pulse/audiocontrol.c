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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <pulse/pulseaudio.h>
#include <pulse/mainloop.h>
#include <pulse/internal.h>
#include <pulse/context.h>
#include <pulse/xmalloc.h>
#include <pulse/operation.h>
#include <pulse/ext-dsp-framework.h>
#include <pulse/ext-device-restore.h>
#include <pulse/ext-stream-restore.h>
#include <pulse/internal.h>
#include <pulse/audiocontrol.h>

#include <pulsecore/log.h>
#include <pulsecore/tagstruct.h>
#include <pulsecore/i18n.h>
#include <pulsecore/macro.h>
#include <pulsecore/core-util.h>
#include <pulsecore/log.h>
#include <pulsecore/pstream-util.h>


//#define REMOTE_MODULE_NAME  "module-hmdsp-sink"
#define REMOTE_MODULE_NAME  "module-combine-sink"


/* Try enough times to make sure the context is in ready status */
#define CONNECT_TRY_CNT     (400)


static pa_threaded_mainloop *mainloop = NULL;

static void context_state_callback(pa_context *c, PA_UNUSED void *userdata) {
    pa_assert(c);

    switch (pa_context_get_state(c)) {
        case PA_CONTEXT_READY:
        case PA_CONTEXT_TERMINATED:
        case PA_CONTEXT_FAILED:
            pa_threaded_mainloop_signal(mainloop, 0);
            break;

        case PA_CONTEXT_UNCONNECTED:
        case PA_CONTEXT_CONNECTING:
        case PA_CONTEXT_AUTHORIZING:
        case PA_CONTEXT_SETTING_NAME:
            break;
    }
}


static void context_event_callback(pa_context *c, const char *event, pa_proplist *p, void *event_userdata)
{
    size_t nbytes = 0;
    event_cb_t cb = (event_cb_t)event_userdata;
    t_Tagged_MSG_Params *msg = NULL;


    //pa_log("context_event_callback!");
    pa_assert(c);

    if(strcmp(event, DSP_EVENT)){
        pa_log("ignores event message it doesn't know");
        return;
    }


    pa_proplist_get(p, PA_PROP_DSP_EVENT, (void *)&msg, &nbytes);

#if 0 /*DEBUG*/
    pa_log("get nbytes = %d",nbytes);


    pa_log("e_AC_Destination    : 0x%x", msg->e_AC_Destination);
    pa_log("e_Msg_Destination   : 0x%x", msg->e_Msg_Destination);
    pa_log("e_Asp_Module        : 0x%x", msg->e_Asp_Module);

    pa_log("SessionID : 0x%x", msg->x_AM_Params.Session_ID);
    pa_log("CmdID     : 0x%x", msg->x_AM_Params.Cmd_ID);
    pa_log("CmdType   : 0x%x", msg->x_AM_Params.Cmd_Type);
    pa_log("InstID    : 0x%x", msg->x_AM_Params.InstID);
    pa_log("Length    : 0x%x", msg->x_AM_Params.Length);

    for(i=0; i<msg->x_AM_Params.Length; i++){
        pa_log("    Data[%d]    : 0x%x", i, msg->x_AM_Params.Data[i]);
    }
#endif

    cb(msg);

}


/*create a connection to pulseaudio, waiting for the processing complete within 1000ms*/
pa_context *pa_ac_create_connection(void)
{
    int ret = PA_ERR_MAX;
    int connect_try = 0;

    pa_mainloop_api *mainloop_api = NULL;
    pa_context *context = NULL;

    /* Set up a new mainloop */
    if (!(mainloop = pa_threaded_mainloop_new())) {
        pa_log(_("pa_mainloop_new() failed."));
        goto quit;
    }

    if (!(mainloop_api = pa_threaded_mainloop_get_api(mainloop))) {
        pa_log(_("pa_threaded_mainloop_get_api() failed."));
        goto quit;
    }

    context = pa_context_new(mainloop_api, NULL);
    if (context == NULL) {
        pa_log(_("pa_context_new() failed."));
        goto quit;
    }

    pa_context_set_state_callback(context, context_state_callback, NULL);

    while(1) {
        if(connect_try == CONNECT_TRY_CNT) {
            pa_log(_("pa_context_connect() failed, pulseaudio not running?"));
            goto quit;
        }
        connect_try++;
        if (pa_context_connect(context, NULL, PA_CONTEXT_NOFAIL, NULL) < 0) {
            pa_log(_("pa_context_connect() failed %d times: %s, retry after 5ms"), connect_try, pa_strerror(pa_context_errno(context)));
        }else{
            pa_log(_("pa_context_connect() success"));
            break;
        }

        pa_msleep(5);
    }

    pa_threaded_mainloop_lock(mainloop);

    if(pa_threaded_mainloop_start(mainloop) < 0) {
        pa_log(_("pa_threaded_mainloop_start() failed"));
        goto unlock_and_fail;
    }

	while(1) {
		pa_context_state_t state;

		state = pa_context_get_state(context);
		if (state == PA_CONTEXT_READY)
			break;

		if (!PA_CONTEXT_IS_GOOD(state)) {
			goto unlock_and_fail;
		}
		/* Wait until the context is ready */
		pa_threaded_mainloop_wait(mainloop);
	}

	pa_threaded_mainloop_unlock(mainloop);

	pa_log("In %s(), new context creating: %p", __func__, context);

	return context;

unlock_and_fail:
		pa_threaded_mainloop_unlock(mainloop);

quit:
    if (context)
        pa_context_unref(context);

    if (mainloop) {
        pa_threaded_mainloop_free(mainloop);
    }

    return NULL;
}


void pa_ac_set_event_callback(pa_context *c, event_cb_t cb)
{
    pa_proplist *proplist = NULL;

    pa_assert(c);

    if(!cb){
        pa_log_error("%s: callback is NULL",__func__);
    }

    proplist = pa_proplist_new();
    pa_proplist_sets(proplist, PA_PROP_AUDIO_MANAGER, "true");
    pa_context_proplist_update(c, PA_UPDATE_REPLACE, proplist, NULL, NULL);
    pa_context_set_event_callback(c, context_event_callback, cb);
}


/*send a AC command to DSP*/
/*this function always return OK, the command status is check in CB*/
void pa_ac_send_command(pa_context *c, t_Tagged_MSG_Params *msg, cmd_status_cb_t cb)
{
    if (mainloop) {
        pa_threaded_mainloop_lock(mainloop);
        pa_context_ext_audio_framework_msg_send(c, MSG_AM_CMD, \
				(void *)msg, cb, NULL, REMOTE_MODULE_NAME);
        pa_threaded_mainloop_unlock(mainloop);
    } else {
        /* mainloop is NULL, print context for debug */
        pa_log("%s() failed, mainloop is NULL, context %p", __func__, c);
    }
}


void pa_ac_set_dump(pa_context *c, t_dump_params *dump_params, dump_status_cb_t cb)
{
    pa_context_ext_audio_framework_msg_send(c, MSG_DUMP_CMD, \
        (void *)dump_params, cb, NULL, REMOTE_MODULE_NAME);

}

int pa_ac_is_connection_good(pa_context *c)
{
    if(!c){
        return 0;
    }

    return PA_CONTEXT_IS_GOOD(pa_context_get_state(c));
}

int pa_ac_disconnect(pa_context *c)
{
    pa_log("In %s", __func__);

    if(PA_UNLIKELY(!c || !mainloop))
    {
        pa_log("%s failed, context %p, mainloop %p", __func__, c, mainloop);
        return -1;
    }

    pa_threaded_mainloop_lock(mainloop);
    pa_context_disconnect(c);
    pa_threaded_mainloop_unlock(mainloop);
    pa_threaded_mainloop_stop(mainloop);
    pa_threaded_mainloop_free(mainloop);
    mainloop = NULL;

    pa_log("In %s, disconnected", __func__);

    return 0;
}


pa_operation* pa_ac_load_module(pa_context *c, const char *name, const char *argument, pa_context_index_cb_t cb, void *userdata) {
    pa_operation *o = NULL;

    if (mainloop) {
        //pa_log("pa_ac_load_module");
        pa_threaded_mainloop_lock(mainloop);
        o = pa_context_load_module(c, name, argument, cb, userdata);
        pa_threaded_mainloop_unlock(mainloop);
    } else {
        /* mainloop is NULL, print context for debug */
        pa_log("%s() failed, mainloop is NULL, context %p", __func__, c);
        return NULL;
    }

    return o;
}

pa_operation* pa_ac_unload_module(pa_context *c, uint32_t idx, pa_context_success_cb_t cb, void *userdata) {
    pa_operation *o = NULL;

    if (mainloop) {
        //pa_log("pa_ac_unload_module");
        pa_threaded_mainloop_lock(mainloop);
        o = pa_context_unload_module(c, idx, cb, userdata);
        pa_threaded_mainloop_unlock(mainloop);
    } else {
        /* mainloop is NULL, print context for debug */
        pa_log("%s() failed, mainloop is NULL, context %p", __func__, c);
        return NULL;
    }

    return o;
}



