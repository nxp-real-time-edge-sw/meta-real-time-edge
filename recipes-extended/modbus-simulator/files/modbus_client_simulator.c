// SPDX-License-Identifier: (GPL-2.0 OR MIT)
/*
 * Copyright 2022 NXP
 */

#include <getopt.h>
#include <modbus.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "errno.h"

static int server_socket = -1;
static int is_write_function = 0;
static int debug = 0;
static char *device_name = NULL;
static char *ip_address = "127.0.0.1";
static int start_reference_at0 = 0;
static int read_write_no = 1;
static int function_code = -1;
static int is_tcp = -1;
static int slave_addr = 1;
static int start_addr = 1;
static int timeout_ms = 1000;
static int opt;
static int use_backend;
static char *opt_port = NULL;
static char *opt_baud = NULL;
static char *opt_data_bits = NULL;
static char *opt_stop_bits = NULL;

enum { TCP = 1, RTU };
enum WriteDataType { DataInt, Data8Array, Data16Array } wDataType = DataInt;

union Data {
  int dataInt;
  uint8_t *data8;
  uint16_t *data16;
} data;

void printUsage(const char progName[]) {
  printf("%s [--%s] <-m {rtu|tcp}> [-a <slave-addr>] [-c <read-no>]\n\t"
         "[-r <start-addr>] [-t <function-code>] [-o <timeout-ms>] "
         "{rtu-params|tcp-params} {serialport|host} [write-data]\n",
         progName, DebugOpt);
  printf("NOTE: register start addrress starts at 0!\n");
  printf(
      "function-code:\n"
      "\t(0x01) Read Coils, (0x02) Read Discrete Inputs, (0x05) Write Single "
      "Coil\n"
      "\t(0x03) Read Holding Registers, (0x04) Read Input Registers, (0x06) "
      "WriteSingle Register\n");
  printf("NOTE: Set Baud rate Order: {0|1|2|3}, equivalent to "
         "{4800|9600|19200|115200}\n");
  printf("rtu-params:\n"
         "\t<-b {0|1|2|3}>, baud-rate\n"
         "\t[-d {7|8}], data-bits\n"
         "\t[-s {1|2}], stop-bits\n"
         "\t[-p {none|even|odd}], verify\n");
  printf("tcp-params:\n"
         "\t<-p port>, port\n");
  printf("Examples (run with default mbServer at port 1502): \n"
         "\t%s --debug -m tcp -c 1 -t 0x04 -r 0 -p 1502 127.0.0.1\n"
         "\t%s --debug -m rtu -a 1 -t 0x06 -r 1 -b 3 -p none /dev/ttyUSB0 1 \n",
         progName, progName);
}

static int select_wDataType(int function_code) {
  switch (function_code) {
  case (MODBUS_FC_READ_COILS):
    wDataType = Data8Array;
    break;
  case (MODBUS_FC_READ_DISCRETE_INPUTS):
    wDataType = DataInt;
    break;
  case (MODBUS_FC_READ_HOLDING_REGISTERS):
  case (MODBUS_FC_READ_INPUT_REGISTERS):
    wDataType = Data16Array;
    break;
  case (MODBUS_FC_WRITE_SINGLE_COIL):
  case (MODBUS_FC_WRITE_SINGLE_REGISTER):
    wDataType = DataInt;
    is_write_function = 1;
    break;
  default:
    printf("ERROR: Function code not correct!");
    return -EINVAL;
  }
  return 0;
}

static int malloc_wDataType(int read_write_no) {
  switch (wDataType) {
  case (DataInt):
    break;
  case (Data8Array):
    data.data8 = malloc(read_write_no * sizeof(uint8_t));
    break;
  case (Data16Array):
    data.data16 = malloc(read_write_no * sizeof(uint16_t));
    break;
  default:
    printf("ERROR: malloc memery error!\n");
    return -ENOMEM;
  }
  return 0;
}

