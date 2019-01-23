/***
  Copyright 2008 HARMAN COC-media

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

#include <pulse/context.h>
#include <pulse/fork-detect.h>
#include <pulse/operation.h>

#include <pulsecore/macro.h>
#include <pulsecore/pstream-util.h>

#include <pulse/direction.h>
#include <pulse/xmalloc.h>
#include <pulsecore/core-util.h>

#include "internal.h"
#include "introspect.h"

#include "ext-dsp-framework.h"

static void pa_context_ext_audio_framework_msg_send_cb(pa_pdispatch *pd, uint32_t command, uint32_t tag, pa_tagstruct *t, void *userdata) {
    pa_operation *o = userdata;
    uint8_t status = 0;

    pa_assert(pd);
    pa_assert(o);
    pa_assert(PA_REFCNT_VALUE(o) >= 1);

    if (!o->context)
        goto finish;

    if (command != PA_COMMAND_REPLY) {
        if (pa_context_handle_error(o->context, command, t, false) < 0)
            goto finish;
    } else {
        if(!pa_tagstruct_eof(t))
            pa_tagstruct_getu8(t, &status);
    }

    if (o->callback) {
        cmd_status_cb_t cb = (cmd_status_cb_t) o->callback;
        cb(status);
    }

finish:
    pa_operation_done(o);
    pa_operation_unref(o);
}

pa_operation* pa_context_ext_audio_framework_msg_send(
	pa_context *c,
	msg_type_t msg_type,
	void *data,
	cmd_status_cb_t cb,
	void *userdata,
	const char *name) {

    uint32_t tag;
    pa_operation *o;
    pa_tagstruct *t;
    uint16_t data_index;
    t_Tagged_MSG_Params *msg;
    t_dump_params *p_dump_params;


    pa_assert(c);
    pa_assert(PA_REFCNT_VALUE(c) >= 1);

    PA_CHECK_VALIDITY_RETURN_NULL(c, !pa_detect_fork(), PA_ERR_FORKED);
    PA_CHECK_VALIDITY_RETURN_NULL(c, c->state == PA_CONTEXT_READY, PA_ERR_BADSTATE);
    PA_CHECK_VALIDITY_RETURN_NULL(c, c->version >= 14, PA_ERR_NOTSUPPORTED);

    o = pa_operation_new(c, NULL, (pa_operation_cb_t) cb, userdata);

    t = pa_tagstruct_command(c, PA_COMMAND_EXTENSION, &tag);
    pa_tagstruct_putu32(t, PA_INVALID_INDEX);
    pa_tagstruct_puts(t, name);

    if(msg_type == MSG_AM_CMD) {
        pa_tagstruct_putu32(t, (uint32_t)MSG_AM_CMD);
        if(data){
            msg = (t_Tagged_MSG_Params *)data;
            pa_tagstruct_putu32(t, (uint32_t)msg->e_AC_Destination);
            pa_tagstruct_putu32(t, (uint32_t)msg->e_Msg_Destination);
            pa_tagstruct_putu32(t, (uint32_t)msg->e_Asp_Module);
            pa_tagstruct_putu32(t, (uint32_t)msg->x_AM_Params.Session_ID);
            pa_tagstruct_putu32(t, (uint32_t)msg->x_AM_Params.Cmd_ID);
            pa_tagstruct_putu32(t, (uint32_t)msg->x_AM_Params.Cmd_Type);
            pa_tagstruct_putu32(t, (uint32_t)msg->x_AM_Params.InstID);
            pa_tagstruct_putu32(t, (uint32_t)msg->x_AM_Params.Length);
            for(data_index=0; data_index < msg->x_AM_Params.Length; data_index++){
                pa_tagstruct_putu8(t, msg->x_AM_Params.Data[data_index]);
            }
        }
    }else if(msg_type == MSG_DUMP_CMD) {
        pa_tagstruct_putu32(t, (uint32_t)MSG_DUMP_CMD);
        if(data){
            p_dump_params = (t_dump_params *)data;
            pa_tagstruct_putu32(t, p_dump_params->time);
            pa_tagstruct_puts(t, p_dump_params->path);
        }
    }else{
        pa_log_error("unknow msg type");
    }


    pa_pstream_send_tagstruct(c->pstream, t);
    pa_pdispatch_register_reply(c->pdispatch, tag, DEFAULT_TIMEOUT, pa_context_ext_audio_framework_msg_send_cb, pa_operation_ref(o), (pa_free_cb_t) pa_operation_unref);


    return o;
}

