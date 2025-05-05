#!/bin/bash

# 🧑‍🔧 NSNBT Setup Script

# Require root
if [[ $EUID -ne 0 ]]; then
   echo "👮 You must be root to launch this script." 
   exit 1
fi

# Check if the script is run from the correct directory (i.e., project root)
if [ ! -d "utils" ]; then
  echo "🚫 Please run this script from the project root directory (~/NSNBT or similar)"
  exit 1
fi

echo "📡 [1/3] Updating system..."
sudo apt update && sudo apt upgrade -y

echo "📦 [2/3] Installing required packages..."
sudo apt install -y \
  git curl build-essential net-tools iproute2 \
  libncurses-dev libssl-dev libz-dev \
  libpcap-dev libcap-dev gcc make pkg-config \
  iptables bridge-utils

echo "🌐 [3/3] Install lwIP"
LWIP_DIR="netstack/lwip"
REPO_URL="https://git.savannah.nongnu.org/git/lwip.git"

if [ -d "$LWIP_DIR" ]; then
  echo "lwIP already exists at $LWIP_DIR. Skipping clone."
else
  git clone "$REPO_URL" "$LWIP_DIR"
  if [ $? -ne 0 ]; then
    echo "❌ Failed to clone lwIP repository. Please check your internet connection."
    exit 1
  fi
fi

echo "✅ All done! lwIP installed in $LWIP_DIR 🚀"
