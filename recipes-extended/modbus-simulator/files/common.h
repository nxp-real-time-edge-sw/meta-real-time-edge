// SPDX-License-Identifier: (GPL-2.0 OR MIT)
/*
 * Copyright 2022 NXP
 */

#ifndef COMMON_H
#define COMMON_H

extern char DebugOpt[];
extern char TcpOptVal[];
extern char RtuOptVal[];
extern char *baud_rate_map[];

int free_modbus_params(char *opt_port, char *opt_baud, char *opt_data_bits,
                       char *opt_stop_bits);

int getInt(const char str[], int *value);

#endif /* COMMON_H */