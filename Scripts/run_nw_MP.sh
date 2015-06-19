#!/bin/bash

source ./scripts/paths.sh

echo " "
echo "---Running Needleman Wunsch OpenMP on simulator---"
for i in 1 2 4
do
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name $benchmark_path/OpenMP/nw/needle 256 10 $i
done
