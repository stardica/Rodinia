#!/bin/bash

source ./scripts/paths.sh

echo " "
echo "---Running B+Tree OpenMP on simulator---"
for i in 1 2 4
do
$m2s_cgm_path/m2s-cgm --x86-sim detailed --x86-config $config_file_path/$cpu_config_file_name $benchmark_path/OpenMP/b+tree/b+tree.out core $i file $benchmark_path/data/b+tree/mil.txt command $benchmark_path/data/b+tree/command.txt
done

#--mem-config /home/stardica/Desktop/m2s-cgm/src/config/cgm-config.ini
