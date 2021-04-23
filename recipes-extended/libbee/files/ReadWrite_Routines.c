/*
 * Copyright 2013 Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 *
 * MikroBUS: bee click board(MRF24J40MA) application demo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <linux/spi/spidev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "registers.h"
#include "Misc_Routines.h"
#include "ReadWrite_Routines.h"

int address_RX_FIFO, address_TX_normal_FIFO;
uint8_t data_RX_FIFO[1 + HEADER_LENGTH + DATA_LENGTH + 2 + 1 + 1], lost_data;

uint8_t ADDRESS_short_1[2], ADDRESS_long_1[8];	/* Source address */
uint8_t ADDRESS_short_2[2], ADDRESS_long_2[8];	/* Destination address */
uint8_t PAN_ID_1[2];				/* Source PAN ID */
uint8_t PAN_ID_2[2];				/* Destination PAN ID */
uint8_t DATA_RX[DATA_LENGTH], DATA_TX[DATA_LENGTH],
	data_TX_normal_FIFO[DATA_LENGTH + HEADER_LENGTH + 2];
uint8_t LQI, RSSI2, SEQ_NUMBER, RX_SIZE;

static const char *device = "/dev/spidev2.0";
static uint32_t mode = 0;	/* MSB */
static uint8_t bits = 8;
static uint32_t speed = 500000;
static int fd;
static uint8_t buf_tx[40] = {0, };
static uint8_t buf_rx[40] = {0, };
volatile bool event_flag = false;

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

	memset(&buf_rx, 0, 40);

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");
}

