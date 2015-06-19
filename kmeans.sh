#!/bin/bash

source /home/stardica/Dropbox/CDA7919DoctoralResearch/Rodinia_Benchmarks/Scripts/paths.sh

$scripts_path/run_kmeans_MP.sh
$scripts_path/run_kmeans_CL.sh