static int get_extra_params(int argc, char **argv, int use_backend) {
  int wDataIdx = 0;
  int inputIntParm = 0;
  int has_device = 0;
  if (1 == debug && 1 == is_write_function)
    printf("write data: ");
  if (optind < argc) {
    while (optind < argc) {
      if (0 == has_device) {
        if (0 != use_backend && RTU == use_backend) {
          device_name = argv[optind];
          has_device = 1;
        } else if (0 != use_backend && TCP == use_backend) {
          ip_address = argv[optind];
          has_device = 1;
        }
      } else {
        switch (wDataType) {
        case (DataInt):
          getInt(argv[optind], &inputIntParm);
          data.dataInt = inputIntParm;
          if (debug)
            printf("0x%x", data.dataInt);
          break;
        case (Data8Array):
          getInt(argv[optind], &inputIntParm);
          data.data8[wDataIdx] = inputIntParm;
          if (debug)
            printf("0x%02x ", data.data8[wDataIdx]);
          break;
        case (Data16Array):
          getInt(argv[optind], &inputIntParm);
          data.data16[wDataIdx] = inputIntParm;
          if (debug)
            printf("0x%04x ", data.data16[wDataIdx]);
          break;
        }
        wDataIdx++;
      }
      optind++;
    }
  }
  return 0;
}

static int modbus_client_processing(modbus_t *ctx, int function_code,
                                    int start_addr, int read_write_no) {
  int ret = -1;
  if (modbus_connect(ctx) == -1) {
    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    modbus_free(ctx);
    return -1;
  } else {
    switch (function_code) {
    case (MODBUS_FC_READ_COILS):
      ret = modbus_read_bits(ctx, start_addr, read_write_no, data.data8);
      break;
    case (MODBUS_FC_READ_DISCRETE_INPUTS):
      printf("MODBUS_FC_READ_DISCRETE_INPUTS 0x02: not implemented yet!\n");
      wDataType = DataInt;
      break;
    case (MODBUS_FC_READ_HOLDING_REGISTERS):
      ret = modbus_read_registers(ctx, start_addr, read_write_no, data.data16);
      break;
    case (MODBUS_FC_READ_INPUT_REGISTERS):
      ret = modbus_read_input_registers(ctx, start_addr, read_write_no,
                                        data.data16);
      break;
    case (MODBUS_FC_WRITE_SINGLE_COIL):
      ret = modbus_write_bit(ctx, start_addr, data.dataInt);
      break;
    case (MODBUS_FC_WRITE_SINGLE_REGISTER):
      ret = modbus_write_register(ctx, start_addr, data.dataInt);
      break;
    default:
      printf("ERROR: incorrect function code input!");
      return -EINVAL;
    }
  }

  if (ret == read_write_no) {
    if (is_write_function)
      printf("Written %d data success!\n", read_write_no);
    else {
      printf("Read %d data success!\n\tData: ", read_write_no);
      int i = 0;
      if (DataInt == wDataType) {
        printf("0x%04x\n", data.dataInt);
      } else {
        const char Format8[] = "0x%02x ";
        const char Format16[] = "0x%04x ";
        const char *format = ((Data8Array == wDataType) ? Format8 : Format16);
        for (; i < read_write_no; ++i) {
          printf(format,
                 (Data8Array == wDataType) ? data.data8[i] : data.data16[i]);
        }
        printf("\n");
      }
    }
  } else {
    modbus_strerror(errno);
  }
  return 0;
}

