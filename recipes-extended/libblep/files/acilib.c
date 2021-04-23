/*
 * Copyright 2013 Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 *
 * MikroBUS: blep click board(nRF8001) aci library
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
#include <signal.h>
#include "services.h"
#include "acilib.h"

static const char *device = "/dev/spidev2.0";
static uint32_t mode = 0;
static uint8_t bits = 8;
static uint32_t speed = 500000;
static int fd;
static uint8_t buf_tx[40] = {0, };
static uint8_t buf_rx[40] = {0, };
struct cmd_params_connect adv_params = {
/* 0x0000 (no timeout) to 0x3FFF */
	.timeout = 0,
/* 16 bits of advertising interval for general discovery */
	.adv_interval = 0x0050
};
volatile bool event_flag = false;
const struct setup_data setup_msgs[NB_SETUP_MESSAGES] = SETUP_MESSAGES_CONTENT;

hook_func event_get_addr_hook = NULL;
hook_func event_get_version_hook = NULL;
hook_func event_timing_hook = NULL;
hook_func event_data_receive_hook = NULL;

static void pabort(const char *s)
{
	perror(s);
	abort();
}

static uint8_t byte_M2L(uint8_t byte)
{
	uint8_t res = 0;

	for (uint8_t i = 0; i < 8; i++) {
		if (byte & (0x01 << i))
			res |= 0x80 >> i;
	}

	return res;
}

static void set_rstpin (uint8_t val)
{
	char cmd[50];

	val = (val > 0) ? 0x03 : 0x02;
	sprintf(cmd, "i2cset -y 0 0x66 0x55 0x%02X b", val);
	system(cmd);
}

static bool get_rdypin(void)
{
	FILE *fp = NULL;
	char data[10] = {'0'};
	int res;

	fp = popen("i2cget -y 0 0x66 0x55 b", "r");
	if (fp == NULL) {
		printf("popen error!\n");
		return false;
	}

	if (fgets(data, sizeof(data), fp) != NULL)
		res = strtol(data, NULL, 0);

	pclose(fp);

	if (res & 0x40)
		return false;
	else
		return true;
}

static void transfer(int fd, uint8_t len)
{
	int ret;
	uint8_t i;

	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)buf_tx,
		.rx_buf = (unsigned long)buf_rx,
		.len = len,
		//.delay_usecs = 1000,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	for (i = 0; i < len; i++)
		buf_tx[i] = byte_M2L(buf_tx[i]);

	memset(&buf_rx, 0, 40);

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");
	/* buf_rx[1] is effective data length */
	for (i = 1; i < len; i++)
		if (i <= (buf_rx[1] + 1))
			buf_rx[i] = byte_M2L(buf_rx[i]);
		else
			buf_rx[i] = 0;
}

