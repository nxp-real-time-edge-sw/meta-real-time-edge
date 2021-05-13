#!/bin/bash

./init_jailhouse_env.sh

../tools/jailhouse disable

../tools/jailhouse enable ../cells/ls1043a-rdb.cell;
../tools/jailhouse cell create ../cells/ls1043a-rdb-inmate-demo.cell;
../tools/jailhouse cell load 1 ../inmates/ivshmem-demo.bin
../tools/jailhouse cell start 1

../tools/ivshmem-demo -d /dev/uio0 -t 1
