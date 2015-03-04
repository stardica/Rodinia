#!/bin/bash

source ./scripts/paths.sh

echo " "
echo "---Running Path Finder OpenMP on simulator---"
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name $benchmark_path/OpenMP/pathfinder/pathfinder 256 16

#--mem-config /home/stardica/Desktop/m2s-cgm/src/config/cgm-config.ini