void spidev_init(struct spi_config *config)
{
	int ret = 0;

	if (config != NULL) {
		device  = config->device;
		mode    = config->mode;
		bits    = config->bits;
		speed   = config->speed;
	}

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

/*
 * Functions for reading and writing registers
 * in short address memory space
 */

/* write data in short address register */
void write_ZIGBEE_short(uint8_t address, uint8_t data_r)
{
	/* calculating addressing mode */
	address = ((address << 1) & 0b01111111) | 0x01;
	buf_tx[0] = address;	/* addressing register */
	buf_tx[1] = data_r;	/* write data in register */
	transfer(fd, 2);
}

/* read data from short address register */
uint8_t read_ZIGBEE_short(uint8_t address)
{
	uint8_t data_r = 0;
	/* calculating addressing mode */
	address = (address << 1) & 0b01111110;
	buf_tx[0] = address;	/* addressing register */
	buf_tx[1] = 0;
	transfer(fd, 2);
	data_r = buf_rx[1];	/* read data from register */

	return data_r;
}

/*
 * Functions for reading and writing registers in long address memory space
 */
/* Write data in long address register */
void write_ZIGBEE_long(uint16_t address, uint8_t data_r)
{
	uint8_t address_high = 0, address_low = 0;

	/* calculating addressing mode */
	address_high = (((uint8_t)(address >> 3)) & 0b01111111) | 0x80;
	address_low  = (((uint8_t)(address << 5)) & 0b11100000) | 0x10;
	buf_tx[0] = address_high;	/* addressing register */
	buf_tx[1] = address_low;	/* addressing register */
	buf_tx[2] = data_r;		/* write data in registerr */
	transfer(fd, 3);
}

/* Read data from long address register */
uint8_t read_ZIGBEE_long(uint16_t address)
{
	uint8_t data_r = 0;
	uint8_t address_high = 0, address_low = 0;

	/*calculating addressing mode */
	address_high = ((uint8_t)(address >> 3) & 0b01111111) | 0x80;
	address_low  = ((uint8_t)(address << 5) & 0b11100000);
	buf_tx[0] = address_high;	/* addressing register */
	buf_tx[1] = address_low;	/* addressing register */
	buf_tx[2] = 0;
	transfer(fd, 3);
	data_r = buf_rx[2];		/* read data from register */

	return data_r;
}

/*
 * Transmit packet
 */
void start_transmit(void)
{
	uint8_t temp = 0;

	temp = read_ZIGBEE_short(TXNCON);
	temp = temp | 0x01;	/* mask for start transmit */
	write_ZIGBEE_short(TXNCON, temp);
}

/*
 * FIFO
 *         ---------------------------------------------------------------------
 * RXFIFO  | Frame Length (m + n + 2) | Header | Data Payload | FCS | LQI | RSSI
 *         ---------------------------------------------------------------------
 *         |          1               |   m    |      n       |  2  |  1  |   1
 *         ---------------------------------------------------------------------
 */
void read_RX_FIFO(uint8_t *data_rx)
{
	uint8_t temp = 0;
	int i = 0;
	/* disable receiving packets off air. */
	temp = read_ZIGBEE_short(BBREG1);
	temp = temp | 0x04;	/* mask for disable receiving packets */
	write_ZIGBEE_short(BBREG1, temp);

	data_RX_FIFO[0] = DATA_LENGTH + HEADER_LENGTH + 2;
	//DATA_LENGTH + HEADER_LENGTH + 2
	for (i = 0; i < 128; i++) {
		if (i <  (1 + data_RX_FIFO[0] + 1 + 1))
			/* reading valid data from RX FIFO */
			data_RX_FIFO[i] = read_ZIGBEE_long(address_RX_FIFO + i);
		if (i >= (1 + data_RX_FIFO[0] + 1 + 1))
			/* reading invalid data from RX FIFO */
			lost_data = read_ZIGBEE_long(address_RX_FIFO + i);
	}

	RX_SIZE = data_RX_FIFO[0];
	/* coping valid data */
	memcpy(data_rx, &data_RX_FIFO[HEADER_LENGTH + 1],
	       data_RX_FIFO[0] - HEADER_LENGTH - 2);
	SEQ_NUMBER = data_RX_FIFO[3];
	/* coping valid data HEADER_LENGTH + DATA_LENGTH + 2*/
	LQI   = data_RX_FIFO[1 + data_RX_FIFO[0]];
	/* coping valid data HEADER_LENGTH + DATA_LENGTH + 3*/
	RSSI2 = data_RX_FIFO[1 + data_RX_FIFO[0] + 1];
	/* enable receiving packets off air. */
	temp = read_ZIGBEE_short(BBREG1);
	/* mask for enable receiving */
	temp = temp & (uint8_t)(~0x04);
	write_ZIGBEE_short(BBREG1, temp);
}

/*          ------------------------------------------------------------------
 * TXFIFO   | Header Length(m) | Frame Length(m + n) | Header | Data Payload |
 *          ------------------------------------------------------------------
 *          |         1        |           1         |    m   |       n      |
 *          ------------------------------------------------------------------
 */
void write_TX_normal_FIFO(char *data, uint8_t length, uint8_t cmd, bool server)
{
	int i = 0;

	data_TX_normal_FIFO[0]  = HEADER_LENGTH;
	data_TX_normal_FIFO[1]  = HEADER_LENGTH + length;//DATA_LENGTH
	data_TX_normal_FIFO[2]  = 0x01;			/* control frame */
	data_TX_normal_FIFO[3]  = 0x88;
	data_TX_normal_FIFO[4]  = cmd;		/* sequence number SEQ_NUMBER*/
	data_TX_normal_FIFO[5]  = PAN_ID_2[1];		/* destinatoin pan */
	data_TX_normal_FIFO[6]  = PAN_ID_2[0];
	if (server) {
		/* destination address */
		data_TX_normal_FIFO[7]  = ADDRESS_short_2[0];
		data_TX_normal_FIFO[8]  = ADDRESS_short_2[1];
		/* source address */
		data_TX_normal_FIFO[11] = ADDRESS_short_1[0];
		data_TX_normal_FIFO[12] = ADDRESS_short_1[1];
	} else {
		/* destination address */
		data_TX_normal_FIFO[7]  = ADDRESS_short_1[0];
		data_TX_normal_FIFO[8]  = ADDRESS_short_1[1];
		/* source address */
		data_TX_normal_FIFO[11] = ADDRESS_short_2[0];
		data_TX_normal_FIFO[12] = ADDRESS_short_2[1];
	}
	data_TX_normal_FIFO[9]  = PAN_ID_1[0];		/* source pan */
	data_TX_normal_FIFO[10] = PAN_ID_1[1];
	//data_TX_normal_FIFO[13] = DATA_TX[0];		/* data */
	memcpy(&data_TX_normal_FIFO[13], data, length);

	//DATA_LENGTH
	for (i = 0; i < (HEADER_LENGTH + length + 2); i++) {
		/* write frame into normal FIFO */
		write_ZIGBEE_long(address_TX_normal_FIFO + i,
				data_TX_normal_FIFO[i]);
	}

	set_not_ACK();
	set_not_encrypt();
	start_transmit();
}
