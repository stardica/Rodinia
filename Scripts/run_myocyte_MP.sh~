#!/bin/bash

source ./scripts/paths.sh

export LD_LIBRARY_PATH=/usr/lib/i386-linux-gnu/

echo " "
echo "---Running Myocyte OpenMP on Simulator---"
for i in 1 2 4
do
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name $benchmark_path/OpenMP/myocyte/myocyte.out 10 1 1 $i
done
