#on simulator
echo " "
echo "---Running SRADv1 vMP on simulator---"
export LD_LIBRARY_PATH=/home/stardica/Desktop/runtime/Release
/home/stardica/Desktop/m2s-cgm/Release/m2s-cgm --x86-sim detailed --x86-config /home/stardica/Desktop/m2s-cgm/src/config/Intel-i7-4790k-CPU-Config.ini --si-sim detailed --si-config /home/stardica/Desktop/m2s-cgm/src/config/Radeon-HD-7870-GPU-Config.ini /home/stardica/Dropbox/CDA6198/sradworkingformbinary/srad 1 0.5 502 458

#--mem-config /home/stardica/Desktop/m2s-cgm/src/config/cgm-config.ini
