#!/bin/bash

source ./scripts/paths.sh

echo " "
echo "---Running SRADv1 OpenMP on simulator---"
for i in 1 2 4
do
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name $benchmark_path/OpenMP/srad/srad_v1/srad 1 0.5 110 100 $i
done
#--mem-config /home/stardica/Desktop/m2s-cgm/src/config/cgm-config.ini
