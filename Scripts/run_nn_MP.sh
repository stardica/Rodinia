#!/bin/bash

source ./scripts/paths.sh

echo " "
echo "---Running NN OpenMP on simulator---"
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name $benchmark_path/OpenMP/nn/nn $benchmark_path/data/nn/list1k.txt 5 30 90
