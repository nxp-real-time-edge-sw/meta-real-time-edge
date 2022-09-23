#!/bin/bash

./init_jailhouse_env.sh

cp /run/media/boot-mmcblk1p1/Image /usr/share/jailhouse/inmates/kernel/

../tools/jailhouse disable;

../tools/jailhouse enable ../cells/imx8mp.cell;
../tools/jailhouse cell linux ../cells/imx8mp-linux-demo.cell ../inmates/kernel/Image -d ../inmates/dtb/inmate-imx8mp-evk.dtb -c "clk_ignore_unused console=ttymxc3,115200 earlycon=ec_imx6q,0x30890000,115200 root=/dev/mmcblk2p2 rootwait rw"

