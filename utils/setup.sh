#!/bin/bash

set -e # Stop script on error

# Args
BOOT="$1"
ROOT="$2"

# Network default settings
NS_IP="192.168.0.10"
GATEWAY_IP="192.168.0.1"
NETMASK="255.255.255.0"

# Default admin user
USERNAME="admin"
PASSWORD="irit2025"

if [ $# -lt 2 ]; then
  echo "Usage: $0 <boot_partition> <root_partition>"
  echo "Exemple: $0 /dev/sdb1 /dev/sdb2"
  exit 1
fi

BOOT_MOUNT="/mnt/ns_boot"
ROOT_MOUNT="/mnt/ns_root"

# Mount the SD card
echo "💾 Mounting SD card..."
sudo mkdir -p "$BOOT_MOUNT" "$ROOT_MOUNT"
sudo mount "$BOOT" "$BOOT_MOUNT"
sudo mount "$ROOT" "$ROOT_MOUNT"

# Setup the default network settings for the ssh connection
echo "🛠️ Network configuration"
sudo sed -i "s|$| ip=${NS_IP}::${GATEWAY_IP}:${NETMASK}::eth0:off|" "$BOOT_MOUNT/cmdline.txt"

# Activate SSH
echo "🔓 SSH activation..."
sudo touch "$BOOT_MOUNT/ssh"

# User configuration
echo "👤 Creating $USERNAME user account..."
HASHED_PASSWORD=$(echo "$PASSWORD" | openssl passwd -6 -stdin)
echo "$USERNAME:$HASHED_PASSWORD" | sudo tee "$BOOT_MOUNT/userconf.txt" > /dev/null

# Unmount the SD card
echo "💾 Unmounting SD card..."
sudo umount "$BOOT_MOUNT"
sudo umount "$ROOT_MOUNT"

# Remove the mount points
echo "🗑️ Removing mount points..."
sudo rm -rf "$BOOT_MOUNT"
sudo rm -rf "$ROOT_MOUNT"

# End
echo "✅ Configuration terminée. Vous pouvez éjecter la carte SD."
