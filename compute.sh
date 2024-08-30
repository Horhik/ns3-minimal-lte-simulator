#!/bin/bash

ns3_dir=$1
source_dir=$PWD
bin_file=$ns3_dir/build/scratch/$(basename $source_dir)

echo "Building the program...
"

cd $ns3_dir
./ns3 build

echo "Running the program...
"

cd $source_dir
$bin_file

echo "Computing the throughput...
"
$source_dir/.venv/bin/python throughput-compute.py
