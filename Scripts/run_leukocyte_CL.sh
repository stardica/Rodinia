#!/bin/bash

source /home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/Scripts/paths.sh

echo " "
echo "---Running Leukocyte OpenCL on simulator---"
#export LD_LIBRARY_PATH=$library_path
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name --si-sim detailed --si-config $config_file_path/$gpu_config_file_name --mem-config $config_file_path/$mem_config_file_name $benchmark_path/OpenCL/leukocyte/OpenCL/leukocyte $benchmark_path/data/leukocyte/testfile.avi 0

#--mem-config /home/stardica/Desktop/m2s-cgm/src/config/cgm-config.ini
