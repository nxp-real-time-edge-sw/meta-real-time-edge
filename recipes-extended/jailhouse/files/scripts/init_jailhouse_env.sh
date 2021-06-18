#!/bin/bash

if [ ! -f "../inmates/kernel/jailhouse.ko" ]; then
	echo "copy jailhouse.ko from /lib/modules/`uname -r`/extra/driver/"
	cp /lib/modules/`uname -r`/extra/driver/jailhouse.ko ../inmates/kernel/
fi

insmod ../inmates/kernel/jailhouse.ko;
