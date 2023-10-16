#!/bin/bash

./init_jailhouse_env.sh

../tools/jailhouse disable

../tools/jailhouse enable ../cells/imx93.cell;
../tools/jailhouse cell create ../cells/imx93-inmate-demo.cell;
../tools/jailhouse cell load 1 ../inmates/gic-demo.bin
../tools/jailhouse cell start 1
