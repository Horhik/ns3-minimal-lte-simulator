#!/usr/bin/env sh

echo "Running Simulation..."
./build/bin/minimal-lte

echo "Running Throughput computation..."
.venv/bin/python throughput-compute.py