void spidev_init(struct spi_config *config)
{
	int ret = 0;

	if (config != NULL) {
		device	= config->device;
		mode 	= config->mode;
		bits	= config->bits;
		speed	= config->speed;
	}

	set_rstpin(0);
	usleep(100);
	set_rstpin(1);

	fd = open(device, O_RDWR);
	if (fd < 0)
		pabort("can't open device");

	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE32, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE32, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	event_flag = true;
	printf("spi mode: 0x%x\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
}

void aci_send_cmd(uint8_t cmd, void *param, uint8_t len)
{
	memset(buf_tx, 0, 40);
	buf_tx[1] = cmd;

	switch (cmd) {
	case ACI_CMD_GET_DEVICE_ADDRESS:
		buf_tx[0] = 0x01;
		while (get_rdypin());
		break;

	case ACI_CMD_GET_DEVICE_VERSION:
		buf_tx[0] = 0x01;
		while (get_rdypin());
		break;

	case ACI_CMD_SET_LOCAL_DATA:
		buf_tx[0] = 2 + len;
		buf_tx[2] = PIPE_GAP_DEVICE_NAME_SET;
		memcpy(&buf_tx[3], param, len);
		printf("Name set.\033[0;31m New name: %s\033[0;39m, %d\n",
			&buf_tx[3], len);
		while (get_rdypin());
		break;

	case ACI_CMD_SEND_DATA:
		buf_tx[0] = 2 + len;
		buf_tx[2] = PIPE_UART_OVER_BTLE_UART_TX_TX;
		memcpy(&buf_tx[3], param, len);
		break;

	case ACI_CMD_CONNECT:
		buf_tx[0] = 0x05;
		memcpy(&buf_tx[2], param, 4);
		break;
	}
	transfer(fd, buf_tx[0] + 1);
	event_flag = true;
}

static uint8_t aci_setup_loop(uint8_t *cmd_status, uint8_t *setup_offset)
{
	usleep(500);
	switch (*cmd_status) {
	case ACI_STATUS_SUCCESS:
	case ACI_STATUS_TRANSACTION_CONTINUE:
		(*setup_offset)++;
		if (*setup_offset < NB_SETUP_MESSAGES) {
			memcpy(buf_tx, &setup_msgs[*setup_offset].buffer[0],
				setup_msgs[*setup_offset].buffer[0] + 1);
			transfer(fd, setup_msgs[*setup_offset].buffer[0] +1);
			printf(".");
		} else
			*cmd_status = ACI_STATUS_TRANSACTION_COMPLETE;
		break;

	case ACI_STATUS_TRANSACTION_COMPLETE:
		/*
		 * Break out of the while loop
		 * when this status code appears
		 */
		printf("Setup complete\n");
		break;

	default:
		/*
		 * An event with any other status code
		 * should be handled by the application
		 */
		printf("Setup status error: 0x%02X; setup_offset = %d\n",
			*cmd_status, *setup_offset);
		return 1;
	}

	return 0;
}

uint8_t do_aci_setup(void)
{
	struct aci_packet setup_event;
	uint8_t setup_offset = 0;
	uint32_t i = 0;
	uint8_t cmd_status = ACI_STATUS_TRANSACTION_CONTINUE;
	bool wait_event = false;

	memcpy(buf_tx, &setup_msgs[setup_offset].buffer[0],
		setup_msgs[setup_offset].buffer[0] + 1);
	transfer(fd, setup_msgs[setup_offset].buffer[0] + 1);
	wait_event = true;

	while (cmd_status != ACI_STATUS_TRANSACTION_COMPLETE) {
	/*
	 * This counter is used to ensure that this function does not loop
	 * forever. When the device returns a valid response, we reset the
	 * counter.
	 */
		if (i++ > 0xFFFFFE) {
			printf("Setup timeout\n");
			return SETUP_FAIL_TIMEOUT;
		}

		if (wait_event && get_rdypin()) {
			wait_event = false;
			transfer(fd, HAL_ACI_MAX_LENGTH);
			memcpy(&setup_event, buf_rx, HAL_ACI_MAX_LENGTH);

			/*
			 * Receiving something other than a Command Response
			 * Event is an error.
			 */
			if (setup_event.opcode != ACI_EVT_CMD_RSP) {
				printf("Setup event is not cmdresponse\n");
				return SETUP_FAIL_NOT_COMMAND_RESPONSE;
			}

			cmd_status = setup_event.params.cmd_rsp.cmd_status;

			if(aci_setup_loop(&cmd_status, &setup_offset))
				return SETUP_FAIL_NOT_SETUP_EVENT;

			/* As the device is responding, reset guard counter */
			i = 0;
			wait_event = true;
		}
	}
	return SETUP_SUCCESS;
}

static void aci_evt_cmd_rsp_handle(struct aci_packet *packet_rx)
{
	switch (packet_rx->params.cmd_rsp.cmd_opcode) {
	case ACI_CMD_GET_DEVICE_ADDRESS:
		if (packet_rx->params.cmd_rsp.cmd_status == ACI_STATUS_SUCCESS)
			if (event_get_addr_hook != NULL)
				event_get_addr_hook(packet_rx);
		else
			printf("Get device address faild!\n");
		break;

	case ACI_CMD_SETUP:
		if (packet_rx->params.cmd_rsp.cmd_status ==
			ACI_STATUS_TRANSACTION_COMPLETE)
			printf("\nSetup complete\n");
		break;

	case ACI_CMD_CONNECT:
		if (packet_rx->params.cmd_rsp.cmd_status == ACI_STATUS_SUCCESS)
			printf("Send broadcast command successfully\n");
		break;

	case ACI_CMD_SET_LOCAL_DATA:
		if (packet_rx->params.cmd_rsp.cmd_status == ACI_STATUS_SUCCESS)
			printf("Set local data successfully\n");
		break;

	case ACI_CMD_GET_DEVICE_VERSION:
		if (packet_rx->params.cmd_rsp.cmd_status == ACI_STATUS_SUCCESS)
			if (event_get_version_hook != NULL)
				event_get_version_hook(packet_rx);
		break;

	case ACI_CMD_CHANGE_TIMING:
		if (packet_rx->params.cmd_rsp.cmd_status != ACI_STATUS_SUCCESS)
			printf("ChangeTimingRequest is error: 0x%02X\n",
					packet_rx->params.cmd_rsp.cmd_status);
			break;

	default:
		break;
	}
}

void aci_event_handle_loop(void)
{
	struct aci_packet packet_rx;
	bool setup_required = false;
	struct evt_params_pipe_error pipe_error;
	bool timing_change_done = false;
	uint8_t tmp = 0;

	if (event_flag && get_rdypin()) {
		event_flag = false;
		memset(&packet_rx, 0, REV_PKG_SIZE);
		memset(buf_tx, 0, 40);
		transfer(fd, HAL_ACI_MAX_LENGTH);
		memcpy(&packet_rx, buf_rx, HAL_ACI_MAX_LENGTH);

		usleep(100);
		switch (packet_rx.opcode) {
		case ACI_EVT_DEVICE_STARTED:
			printf("\033[0;33m Event device started: \033[0;39m");

			switch (packet_rx.params.device_started.device_mode) {
			case ACI_DEVICE_TEST:
				printf("\033[0;33mTest\033[0;39m");
				break;

			case ACI_DEVICE_SETUP:
				setup_required = true;
				printf("\033[0;33mSetup\033[0;39m");
				break;

			case ACI_DEVICE_STANDBY:
				aci_send_cmd(ACI_CMD_CONNECT, &adv_params, 4);
				printf("\033[0;33mStandby\033[0;39m\n");
				printf(" Advertising started :");
				printf(" Tap Connect on the nRF UART app ");
				break;
			}
			printf("\n");
			printf("Error:%s\n",
				packet_rx.params.device_started.hw_error ?
				"fatal" : "no");
			break;

		case ACI_EVT_CMD_RSP:
			aci_evt_cmd_rsp_handle(&packet_rx);
			break;

		case ACI_EVT_CONNECTED:
			timing_change_done = false;
			printf("\033[0;33m Evt Connected\033[0;39m\n");
			break;

		case ACI_EVT_DISCONNECTED:
			printf("\033[0;33m Evt Disconnected\033[0;39m\n");
			aci_send_cmd(ACI_CMD_CONNECT, &adv_params, 4);
			printf(" Advertising started :");
			printf(" Tap Connect on the nRF UART app ");
			break;

		case ACI_EVT_PIPE_STATUS:
			printf(" Evt Pipe Status\n");
			if (timing_change_done == false)
				timing_change_done = true;
			break;

		case ACI_EVT_TIMING:
			if (event_timing_hook != NULL)
				event_timing_hook(&packet_rx);
			break;

		case ACI_EVT_DATA_RECEIVED:
			if ((packet_rx.params.payload[0] ==
					PIPE_UART_OVER_BTLE_UART_RX_RX) &&
					(event_data_receive_hook != NULL))
					event_data_receive_hook(&packet_rx);
			break;
 
		case ACI_EVT_DATA_CREDIT:
			printf("\033[0;31mThe number of data\033[0;39m");
			printf("\033[0;31m command buffer is %d\033[0;39m\n",
					packet_rx.params.payload[0]);
			break;

		case ACI_EVT_PIPE_ERROR:
			memcpy(&pipe_error, &packet_rx.params.payload[0], 29);
			printf("\033[0;31m ACI Evt Pipe Error:\033[0;39m");
			printf("\033[0;31m Pipe #%d\033[0;39m\n",
					pipe_error.pipe_number);
			printf("\033[0;31m Pipe Error Code:\033[0;39m");
			printf("\033[0;31m 0x%02X\033[0;39m\n",
					pipe_error.error_code);
			printf("\033[0;31m Pipe Error Data:\033[0;39m");
			for (tmp = 0; tmp < (packet_rx.length - 3); tmp++)
				printf(" 0x%02X", pipe_error.error_data[tmp]);
			printf("\n");
			if (pipe_error.error_code ==
					ACI_STATUS_ERROR_DEVICE_STATE_INVALID)
				printf(" Please connect the device");
				printf(" before sending data\n");
			break;

		default:
			printf("Unknow event:0x%02X\n", packet_rx.opcode);
			break;
		}
		event_flag = true;
	}

	if (setup_required) {
		printf("Start setup command\n");
		if (SETUP_SUCCESS == do_aci_setup()) {
			event_flag = true;
			setup_required = false;
		} else {
			printf("Failed to initialize blep\n");
		}
	}
}

void close_spidev(void)
{
	close(fd);
}
