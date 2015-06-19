#!/bin/bash

source ./scripts/paths.sh

echo " "
echo "---Running cfd OpenCL on simulator---"
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name $benchmark_path/OpenMP/cfd/euler3d_cpu_p1 $benchmark_path/data/cfd/fvcorr.domn.097K

$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name $benchmark_path/OpenMP/cfd/euler3d_cpu_p2 $benchmark_path/data/cfd/fvcorr.domn.097K

$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name $benchmark_path/OpenMP/cfd/euler3d_cpu_p4 $benchmark_path/data/cfd/fvcorr.domn.097K
