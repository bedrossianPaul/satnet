#!/bin/bash

set -e # Stop script on error

# CONFIGURATION
IFACE_LOCAL="$1"  # Config interface
IFACE_NET="$2"    # Internet access

# Default network settings
IP_LOCAL="192.168.0.1"
NETMASK="255.255.255.0"

# Require root
if [[ $EUID -ne 0 ]]; then
   echo "👮 You must be root to launch this script." 
   exit 1
fi

if [ $# -lt 2 ]; then
  echo "Usage: $0 <NS_interface> <Internet_interface>"
  echo "Exemple: $0 eth0 wlan0"
  exit 1
fi

# Cleanup function
# This function will be called when the script is interrupted
cleanup() {
  echo "🧹 Cleaning..."
  ip addr flush dev "$IFACE_LOCAL"
  ip link set "$IFACE_LOCAL" down
  iptables -t nat -D POSTROUTING -o "$IFACE_NET" -j MASQUERADE
  iptables -D FORWARD -i "$IFACE_NET" -o "$IFACE_LOCAL" -m state --state RELATED,ESTABLISHED -j ACCEPT
  iptables -D FORWARD -i "$IFACE_LOCAL" -o "$IFACE_NET" -j ACCEPT
  echo 0 > /proc/sys/net/ipv4/ip_forward
  USER_HOME=$(getent passwd "$SUDO_USER" | cut -d: -f6)
  KNOWN_HOSTS="$USER_HOME/.ssh/known_hosts"

  if [ -f "$KNOWN_HOSTS" ]; then
  echo "🧼 Removing SSH key for 192.168.0.10 from $KNOWN_HOSTS"
  ssh-keygen -R 192.168.0.10 -f "$KNOWN_HOSTS" 2>/dev/null || true
  fi
  echo "✅ Clean done."
  exit 0
}

trap cleanup SIGINT SIGTERM

# Local interface configuration
echo "🔧 Conf. $IFACE_LOCAL with IP $IP_LOCAL..."
ip addr flush dev "$IFACE_LOCAL"
ip addr add "$IP_LOCAL/24" dev "$IFACE_LOCAL"
ip link set "$IFACE_LOCAL" up

# Routing activation
echo "🌍 Activating forward option..."
echo 1 > /proc/sys/net/ipv4/ip_forward

# Configure NAT
echo "🔀 Config. forward rules (NAT)..."
iptables -t nat -A POSTROUTING -o "$IFACE_NET" -j MASQUERADE
iptables -A FORWARD -i "$IFACE_NET" -o "$IFACE_LOCAL" -m state --state RELATED,ESTABLISHED -j ACCEPT
iptables -A FORWARD -i "$IFACE_LOCAL" -o "$IFACE_NET" -j ACCEPT

echo "✅ Activated. Press Ctrl+C to stop."

while true; do sleep 1; done
