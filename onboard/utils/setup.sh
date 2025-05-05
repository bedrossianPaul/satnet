#!/bin/bash

IFACE="$1"
CHANNEL="$2"
IPADDR="$3"
ESSID="SatNet"

channel_to_freq() {
  case "$1" in
    1) echo 2412 ;;
    2) echo 2417 ;;
    3) echo 2422 ;;
    4) echo 2427 ;;
    5) echo 2432 ;;
    6) echo 2437 ;;
    7) echo 2442 ;;
    8) echo 2447 ;;
    9) echo 2452 ;;
    10) echo 2457 ;;
    11) echo 2462 ;;
    *) echo "❌ Unsupported channel $1" >&2; exit 1 ;;
  esac
}

if [ $# -lt 3 ]; then
  echo "Usage: $0 <interface> <channel> <IP address>"
  echo "Example: $0 wlan0 1 10.0.0.1"
  exit 1
fi

echo "🧹 Cleaning network interfaces..."
sudo systemctl stop wpa_supplicant.service 2>/dev/null || true
sudo systemctl stop NetworkManager 2>/dev/null || true
sudo killall wpa_supplicant 2>/dev/null || true
sudo killall dhclient 2>/dev/null || true
sudo ip link set $IFACE down || true
sudo iw dev $IFACE ibss leave 2>/dev/null || true
sudo ip addr flush dev $IFACE
ip link show tap0 &>/dev/null && sudo ip link delete tap0
ip link show lwipbridge &>/dev/null && sudo ip link delete lwipbridge

echo "⚙️ Configuring tap0 and lwipbridge..."
export PRECONFIGURED_TAPIF=tap0
USER=`whoami`

sudo ip tuntap add dev $PRECONFIGURED_TAPIF mode tap user $USER
sudo ip link set $PRECONFIGURED_TAPIF up
sudo brctl addbr lwipbridge
sudo brctl addif lwipbridge $PRECONFIGURED_TAPIF
sudo ip addr add 192.168.1.1/24 dev lwipbridge
sudo ip link set dev lwipbridge up


# Convert channel number to frequency in MHz (only basic mapping)
FREQ=$(channel_to_freq "$CHANNEL")

echo "📡 Joining IBSS network '$ESSID' on frequency $FREQ MHz (channel $CHANNEL)..."

sudo iw dev $IFACE set type ibss
sudo ip link set $IFACE up
sudo iw dev $IFACE ibss join "$ESSID" $FREQ
sudo ip a a $IPADDR/24 dev $IFACE
sudo ip link set dev $IFACE up

echo "🔀 Enbale IP forwarding"
echo 1 | sudo tee /proc/sys/net/ipv4/ip_forward
sysctl -p

echo "💻 Reset NAT"
# Flush toutes les règles (toutes les tables)
sudo iptables -F
sudo iptables -t nat -F
sudo iptables -t mangle -F
sudo iptables -t raw -F

# Supprimer toutes les chaînes personnalisées
sudo iptables -X
sudo iptables -t nat -X
sudo iptables -t mangle -X
sudo iptables -t raw -X

# Réinitialiser les politiques par défaut
sudo iptables -P INPUT ACCEPT
sudo iptables -P FORWARD ACCEPT
sudo iptables -P OUTPUT ACCEPT




echo "✅ $IFACE is now in ad-hoc mode (IBSS) with IP $IPADDR on channel $CHANNEL. And ready to use LWIP"
