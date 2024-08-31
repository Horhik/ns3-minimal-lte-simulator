#!/usr/bin/env sh

echo "INFO: Running Simulation..."
./build/bin/minimal-lte

echo "INFO: Running Throughput computation..."
.venv/bin/python throughput-compute.py
