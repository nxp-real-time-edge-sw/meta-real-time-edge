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

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include "registers.h"
#include "ReadWrite_Routines.h"

/* RST pin */
static void set_rstpin(uint8_t val)
{
	char cmd[50];

	val = (val > 0) ? 0x03 : 0x02;
	sprintf(cmd, "i2cset -y 0 0x66 0x55 0x%02X b", val);
	system(cmd);
}


/*
 * Reset functions
 */

/* Reset from pin */
void pin_reset(void)
{
	set_rstpin(0);	/* activate reset */
	usleep(5000);
	set_rstpin(1);	/* deactivate reset */
	usleep(5000);
}

void PWR_reset(void)
{
	/* 0x04  mask for RSTPWR bit */
	write_ZIGBEE_short(SOFTRST, 0x04);
}

void BB_reset(void)
{
	/* 0x02 mask for RSTBB bit */
	write_ZIGBEE_short(SOFTRST, 0x02);
}

void MAC_reset(void)
{
	/* 0x01 mask for RSTMAC bit */
	write_ZIGBEE_short(SOFTRST, 0x01);
}

void software_reset(void)
{
	/* PWR_reset,BB_reset and MAC_reset at once */
	write_ZIGBEE_short(SOFTRST, 0x07);
}

void RF_reset(void)
{
	uint8_t temp = 0;

	temp = read_ZIGBEE_short(RFCTL);
	temp = temp | 0x04;		/* mask for RFRST bit */
	write_ZIGBEE_short(RFCTL, temp);
	temp = temp & ((uint8_t)~0x04);	/* mask for RFRST bit */
	write_ZIGBEE_short(RFCTL, temp);
	usleep(1000);
}
