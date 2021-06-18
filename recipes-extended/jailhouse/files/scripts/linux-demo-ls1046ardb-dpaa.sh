#!/bin/bash

./init_jailhouse_env.sh

../tools/jailhouse disable;

../tools/jailhouse enable ../cells/ls1046a-rdb.cell;
../tools/jailhouse cell linux ../cells/ls1046a-rdb-dpaa-linux-demo.cell -i ../inmates/rootfs/rootfs.cpio ../inmates/kernel/Image -d ../inmates/dtb/inmate-ls1046a-rdb-dpaa.dtb -c "console=ttyS0,115200 earlycon=uart8250,mmio,0x21c0600"

