#!/bin/bash

source ./scripts/paths.sh

echo " "
echo "---Running Particle Filter OpenMP on simulator---"
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name $benchmark_path/OpenMP/particlefilter/particle_filter -x 128 -y 128 -z 1 -np 1
