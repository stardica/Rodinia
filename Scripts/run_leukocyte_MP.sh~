#!/bin/bash

source ./scripts/paths.sh

echo " "
echo "---Running Leukocyte OpenMP on simulator---"
for i in 1 2 4
do
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name $benchmark_path/OpenMP/leukocyte/OpenMP/leukocyte 1 $i $benchmark_path/data/leukocyte/testfile.avi
done

