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

#ifndef __READWRITE_ROUTINES_H__
#define __READWRITE_ROUTINES_H__

#include <stdint.h>
#include <stdbool.h>

#define DATA_LENGTH	100
#define HEADER_LENGTH	11

struct spi_config {
	uint32_t mode;
	uint32_t speed;
	char *device;
	uint8_t bits;
};

extern uint8_t DATA_RX[DATA_LENGTH], DATA_TX[DATA_LENGTH];
extern uint8_t LQI, RSSI2, SEQ_NUMBER, RX_SIZE;

void spidev_init(struct spi_config *config);
uint8_t read_ZIGBEE_long(uint16_t address);
void write_ZIGBEE_long(uint16_t address, uint8_t data_r);
uint8_t read_ZIGBEE_short(uint8_t address);
void write_ZIGBEE_short(uint8_t address, uint8_t data_r);
void read_RX_FIFO(uint8_t *data_rx);
void start_transmit(void);
void write_TX_normal_FIFO(char *data, uint8_t length, uint8_t cmd, bool server);

#endif
