/*
 * Copyright 2013 Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 *
 * MikroBUS: blep click board(nRF8001) application demo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <pthread.h>
#include <signal.h>
#include "services.h"
#include "acilib.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define IS_DEVADDR_GETTING(buf)	(memcmp((void*) buf, (void*) "devaddr", 7) == 0)
#define IS_VERSION_GETTING(buf) (memcmp((void*) buf, (void*) "version", 7) == 0)
#define IS_NAME_SETTING(buf)	(memcmp((void*) buf, (void*) "name=", 5) == 0)
#define IS_ECHO_DATA(buf)	(memcmp((void*) buf, (void*) "echo ", 5) == 0)

void event_get_addr_handle(struct aci_packet *packet_rx)
{
	printf("Device address");
	for (uint8_t tmp = 0; tmp < 6; tmp++)
		printf("\033[0;31m:%02X\033[0;39m",
			packet_rx->params.cmd_rsp.content.
			get_device_address.addr[5 - tmp]);
	printf("\n");

}

void event_get_version_handle(struct aci_packet *packet_rx)
{
	struct getversion_response get_device_version;

	memcpy(&get_device_version,
		&packet_rx->params.cmd_rsp.content.response_data[0], 9);
	printf("Device version\n");
	printf("\033[0;31m Configuration ID:0x%02X\033[0;39m\n",
			get_device_version.configuration_id);
	printf("\033[0;31m ACI protocol version:%d\033[0;39m\n",
			get_device_version.aci_version);
	printf("\033[0;31m Current setup format:%d\033[0;39m\n",
			get_device_version.setup_format);
	printf("\033[0;31m Setup ID:0x%02X\033[0;39m\n",
			get_device_version.setup_id);
	printf("\033[0;31m Configuration status:%s\033[0;39m\n",
		get_device_version.setup_status ? "locked(NVM)" : "open(VM)");
}

void event_timing_handle(struct aci_packet *packet_rx)
{
	struct evt_params_timing timing;

	memcpy(&timing, &packet_rx->params.payload[0], 6);
	printf(" Evt link connection interval changed \n");
	printf("\033[0;31m ConnectionInterval:0x%04X\033[0;39m\n",
				timing.conn_rf_interval);
	printf("\033[0;31m SlaveLatency:0x%04X\033[0;39m\n",
				timing.conn_slave_rf_latency);
	printf("\033[0;31m SupervisionTimeout:0x%04X\033[0;39m\n",
				timing.conn_rf_timeout);
}

void event_data_receive_handle(struct aci_packet *packet_rx)
{
	printf("DataReceivedEvent:\033[0;31m %s\033[0;39m\n",
			&packet_rx->params.payload[1]);
}

void set_hook_func(void)
{
	event_get_addr_hook	= event_get_addr_handle;
	event_get_version_hook	= event_get_version_handle;
	event_timing_hook	= event_timing_handle;
	event_data_receive_hook	= event_data_receive_handle;
}

pthread_t console_thread, event_thread;
void app_exit(int signo)
{
	close_spidev();
	pthread_cancel(console_thread);
	pthread_cancel(event_thread);
}

void *console_handle(void *argv)
{
	char str[50];
	uint8_t str_len = 0;
	uint8_t i = 0;
	
	while(1) {
		memset(str, 0, 50);
		printf("Please input a command!\n");
		fgets(str, 50, stdin);

		if (IS_DEVADDR_GETTING(str)) {
			aci_send_cmd(ACI_CMD_GET_DEVICE_ADDRESS, NULL, 0);
		} else if (IS_NAME_SETTING(str)) {
			/* remove the enter character */
			str_len = strlen(&str[5]) - 1;
			str_len = str_len > PIPE_GAP_DEVICE_NAME_SET_MAX_SIZE ?
					PIPE_GAP_DEVICE_NAME_SET_MAX_SIZE :
					str_len;
			aci_send_cmd(ACI_CMD_SET_LOCAL_DATA, &str[5], str_len);
		} else if (IS_ECHO_DATA(str)) {
			str_len  = strlen(&str[5]) - 1;
			str_len = PIPE_UART_OVER_BTLE_UART_TX_TX_MAX_SIZE >
					str_len ? str_len :
					PIPE_UART_OVER_BTLE_UART_TX_TX_MAX_SIZE;
			aci_send_cmd(ACI_CMD_SEND_DATA, &str[5], str_len);
		} else if (IS_VERSION_GETTING(str)) {
			aci_send_cmd(ACI_CMD_GET_DEVICE_VERSION, NULL, 0);
		}
	}
	pthread_exit(0);
}


void *event_handle(void *argv)
{
	for (;;) {
		aci_event_handle_loop();
		usleep(10000);
	}
	pthread_exit(0);
}

int main(void)
{
	/* Default: mode 0, speed 500k, bit 8 */
	struct spi_config config = {
		.mode = 0,
		.speed = 500000,
		.bits = 8,
		.device = "/dev/spidev2.0"
	};

	/* Broadcast parameter setting */
	/* Default: no timeout, advertising interval is 50ms */
	adv_params.timeout = 0;
	adv_params.adv_interval = 0x0050;

	set_hook_func();
	spidev_init(&config);
	
	signal(SIGINT, app_exit);
	pthread_create(&console_thread, NULL, console_handle, NULL);
	pthread_create(&event_thread, NULL, event_handle, NULL);

	pthread_join(console_thread, NULL);
	pthread_join(event_thread, NULL);
	printf("Application exit!\n");
	return 0;
}
