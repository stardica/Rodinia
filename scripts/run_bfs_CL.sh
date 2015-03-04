#!/bin/bash

source ./scripts/paths.sh

echo " "
echo "---Running Breadth-First Search OpenCL on Simulator---"
export LD_LIBRARY_PATH=$library_path
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name --si-sim detailed --si-config $config_file_path/$gpu_config_file_name $benchmark_path/OpenCL/bfs/bfs $benchmark_path/data/bfs/graph128.txt

#--mem-config /home/stardica/Desktop/m2s-cgm/src/config/cgm-config.ini
