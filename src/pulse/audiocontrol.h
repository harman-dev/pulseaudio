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


#ifndef fooaudiocontrolhfoo
#define fooaudiocontrolhfoo

#include <pulse/ext-dsp-framework.h>
#include <pulse/introspect.h>

pa_context *pa_ac_create_connection(void);
void pa_ac_set_event_callback(pa_context *c, event_cb_t cb);
void pa_ac_send_command(pa_context *c, t_Tagged_MSG_Params *msg, cmd_status_cb_t cb);
void pa_ac_set_dump(pa_context *c, t_dump_params *dump_params, dump_status_cb_t cb);
int  pa_ac_is_connection_good(pa_context *c);
int  pa_ac_disconnect(pa_context *c);
pa_operation* pa_ac_load_module(pa_context *c, const char *name, const char *argument, pa_context_index_cb_t cb, void *userdata);
pa_operation* pa_ac_unload_module(pa_context *c, uint32_t idx, pa_context_success_cb_t cb, void *userdata);

#endif
