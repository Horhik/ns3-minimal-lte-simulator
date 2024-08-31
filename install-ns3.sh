#!/usr/bin/env sh

echo "Downloading ns3 v42"

wget https://www.nsnam.org/releases/ns-allinone-3.42.tar.bz2
tar -xvjf  /tmp/ns-allinone-3.42.tar.bz2 -C ~/.local

cd ./ns-allinone-3.42/ns-3.42

echo "Building ns3"
./ns3 configure
./ns3 build

echo "setting build arguments"

export NS3_VERSION=42
export NS3_PATH=~/.local/ns-allinone-3.42/ns-3.42
