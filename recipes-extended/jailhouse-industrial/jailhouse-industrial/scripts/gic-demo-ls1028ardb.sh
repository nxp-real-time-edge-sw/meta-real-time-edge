#!/bin/bash

./init_jailhouse_env.sh

../tools/jailhouse disable

../tools/jailhouse enable ../cells/ls1028a-rdb.cell;
../tools/jailhouse cell create ../cells/ls1028a-rdb-inmate-demo.cell;
../tools/jailhouse cell load 1 ../inmates/gic-demo.bin
../tools/jailhouse cell start 1
