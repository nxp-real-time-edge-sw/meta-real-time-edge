#!/bin/bash

CPUREV=$(cat /sys/devices/soc0/soc_id)
if [ $CPUREV == "i.MX8MP" ]
then
    echo "config sysrepo-tsn related components"
else
    exit 0
fi

CMD=$1

if [ $CMD == "start" ]; then
    /etc/init.d/sysrepo-init ${CMD}
    /etc/init.d/sysrepod ${CMD}
    /etc/init.d/sysrepo-plugind ${CMD}
    /etc/init.d/sysrepo-tsnd ${CMD}
    /etc/init.d/netopeer2-server ${CMD}
else
    /etc/init.d/netopeer2-server ${CMD}
    /etc/init.d/sysrepo-tsnd ${CMD}
    /etc/init.d/sysrepo-plugind ${CMD}
    /etc/init.d/sysrepod ${CMD}
fi

exit 0
