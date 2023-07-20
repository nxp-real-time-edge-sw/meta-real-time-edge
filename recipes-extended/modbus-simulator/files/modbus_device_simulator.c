// SPDX-License-Identifier: (GPL-2.0 OR MIT)
/*
 * Copyright 2022 NXP
 */

#include <arpa/inet.h>
#include <errno.h>
#include <getopt.h>
#include <modbus.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common.h"

#define CPU_TEMP_FILE0 "/sys/devices/virtual/thermal/thermal_zone0/temp"
#define IMX8MM_LED_CONTROL "/sys/class/leds/status/brightness"
#define IMX8MP_LED_CONTROL "/sys/class/leds/yellow:status/brightness"

/* Modbus registers start address */
#define READ_CPU_TEMPERATURE_REGISTERS_START_ADDRESS 0
#define DEVICE_REGISTERS_START_ADDRESS 0
#define BAUD_REGISTERS_START_ADDRESS 1
#define LED_REGISTERS_START_ADDRESS 0

#define NB_CONNECTION 10
#define BAUD_RATE_MAP_LENGTH 32

#define CPU_TEMPERATURE 23000

static modbus_t *ctx = NULL;
static modbus_mapping_t *mb_mapping;
static int opt;
static int ret = 0;
static int is_tcp = -1;
static int use_backend;
static int debug = 0;
static int slave_address = 1;
static int server_socket = -1;
static int led_status = 1;
static char *opt_port = NULL;
static char *opt_baud = NULL;
static char *opt_data_bits = NULL;
static char *opt_stop_bits = NULL;

enum { TCP, RTU };

static void close_sigint(int dummy) {
  if (server_socket != -1) {
    close(server_socket);
  }
  modbus_free(ctx);
  modbus_mapping_free(mb_mapping);

  exit(dummy);
}

void printUsage(const char progName[]) {
  printf("%s [--%s] <-m {tcp|rtu}> [-a <slave-addr>] {rtu-params|tcp-params} "
         "{serialport|host}\n",
         progName, DebugOpt);
  printf("rtu-params:\n"
         "\t<-b {4800|9600|19200|115200}>, baud-rate\n"
         "\t[-d {7|8}], data-bits\n"
         "\t[-s {1|2}], stop-bits\n"
         "\t[-p {none|even|odd}], verify\n");
  printf("tcp-params:\n"
         "\t<-p port>, port\n");
  printf("Examples (run with default mbServer at port 1502): \n"
         "\t%s --debug -m tcp -p 1502 0.0.0.0    \n"
         "\t%s --debug -m rtu -a 1 -b 115200 -p none /dev/ttyUSB1    \n",
         progName, progName);
}

