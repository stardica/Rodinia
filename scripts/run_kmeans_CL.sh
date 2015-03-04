#!/bin/bash

source ./scripts/paths.sh

echo " "
echo "---Running K Means OpenCL on simulator---"
export LD_LIBRARY_PATH=$library_path
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name --si-sim detailed --si-config $config_file_path/$gpu_config_file_name $benchmark_path/OpenCL/kmeans/kmeans -o -i $benchmark_path/data/kmeans/100
