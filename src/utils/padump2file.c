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


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pulse/ext-dsp-framework.h>
#include <pulse/audiocontrol.h>

static volatile int msg_pending;

static void dump_status_callback(int result)
{
    if(result == PA_DUMP_OK){
        printf("dump start\n");
    }else if(result == PA_DUMP_ERR_ACCESS){
        printf("dump file open failed\n");
    }else if(result == PA_DUMP_ERR_BUSY){
        printf("dump is already runing\n");
    }else{
        printf("unknow error\n");
    }

    msg_pending = 0;
}

int main(int argc, char *argv[])
{
    int opt =0;
    pa_context *context = NULL;
    int dump_time = 5;
    char path[128];
    struct tm *ptm;
    long ts;
    t_dump_params dump_params;

    printf("--------------------------------------------------\n");

    while ((opt = getopt(argc, argv, "t:")) != -1){
        switch (opt) {
            case 't':
            if(optarg) {
                dump_time = strtoul((char*) optarg, 0, 0);
            }
            break;
            default:
                printf("invalid option");
                return -1;
        }
    }

    if(dump_time <= 0) {
        printf("Invalid dump_time '%d'", dump_time);
        return -1;
    }

    printf("Connecting to pulseaudio...");
    context = pa_ac_create_connection();
    if(!context){
        printf("pa_create_connection failed, exit\n");
        return -1;
    }
    printf(" OK\n");

    ts = time(NULL);
    ptm = localtime(&ts);

    sprintf(dump_params.path, DUMP_HOME "/" "dump-%02d%02d-%02d%02d%02d-%ds/",
            ptm->tm_mon+1,ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec, dump_time);

    dump_params.time = dump_time;

    printf("Sending dump request %ds...\n", dump_params.time);
    printf("Path: %s...\n", dump_params.path);

    pa_ac_set_dump(context, &dump_params, (dump_status_cb_t)dump_status_callback);

    msg_pending = 1;
    /*wait for msg send complete*/
    while(msg_pending) {
        usleep(100000);
    }

    pa_ac_disconnect(context);

    printf("--------------------------------------------------\n");

    return 0;

}

