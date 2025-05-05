#/bin/sh

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <local_path> <remote_path>"
    exit 1
fi

scp -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -r $1 admin@192.168.0.10:$2
