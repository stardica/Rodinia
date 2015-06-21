#!/bin/bash

source /home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/Scripts/paths.sh

echo " "
echo "---Running SRADv2 OpenMP on simulator---"
for i in 1 2 4
do
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name --si-sim detailed --si-config $config_file_path/$gpu_config_file_name --mem-config $config_file_path/$mem_config_file_name $benchmark_path/OpenMP/srad/srad_v2/srad 128 128 0 32 0 32 $i 0.5 1
done
