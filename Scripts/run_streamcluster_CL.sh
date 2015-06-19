#!/bin/bash

source ./scripts/paths.sh

echo " "
echo "---Running Stream Cluster OpenCL on simulator---"
export LD_LIBRARY_PATH=$library_path
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name --si-sim detailed --si-config $config_file_path/$gpu_config_file_name $benchmark_path/OpenCL/StreamCluster/streamcluster 10 20 8 16 16 20 none $benchmark_path/output_gpu 1 -t gpu -d 0
#--mem-config /home/stardica/Desktop/m2s-cgm/src/config/cgm-config.ini
