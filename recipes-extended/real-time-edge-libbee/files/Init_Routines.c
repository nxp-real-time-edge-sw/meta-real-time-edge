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

#include <stdint.h>
#include <unistd.h>
#include <linux/spi/spidev.h>
#include "registers.h"
#include "ReadWrite_Routines.h"
#include "Reset_Routines.h"
#include "Misc_Routines.h"

extern uint8_t ADDRESS_short_1[], ADDRESS_short_2[], ADDRESS_long_1[],
	ADDRESS_long_2[], PAN_ID_1[], PAN_ID_2[];
extern uint8_t LQI, RSSI2, SEQ_NUMBER, lost_data;
extern uint16_t address_RX_FIFO, address_TX_normal_FIFO;
extern uint8_t DATA_TX[];

void Initialize(bool server_flag)
{
	uint8_t i = 0;

	/* variable initialization */
	LQI = 0;
	RSSI2 = 0;
	SEQ_NUMBER = 0x23;
	lost_data = 0;
	address_RX_FIFO = 0x300;
	address_TX_normal_FIFO = 0;

	for (i = 0; i < 2; i++) {
		ADDRESS_short_1[i] = 1;
		ADDRESS_short_2[i] = 2;
		PAN_ID_1[i] = 3;
		PAN_ID_2[i] = 3;
	}

	for (i = 0; i < 8; i++) {
		ADDRESS_long_1[i] = 1;
		ADDRESS_long_2[i] = 2;
	}

	usleep(5000);

	/*
	 * SPI config
	 * Default: mode 0, speed 500k, bit 8
	 */
	struct spi_config config = {
		.mode = 0,
		.speed = 500000,
		.bits = 8,
		.device = "/dev/spidev2.0"
	};
	spidev_init(&config);

	pin_reset();				/* Activate reset from pin */
	software_reset();			/* Activate software reset */
	RF_reset();				/* RF reset */
	wakepin_init();
	set_wake_from_pin();			/* Set wake from pin */

	if (server_flag) {
		set_long_address(ADDRESS_long_1);	/* Set long address */
		set_short_address(ADDRESS_short_1);	/* Set short address */
		set_PAN_ID(PAN_ID_1);			/* Set PAN_ID */
	} else {
		set_long_address(ADDRESS_long_2);	/* Set long address */
		set_short_address(ADDRESS_short_2);	/* Set short address */
		set_PAN_ID(PAN_ID_2);			/* Set PAN_ID */
	}

	init_ZIGBEE_nonbeacon();		/* Initialize ZigBee module */
	nonbeacon_PAN_coordinator_device();
	set_TX_power(31);			/* Set max TX power */
	/* 1 all frames, 3 data frame only */
	set_frame_format_filter(1);
	set_reception_mode(1);			/* 1 normal mode */

	pin_wake();				/* Wake from pin */
}
