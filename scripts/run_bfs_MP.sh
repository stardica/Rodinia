#!/bin/bash

source ./scripts/paths.sh

echo " "
echo "---Running Breadth-First Search OpenMP on Simulator---"
for i in 1 2 4
do
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name $benchmark_path/OpenMP/bfs/bfs $i $benchmark_path/data/bfs/graph128.txt
done
