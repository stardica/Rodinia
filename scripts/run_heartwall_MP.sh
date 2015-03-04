#!/bin/bash

source ./scripts/paths.sh

echo " "
echo "---Running Heart Wall OpenMP on Simulator---"
for i in 1 2 4
do
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name $benchmark_path/OpenMP/heartwall/heartwall $benchmark_path/data/heartwall/test.avi 1 $i
done
