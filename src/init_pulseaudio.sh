#!/system/bin/sh

# enhance kernel log
#echo 'file *snd* +p' > /sys/kernel/debug/dynamic_debug/control
#echo 'file *skl* +p' > /sys/kernel/debug/dynamic_debug/control
#echo 'file *soc* +p' > /sys/kernel/debug/dynamic_debug/control
#echo 'file *hda* +p' > /sys/kernel/debug/dynamic_debug/control

# set pipe line for Intel I2S[3] <-> Dirana3 TDM[0]
#/system/bin/alsa_amixer -c0 cset name='dirana_out mo media1_in mi Switch' 'On'


# set piple line for I2S[1] Bluetooth downlink and uplink
#/system/bin/alsa_amixer -c0 cset name='bthfp_pt_pb mo media2_in mi Switch' 'On'
#/system/bin/alsa_amixer -c0 cset name='media1_out mo bthfp_pt_cp mi Switch' 'On'


# set pipe line for Intel I2S[4] <-> Dirana3 TDM[1]
#/system/bin/alsa_amixer -c0 cset name='modem_pt_pb mo media3_in mi Switch' 'On'
#/system/bin/alsa_amixer -c0 cset name='media2_out mo modem_pt_cp mi Switch' 'On'

# start pulseaudio
if [ `getprop avbstreamhandler.status` = "up" ]
then
echo `date`": AVB runs up, all function bootup   " > /data/pulseaudio/pa_boot.log
export PULSE_SCRIPT=/etc/pulse/default.pa
elif [ `getprop avbstreamhandler.status` = "exit"]
then
echo `date`": AVB runs failed, bootup without AVB" > /data/pulseaudio/pa_boot.log
export PULSE_SCRIPT=/etc/pulse/default.withoutAVB.pa
fi

/system/bin/pulseaudio

