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

#include <pulse/audiocontrol.h>

static volatile int msg_pending;
static volatile int event_pending;

static void help(const char *argv0) {

    printf("\n%s Usage: Send Audio Control Command to Audio Processing unit\n", argv0);
    printf("Reference to <Audio Controller Command Interface Specification>\n");

    printf("\n");
    printf("Examples:\n");
    printf("%s -C 0x2203 -I 0 -L 5 -D 0,0,0,0,100\n", argv0);

    printf("\n");
    printf(" Main operation:\n");
    printf("  -P    product\n");
    printf("  -C    command ID\n");
    printf("  -I    instance ID\n");
    printf("  -L    data length as byte\n");
    printf("  -D    data split with ','\n");
    printf("  -w    wait for event back\n");

}


static void event_callback(t_Tagged_MSG_Params *msg)
{
    int i;
    printf("Return an event:\n{\n");

    printf("    AC Dest   : %d\n", msg->e_AC_Destination);
    printf("    MsgDest   : %d\n", msg->e_Msg_Destination);
    printf("    Asp       : %d\n", msg->e_Asp_Module);

    printf("    SessionID : 0x%x\n", msg->x_AM_Params.Session_ID);
    printf("    CmdID     : 0x%x\n", msg->x_AM_Params.Cmd_ID);
    printf("    CmdType   : 0x%x\n", msg->x_AM_Params.Cmd_Type);
    printf("    InstID    : 0x%x\n", msg->x_AM_Params.InstID);
    printf("    Length    : %d\n", msg->x_AM_Params.Length);
    printf("    DATA      :");

    for(i=0; i<msg->x_AM_Params.Length; i++){
        printf(" %d", msg->x_AM_Params.Data[i]);
    }
    printf("\n}\n");
    event_pending = 0;
}

static void command_status_callback(int result)
{

    if(result == 0x01){
        printf("command sent OK!\n");
    }else{
        printf("command sent Fail!\n");
    }

    msg_pending = 0;
}

/*
./dummy_audiomanagement -P dra6xx -C 0x2203 -I 0 -L 5 -D 0,255,96,0,100
./dummy_audiomanagement -P dra6xx -C 0x2203 -I 0 -L 5 -D 0,0,0,0,100
*/
int main(int argc, char *argv[])
{
    int opt =0;
    t_Tagged_MSG_Params msg;
    int cmdID = -1;
    int instID = -1;
    int cmdLen = -1;
    int data_real_len = -1;
    int wait_event_back = 0;

    int len = 0;
    int i, j, k;
    int attempt;
    int cmdData[255];
    char cmdStr[255];
    char cmdStr1[20];

    pa_context *context = NULL;


    memset(&msg, 0, sizeof(msg));

    while ((opt = getopt(argc, argv, "bC:I:L:D:P:")) != -1){
        switch (opt) {
            case 'P':
            /*TODO*/
            break;

            case 'b':
                wait_event_back = 1;
            break;

            case 'C':
            if(optarg){
                cmdID = strtoul((char*) optarg, 0, 0);
                if(cmdID < 0){
                    printf("Invalid cmdID '%d'", cmdID);
                    return -1;
                }
            }
            break;

            case 'I':
            if(optarg){
                instID = strtoul((char*) optarg, 0, 0);
                if(instID  < 0){
                    printf("Invalid instID '%d'", instID );
                    return -1;
                }
            }
            break;

            case 'L':
            if(optarg){
                cmdLen = strtoul((char*) optarg, 0, 0);
                if(cmdLen  < 0){
                    printf("Invalid cmdLen '%d'", cmdLen);
                    return -1;
                }
            }
            break;

            case 'D':
            if(optarg){
                i = j = k = 0;
                len =  strlen(optarg);
                if(len >= sizeof(cmdStr)){
                    printf("Invalid data too long not support");
                    return -1;
                }
                memcpy(cmdStr, optarg, len);
                cmdStr[len] = '\0';
                for(j=0; j<(len+1); j++) {
                    if (cmdStr[j] == ',' || cmdStr[j] == '\0'){
                        memset(cmdStr1, 0, 20);
                        memcpy(cmdStr1, &cmdStr[j-k], k);
                        cmdData[i] = strtoul((char*) cmdStr1,0, 0);
                        i++;
                        k = -1;
                    }
                    k++;
                }
                data_real_len = i;
            }
            break;
        }
    }

    /*check data length*/
    if(data_real_len != cmdLen){
        printf("Invalid Data Length[%d]\n", cmdLen);
        return -1;
    }

    if(cmdID < 0 || instID < 0 || cmdLen < 0){
        printf("%s: You must specify cmdID & instID & cmdLen!\n",argv[0]);
        help(argv[0]);
        return -1;
    }

    printf("--------------------------------------------------\n");

    msg.e_AC_Destination = LOCAL_AC_INSTANCE_E;

    if(cmdID == 0x0201 || cmdID == 0x0202){
        msg.e_Msg_Destination = CROSSBAR_E;
    }else{
        msg.e_Msg_Destination = ASP_MODULE_E;
    }

    msg.x_AM_Params.Cmd_ID = cmdID;
    msg.x_AM_Params.Cmd_Type = CMD_SETGET;
    msg.x_AM_Params.InstID = instID;
    msg.x_AM_Params.Length = cmdLen;
    for(i=0;i<cmdLen;i++){
        msg.x_AM_Params.Data[i] = cmdData[i];
    }

    printf("Connecting to pulseaudio...");
    context = pa_ac_create_connection();
    if(context == NULL){
        printf("pa_create_connection failed, exit\n");
        return -1;
    }
    printf(" OK\n");

    if(wait_event_back){
        pa_ac_set_event_callback(context, event_callback);
    }

    printf("Sending Command to AudioProcessing...\n");
    printf("    cmdID  : 0x%x\n", cmdID);
    printf("    instID : 0x%x\n", instID);
    printf("    cmdLen : 0x%x\n", cmdLen);
    printf("    data   : [");
    for(i=0;i<cmdLen;i++)
        printf(" %d", cmdData[i]);
    printf("]\n");


    msg_pending = 1;
    event_pending = 1;
    pa_ac_send_command(context, &msg, (cmd_status_cb_t)command_status_callback);

    /*wait for msg send complete*/
    while(msg_pending){
        usleep(1000);
    }

    if(wait_event_back){
        /*wait event back for a limited time, it may no event back for some condition*/
        for(attempt=100;attempt>0;attempt--){
            if(event_pending == 0){
                break;
            }
            usleep(1000);
        }
        if(attempt == 0){
            //printf("wait event back timeout\n");
        }
    }
    pa_ac_disconnect(context);

    //printf("connection status %d\n", pa_is_connection_good());

    printf("--------------------------------------------------\n");

    return 0;

}

