#!/bin/bash

./init_jailhouse_env.sh

../tools/jailhouse disable

../tools/jailhouse enable ../cells/imx8mp.cell;
../tools/jailhouse cell create ../cells/imx8mp-inmate-demo.cell;
../tools/jailhouse cell load 1 ../inmates/ivshmem-demo.bin
../tools/jailhouse cell start 1

../tools/ivshmem-demo -d /dev/uio0 -t 1
