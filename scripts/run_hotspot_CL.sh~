#!/bin/bash

source ./scripts/paths.sh

echo " "
echo "---Running Hot Spot OpenCL on simulator---"
export LD_LIBRARY_PATH=$library_path
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name --si-sim detailed --si-config $config_file_path/$gpu_config_file_name $benchmark_path/OpenCL/hotspot/hotspot 64 2 1 $benchmark_path/data/hotspot/temp_64 $benchmark_path/data/hotspot/power_64 $/hotspot_output_cl

#--mem-config /home/stardica/Desktop/m2s-cgm/src/config/cgm-config.ini