static int get_input_params(int argc, char **argv) {
  while (1) {
    int option_index = 0;
    static struct option long_options[] = {{DebugOpt, no_argument, 0, 0},
                                           {0, 0, 0, 0}};

    opt = getopt_long(argc, argv, "a:b:d:c:m:r:s:t:p:o:0", long_options,
                      &option_index);
    if (opt == -1) {
      break;
    }

    switch (opt) {
    case 0:
      if (0 == strcmp(long_options[option_index].name, DebugOpt)) {
        debug = 1;
      }
      break;

    case 'a':
      if (!getInt(optarg, &slave_addr)) {
        printf("ERROR: slave address (%s) is false, please input integer!\n\n",
               optarg);
        printUsage(argv[0]);
        return -EINVAL;
      }
      break;
    case 'c':
      if (!getInt(optarg, &read_write_no)) {
        printf("ERROR: elements to read/write (%s) is false, please input "
               "integer!\n\n",
               optarg);
        printUsage(argv[0]);
        return -EINVAL;
      }
      break;

    case 'm':
      if (0 == strcmp(optarg, TcpOptVal)) {
        is_tcp = 1;
        use_backend = TCP;
      } else if (0 == strcmp(optarg, RtuOptVal)) {
        is_tcp = 0;
        use_backend = RTU;
      } else {
        printf("ERROR: %s unrecognized connection type \n\n", optarg);
        printUsage(argv[0]);
        return -EINVAL;
      }
      break;

    case 'r':
      if (!getInt(optarg, &start_addr)) {
        printf("ERROR: start address (%s) is false, please input integer!\n\n",
               optarg);
        printUsage(argv[0]);
        return -EINVAL;
      }
      break;

    case 't':
      if (!getInt(optarg, &function_code)) {
        printf("ERROR: function code (%s) is false, please input integer!\n\n",
               optarg);
        printUsage(argv[0]);
        return -EINVAL;
      }
      break;

    case 'o':
      if (!getInt(optarg, &timeout_ms)) {
        printf("ERROR: timeout (%s) is false, please input integer!\n\n",
               optarg);
        printUsage(argv[0]);
        return -EINVAL;
      }
      printf("Timeout is %d ms\r\n", timeout_ms);
      break;

    case '0':
      start_reference_at0 = 1;
      break;
    case 'p':
      opt_port = strdup(optarg);
      break;
    case 'b':
      opt_baud = strdup(optarg);
      if (atoi(opt_baud) < 0 || atoi(opt_baud) > 3) {
        printf("ERROR: baud rate index is false, please see Usage!\n");
        printUsage(argv[0]);
        return -EINVAL;
      }
      break;
    case 'd':
      opt_data_bits = strdup(optarg);
      break;
    case 's':
      opt_stop_bits = strdup(optarg);
      break;
    default:
      printUsage(argv[0]);
    }
  }
  return 0;
}

int main(int argc, char **argv) {
  modbus_t *ctx;

  if (argc == 1) {
    printUsage(argv[0]);
    return -EINVAL;
  }

    /* get input params */
    if (get_input_params(argc, argv)) {
      return -EINVAL;
    }

    if (-1 == is_tcp) {
      printf("ERROR: please set connection type (-m {rtu|tcp})!\n");
      printUsage(argv[0]);
      return -EINVAL;
    }

    if (1 == start_reference_at0) {
      start_addr--;
    }

    /* select wDataType type */
    if (select_wDataType(function_code)) {
      return -EINVAL;
    }

    if (is_write_function) {
      int data_no = argc - optind - 1;
      read_write_no = data_no;
    }

    /* require wDataType memory */
    if (malloc_wDataType(read_write_no)) {
      return -ENOMEM;
    }

    /* get input device params */
    get_extra_params(argc, argv, use_backend);

    if (1 == debug && 1 == is_write_function)
      printf("\n");

    if (is_tcp) {
      ctx = modbus_new_tcp(ip_address, atoi(opt_port));
    } else {
      ctx = modbus_new_rtu(device_name, atoi(baud_rate_map[atoi(opt_baud)]),
                           'N', 8, 1);
    }
    modbus_set_debug(ctx, debug);
    modbus_set_slave(ctx, slave_addr);

    /* handle rtu/tcp requests */
    if (modbus_client_processing(ctx, function_code, start_addr,
                                 read_write_no)) {
      return -1;
    }

    /* close connection */
    modbus_close(ctx);
    modbus_free(ctx);
    switch (wDataType) {
    case (DataInt):
      break;
    case (Data8Array):
      free(data.data8);
      break;
    case (Data16Array):
      free(data.data16);
      break;
    }

    free_modbus_params(opt_port, opt_baud, opt_data_bits, opt_stop_bits);

    return 0;
}
