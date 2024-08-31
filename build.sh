#!/bin/bash

echo "INFO: setting ns3 version and path"
echo "INFO: you can set version and path to ns3 using 1st and 2nd argument"

export NS3_VERSION=$1 #42
export NS3_PATH=$2 #~/.local/lib/ns-allinone-3.42/ns-3.42

echo "building..."

mkdir build
cd build
cmake ..
make

echo "

INFO: Your executable are in ./build/bin/

"

echo "INFO: Creating python venv"
cd ..
python -m venv .venv
source .venv/bin/activate
.venv/bin/pip install pandas
source .venv/bin/activate
