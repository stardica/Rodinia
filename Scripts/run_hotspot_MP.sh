#!/bin/bash

source ./scripts/paths.sh

echo " "
echo "---Running Hot Spot OpenMP on simulator---"
for i in 1 2 4
do
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name $benchmark_path/OpenMP/hotspot/hotspot 64 64 1 $i $benchmark_path/data/hotspot/temp_64 $benchmark_path/data/hotspot/power_64
done

#--mem-config /home/stardica/Desktop/m2s-cgm/src/config/cgm-config.ini
