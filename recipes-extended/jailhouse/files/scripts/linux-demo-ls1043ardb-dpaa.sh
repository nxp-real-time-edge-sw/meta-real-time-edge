#!/bin/bash

./init_jailhouse_env.sh

cp /run/media/boot-mmcblk0p1/Image /usr/share/jailhouse/inmates/kernel/

../tools/jailhouse disable;

../tools/jailhouse enable ../cells/ls1043a-rdb.cell;
../tools/jailhouse cell linux ../cells/ls1043a-rdb-dpaa-linux-demo.cell -i ../inmates/rootfs/rootfs.cpio ../inmates/kernel/Image -d ../inmates/dtb/inmate-ls1043a-rdb-dpaa.dtb -c "console=ttyS0,115200 earlycon=uart8250,mmio,0x21c0600"