static int get_input_params(int argc, char **argv) {
  while (1) {
    int option_index = 0;
    static struct option long_options[] = {{DebugOpt, no_argument, 0, 0},
                                           {0, 0, 0, 0}};

    opt = getopt_long(argc, argv, "a:b:d:m:s:p:", long_options, &option_index);
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
      if (!getInt(optarg, &slave_address)) {
        printf("ERROR: slave address (%s) is false, please input integer!\n\n",
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
        printf("ERROR: %s Unrecognized connection type \n\n", optarg);
        printUsage(argv[0]);
        return -EINVAL;
      }
      break;

    // tcp/rtu params
    case 'p':
      opt_port = strdup(optarg);
      break;
    case 'b':
      opt_baud = strdup(optarg);
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

static int get_cpu_temperature() {
  FILE *fp = NULL;
  // set a dummy value of temperature
  int temp = CPU_TEMPERATURE;

  if (access(CPU_TEMP_FILE0, F_OK) == 0) {
    fp = fopen(CPU_TEMP_FILE0, "r");
    fscanf(fp, "%d", &temp);
    fclose(fp);
  } 
  return temp / 10;
}

static int get_led_status() {
  FILE *fp = NULL;
  int status = led_status;
  char *ledPath = NULL;
  int board_read_led = 1;
  if (access(IMX8MM_LED_CONTROL, F_OK) == 0) {
    fp = fopen(IMX8MM_LED_CONTROL, "r");
    ledPath = IMX8MM_LED_CONTROL;
  } else if (access(IMX8MP_LED_CONTROL, F_OK) == 0) {
    fp = fopen(IMX8MP_LED_CONTROL, "r");
    ledPath = IMX8MP_LED_CONTROL;
  } else {
    board_read_led = 0;
  }
  if (board_read_led) {
    if (!fp) {
      printf("ERROR: read led status %s %s\n", ledPath, strerror(errno));
      return -EIO;
    }
    fscanf(fp, "%d", &status);
    fclose(fp);
  }
  return status & 0xF;
}

static int set_led_status(int status) {
  FILE *fp = NULL;
  char sw[][2] = {"0", "1"};
  char *ledPath = NULL;
  int board_write_led = 1;

  if (access(IMX8MM_LED_CONTROL, F_OK) == 0) {
    fp = fopen(IMX8MM_LED_CONTROL, "w");
    ledPath = IMX8MM_LED_CONTROL;
  } else if (access(IMX8MP_LED_CONTROL, F_OK) == 0) {
    fp = fopen(IMX8MP_LED_CONTROL, "w");
    ledPath = IMX8MP_LED_CONTROL;
  } else {
    board_write_led = 0;
  }

  if (board_write_led) {
    if (!fp) {
      printf("ERROR: write led status %s %s\n", ledPath, strerror(errno));
      return -EIO;
    }
    fwrite(sw[status], strlen(sw[status]), 1, fp);
    fclose(fp);
  }
  led_status = atoi(sw[status]);
  return 0;
}

static int modbus_device_processing(modbus_t *ctx, uint8_t *query,
                                    modbus_mapping_t *mb_mapping, int rc) {
  int ret = 0;
  int reg_adds = 0, baudRateIndex = 0;
  int led_status, cpu_temperature;
  int header_length;
  /* Special server behavior to test client */
  header_length = modbus_get_header_length(ctx);
  /* read coils */
  if (query[header_length] == MODBUS_FC_READ_COILS) {
    reg_adds = MODBUS_GET_INT16_FROM_INT8(query, header_length + 1);
    switch (reg_adds) {
    case (LED_REGISTERS_START_ADDRESS):
      if ((led_status = get_led_status()) < 0) {
        ret = -1;
        modbus_reply_exception(ctx, query, MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE);
      } else {
        modbus_set_bits_from_byte(mb_mapping->tab_bits,
                                  LED_REGISTERS_START_ADDRESS, led_status);
        ret = 0;
        modbus_reply(ctx, query, rc, mb_mapping);
      }
      break;
    default:
      ret = -1;
      modbus_reply_exception(ctx, query, MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
    }
  }
  /* write singel coil */
  else if (query[header_length] == MODBUS_FC_WRITE_SINGLE_COIL) {
    reg_adds = MODBUS_GET_INT16_FROM_INT8(query, header_length + 1);
    switch (reg_adds) {
    case (LED_REGISTERS_START_ADDRESS):
      if (set_led_status(MODBUS_GET_INT16_FROM_INT8(query, header_length + 3) ==
                                 0xFF00
                             ? 0x01
                             : 0x00) < 0) {
        ret = -1;
        modbus_reply_exception(ctx, query, MODBUS_EXCEPTION_ILLEGAL_DATA_VALUE);
      }
      ret = 0;
      modbus_reply(ctx, query, rc, mb_mapping);
      break;
    default:
      ret = -1;
      modbus_reply_exception(ctx, query, MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
    }
  }
  /* read input registers */
  else if (query[header_length] == MODBUS_FC_READ_INPUT_REGISTERS) {
    reg_adds = MODBUS_GET_INT16_FROM_INT8(query, header_length + 1);
    switch (reg_adds) {
    case (READ_CPU_TEMPERATURE_REGISTERS_START_ADDRESS):
      /* read cpu temperature*/
      cpu_temperature = get_cpu_temperature();
      mb_mapping->tab_input_registers[0] = cpu_temperature & 0xFFFF;
      ret = 0;
      modbus_reply(ctx, query, rc, mb_mapping);
      break;
    default:
      ret = -1;
      modbus_reply_exception(ctx, query, MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
    }
  }
  /* write single register */
  else if (query[header_length] == MODBUS_FC_WRITE_SINGLE_REGISTER) {
    reg_adds = MODBUS_GET_INT16_FROM_INT8(query, header_length + 1);
    switch (reg_adds) {
    case (DEVICE_REGISTERS_START_ADDRESS):
      slave_address =
          MODBUS_GET_INT16_FROM_INT8(query, header_length + 3) & 0xFF;
      ret = 1;
      modbus_reply(ctx, query, rc, mb_mapping);
      break;
    case (BAUD_REGISTERS_START_ADDRESS):
      baudRateIndex = MODBUS_GET_INT16_FROM_INT8(query, header_length + 3);
      if (baudRateIndex >= BAUD_RATE_MAP_LENGTH / sizeof(char *) ||
          baudRateIndex < 0) {
        ret = -1;
        modbus_reply_exception(ctx, query,
                               MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
        break;
      }
      opt_baud = baud_rate_map[baudRateIndex];
      modbus_reply(ctx, query, rc, mb_mapping);
      ret = 1;
      break;
    default:
      ret = -1;
      modbus_reply_exception(ctx, query, MODBUS_EXCEPTION_ILLEGAL_DATA_ADDRESS);
      break;
    }
  }
  return ret;
}

static int rtu_run() {
  int rc = 0;
  int ret = 0;
  rc = modbus_connect(ctx);
  if (rc == -1) {
    fprintf(stderr, "Unable to connect %s\n", modbus_strerror(errno));
    modbus_free(ctx);
    return -1;
  }
  for (;;) {
    uint8_t query[MODBUS_RTU_MAX_ADU_LENGTH];
    rc = modbus_receive(ctx, query);
    if (rc > 0) {
      ret = modbus_device_processing(ctx, query, mb_mapping, rc);
      if (ret) {
        /*reset modbus server due to modify baud rate or slave address*/
        break;
      }
    } else if (rc == -1) {
      /* Connection closed by the client or error */
      printf("ERROR: Client disconnected: %s\n", modbus_strerror(errno));
      break;
    }
  }
  return 0;
}

static int tcp_run() {
  int rc = 0;
  int ret = 0;
  uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
  int master_socket;
  fd_set refset;
  fd_set rdset;
  /* Maximum file descriptor number */
  int fdmax;

  server_socket = modbus_tcp_listen(ctx, NB_CONNECTION);
  if (server_socket == -1) {
    fprintf(stderr, "Unable to listen TCP connection\n");
    modbus_free(ctx);
    return -1;
  }

  signal(SIGINT, close_sigint);

  /* Clear the reference set of socket */
  FD_ZERO(&refset);
  /* Add the server socket */
  FD_SET(server_socket, &refset);

  /* Keep track of the max file descriptor */
  fdmax = server_socket;

  for (;;) {
    rdset = refset;
    if (select(fdmax + 1, &rdset, NULL, NULL, NULL) == -1) {
      perror("Server select() failure.");
      close_sigint(1);
    }
    /* Run through the existing connections looking for data to be
     * read */
    for (master_socket = 0; master_socket <= fdmax; master_socket++) {
      if (!FD_ISSET(master_socket, &rdset)) {
        continue;
      }

      if (master_socket == server_socket) {
        /* A client is asking a new connection */
        socklen_t addrlen;
        struct sockaddr_in clientaddr;
        int newfd;

        /* Handle new connections */
        addrlen = sizeof(clientaddr);
        memset(&clientaddr, 0, sizeof(clientaddr));
        newfd = accept(server_socket, (struct sockaddr *)&clientaddr, &addrlen);
        if (newfd == -1) {
          perror("Server accept() error");
        } else {
          FD_SET(newfd, &refset);

          if (newfd > fdmax) {
            /* Keep track of the maximum */
            fdmax = newfd;
          }
          printf("New connection from %s:%d on socket %d\n",
                 inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port, newfd);
        }
      } else {
        modbus_set_socket(ctx, master_socket);
        rc = modbus_receive(ctx, query);
        if (rc > 0) {
          ret = modbus_device_processing(ctx, query, mb_mapping, rc);
        } else if (rc == -1) {
          /* This example server in ended on connection closing or
           * any errors. */
          printf("Connection closed on socket %d\n", master_socket);
          close(master_socket);

          /* Remove from reference set */
          FD_CLR(master_socket, &refset);

          if (master_socket == fdmax) {
            fdmax--;
          }
        }
      }
    }
  }
  return 0;
}

int main(int argc, char **argv) {
  int diNo = 10;
  int coilsNo = 10;
  int irNo = 10;
  int hrNo = 10;

  char *device_name = NULL;
  char *ip_address = "127.0.0.1";

  if (argc == 1) {
    printUsage(argv[0]);
    return -EINVAL;
  }
  /* get input params */
  if (get_input_params(argc, argv)) {
    return -EINVAL;
  }

  if (1 == argc - optind) {
    if (is_tcp) {
      ip_address = argv[optind];
    } else {
      device_name = argv[optind];
    }
  }

    if (-1 == is_tcp) {
      printf("ERROR: Please set connection type (-m {rtu|tcp})!\n");
      printUsage(argv[0]);
      return -EINVAL;
    }

    while (1) {
      ret = 0;
      if (is_tcp) {
        ctx = modbus_new_tcp(ip_address, atoi(opt_port));
      } else {
        ctx = modbus_new_rtu(device_name, atoi(opt_baud), 'N', 8, 1);
      }
      // prepare mapping
      mb_mapping =
          modbus_mapping_new(coilsNo = 1, diNo = 1, hrNo = 2, irNo = 1);
      if (mb_mapping == NULL) {
        fprintf(stderr, "Failed to allocate the mapping: %s\n",
                modbus_strerror(errno));
        modbus_free(ctx);
        return -ENOMEM;
      }

      if (debug) {
        printf("Debug: \n \tCoils: 0x%04x\n\tDigital inputs: 0x%04x\n\tHolding "
               "registers: 0x%04x\n\tInput registers: 0x%04x\n",
               coilsNo, diNo, hrNo, irNo);
        if (is_tcp) {
          printf("\tIp address: %s\n\tPort: %s\n\t", ip_address, opt_port);
        } else {
          printf("\tSlave address: %d\n\tDevice name: %s\n\tBaud rate: %s\n\t",
                 slave_address, device_name, opt_baud);
        }
      }

      modbus_set_debug(ctx, debug);
      modbus_set_slave(ctx, slave_address);

      if (RTU == use_backend) {
        /* handle rtu requests */
        rtu_run();
      } else if (TCP == use_backend) {
        /* handle tcp requests */
        tcp_run();
      }
      printf("--------------------Restart--------------------\n");
      /* Close TCP connection */
      if (use_backend == TCP) {
        if (server_socket != -1) {
          close(server_socket);
        }
      }
      modbus_mapping_free(mb_mapping);
      free_modbus_params(opt_port, opt_baud, opt_data_bits, opt_stop_bits);
      /* Close connection */
      sleep(1);
      modbus_close(ctx);
      modbus_free(ctx);
    }
    return 0;
}
