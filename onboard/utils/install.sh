#!/bin/bash

# Require root
if [[ $EUID -ne 0 ]]; then
   echo "👮 You must be root to launch this script." 
   exit 1
fi

# Check if the script is run from the correct directory
if [ ! -d "utils" ]; then
  echo "🚫 Please run this script from ~ directory"
  exit 1
fi

echo "📡 [1/2] System update..."
sudo apt update && sudo apt upgrade -y

echo "📦 [2/2] Installing necessary packages..."
sudo apt install -y git curl build-essential net-tools iproute2 libncurses-dev libssl-dev libz-dev libpcap-dev libcap-dev gcc make pkg-config iptables bridge-utils

# TODO : install lwip and move it

echo "✅ All done !"
