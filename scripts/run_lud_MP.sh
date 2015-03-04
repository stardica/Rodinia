#!/bin/bash

source ./scripts/paths.sh

echo " "
echo "---Running Lower Upper Decomposition OpenMP on simulator---"
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name $benchmark_path/OpenMP/lud/omp/lud_omp_p1 -s 128 -v

$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name $benchmark_path/OpenMP/lud/omp/lud_omp_p2 -s 128 -v

$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name $benchmark_path/OpenMP/lud/omp/lud_omp_p4 -s 128 -v
