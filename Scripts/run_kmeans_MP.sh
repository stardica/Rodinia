#!/bin/bash

source /home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/Scripts/paths.sh

echo " "
echo "---Running K Means OpenMP on simulator---"
for i in 1 2 4
do
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name --si-sim detailed --si-config /home/stardica/Desktop/m2s-cgm/src/config/Radeon-HD-7870-GPU-Config.ini --mem-config $config_file_path/$mem_config_file_name $benchmark_path/OpenMP/kmeans/kmeans_openmp/kmeans -n $i -i $benchmark_path/data/kmeans/100
done
