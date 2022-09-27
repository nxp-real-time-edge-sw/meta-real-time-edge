// SPDX-License-Identifier: (GPL-2.0 OR MIT)
/*
 * Copyright 2022 NXP
 */

#include <stdio.h>
#include <stdlib.h>

#include "common.h"

char DebugOpt[] = "debug";
char TcpOptVal[] = "tcp";
char RtuOptVal[] = "rtu";
char *baud_rate_map[] = {"4800", "9600", "19200", "115200"};

int free_modbus_params(char *opt_port, char *opt_baud, char *opt_data_bits,
                       char *opt_stop_bits) {
  if (opt_baud) {
    free(opt_baud);
  }
  if (opt_stop_bits) {
    free(opt_stop_bits);
  }
  if (opt_data_bits) {
    free(opt_data_bits);
  }
  if (opt_port) {
    free(opt_port);
  }

  return 0;
}

int getInt(const char str[], int *value) {
  int val;
  int ret = sscanf(str, "0x%x", &val);
  if (0 >= ret) {
    ret = sscanf(str, "%d", &val);
  }

  if (value) {
    *value = val;
  }

  return ret >= 0 ? 1 : 0;
}