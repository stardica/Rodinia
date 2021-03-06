#!/bin/bash

source /home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/Scripts/paths.sh

echo " "
echo "---Running Stream Culster OpenMP on simulator---"
for i in 1 2 4
do
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name --si-sim detailed --si-config $config_file_path/$gpu_config_file_name --mem-config $config_file_path/$mem_config_file_name $benchmark_path/OpenMP/streamcluster/sc_omp 10 20 8 16 16 20 none $benchmark_path/output $i
done
# --mem-config /home/stardica/Desktop/m2s-cgm/src/config/cgm-config.ini
