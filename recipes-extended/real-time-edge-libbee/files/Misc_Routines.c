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
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include "registers.h"
#include "ReadWrite_Routines.h"
#include "Reset_Routines.h"
#include "Misc_Routines.h"

void wakepin_init(void)
{
	if (access("/sys/class/gpio/gpio479", F_OK))
		system("echo 479 > /sys/class/gpio/export");

	system("echo out > /sys/class/gpio/gpio479/direction");
}

void wakepin_deinit(void)
{
	system("echo 479 > /sys/class/gpio/unexport");
}

void set_wakepin(uint8_t value)
{
	char cmd[40] = {0};

	value = (value == 0) ? 0 : 1;
	sprintf(cmd, "echo %d > /sys/class/gpio/gpio479/value", value);
	system(cmd);
}

/*
 *  Interrupt
 */
void enable_interrupt(void)
{
	/* 0xF7 Only reception interrupt is enable */
	write_ZIGBEE_short(INTCON_M, 0xF7);
}

/*
 *  Set channel
 */
void set_channel(uint8_t channel_number)
{
	/* 11-26 possible channels */
	if ((channel_number > 26) || (channel_number < 11))
		channel_number = 11;

	switch (channel_number) {
	case 11:
		write_ZIGBEE_long(RFCON0, 0x02);  /* 0x02 for 11. channel */
		break;
	case 12:
		write_ZIGBEE_long(RFCON0, 0x12);  /* 0x12 for 12. channel */
		break;
	case 13:
		write_ZIGBEE_long(RFCON0, 0x22);  /* 0x22 for 13. channel */
		break;
	case 14:
		write_ZIGBEE_long(RFCON0, 0x32);  /* 0x32 for 14. channel */
		break;
	case 15:
		write_ZIGBEE_long(RFCON0, 0x42);  /* 0x42 for 15. channel */
		break;
	case 16:
		write_ZIGBEE_long(RFCON0, 0x52);  /* 0x52 for 16. channel */
		break;
	case 17:
		write_ZIGBEE_long(RFCON0, 0x62);  /* 0x62 for 17. channel */
		break;
	case 18:
		write_ZIGBEE_long(RFCON0, 0x72);  /* 0x72 for 18. channel */
		break;
	case 19:
		write_ZIGBEE_long(RFCON0, 0x82);  /* 0x82 for 19. channel */
		break;
	case 20:
		write_ZIGBEE_long(RFCON0, 0x92);  /* 0x92 for 20. channel */
		break;
	case 21:
		write_ZIGBEE_long(RFCON0, 0xA2);  /* 0xA2 for 21. channel */
		break;
	case 22:
		write_ZIGBEE_long(RFCON0, 0xB2);  /* 0xB2 for 22. channel */
		break;
	case 23:
		write_ZIGBEE_long(RFCON0, 0xC2);  /* 0xC2 for 23. channel */
		break;
	case 24:
		write_ZIGBEE_long(RFCON0, 0xD2);  /* 0xD2 for 24. channel */
		break;
	case 25:
		write_ZIGBEE_long(RFCON0, 0xE2);  /* 0xE2 for 25. channel */
		break;
	case 26:
		write_ZIGBEE_long(RFCON0, 0xF2);  /* 0xF2 for 26. channel */
		break;
	}
	RF_reset();
}

/*
 *  Set CCA mode
 */
void set_CCA_mode(uint8_t CCA_mode)
{
	uint8_t temp = 0;

	switch (CCA_mode) {
	case 1: {	/* ENERGY ABOVE THRESHOLD */
		temp = read_ZIGBEE_short(BBREG2);
		temp = temp | 0x80;	/* 0x80 mask */
		temp = temp & 0xDF;	/* 0xDF mask */
		write_ZIGBEE_short(BBREG2, temp);
		/* Set CCA ED threshold to -69 dBm */
		write_ZIGBEE_short(CCAEDTH, 0x60);
	}
	break;

	case 2: {	/* CARRIER SENSE ONLY */
		temp = read_ZIGBEE_short(BBREG2);
		temp = temp | 0x40;	/* 0x40 mask */
		temp = temp & 0x7F;	/* 0x7F mask */
		write_ZIGBEE_short(BBREG2, temp);
		/* carrier sense threshold */
		temp = read_ZIGBEE_short(BBREG2);
		temp = temp | 0x38;
		temp = temp & 0xFB;
		write_ZIGBEE_short(BBREG2, temp);
	}
	break;

	case 3: {	/* CARRIER SENSE AND ENERGY ABOVE THRESHOLD */
		temp = read_ZIGBEE_short(BBREG2);
		temp = temp | 0xC0;	/* 0xC0 mask */
		write_ZIGBEE_short(BBREG2, temp);
		/* carrier sense threshold */
		temp = read_ZIGBEE_short(BBREG2);
		temp = temp | 0x38;	/* 0x38 mask */
		temp = temp & 0xFB;	/* 0xFB mask */
		write_ZIGBEE_short(BBREG2, temp);
		/* Set CCA ED threshold to -69 dBm */
		write_ZIGBEE_short(CCAEDTH, 0x60);
	}
	break;
	}
}

/*
 *  Set RSSI mode
 */
void set_RSSI_mode(uint8_t RSSI_mode)
{
	/* 1 for RSSI1, 2 for RSSI2 mode */
	uint8_t temp = 0;

	switch (RSSI_mode) {
	case 1: {
		temp = read_ZIGBEE_short(BBREG6);
		temp = temp | 0x80;	/* 0x80 mask for RSSI1 mode */
		write_ZIGBEE_short(BBREG6, temp);
	}
	break;

	case 2:	/* 0x40 data for RSSI2 mode */
		write_ZIGBEE_short(BBREG6, 0x40);
		break;
	}
}

/*
 * Set type of device
 */
void nonbeacon_PAN_coordinator_device(void)
{
	uint8_t temp = 0;

	temp = read_ZIGBEE_short(RXMCR);
	temp = temp | 0x08;	/* 0x08 mask for PAN coordinator */
	write_ZIGBEE_short(RXMCR, temp);

	temp = read_ZIGBEE_short(TXMCR);
	temp = temp & 0xDF;	/* 0xDF mask for CSMA-CA mode */
	write_ZIGBEE_short(TXMCR, temp);

	write_ZIGBEE_short(ORDER, 0xFF);	/* BO, SO are 15 */
}

void nonbeacon_coordinator_device(void)
{
	uint8_t temp = 0;

	temp = read_ZIGBEE_short(RXMCR);
	temp = temp | 0x04;	/* 0x04 mask for coordinator */
	write_ZIGBEE_short(RXMCR, temp);

	temp = read_ZIGBEE_short(TXMCR);
	temp = temp & 0xDF;	/* 0xDF mask for CSMA-CA mode */
	write_ZIGBEE_short(TXMCR, temp);

	write_ZIGBEE_short(ORDER, 0xFF);	/* BO, SO  are 15 */
}

void nonbeacon_device(void)
{
	uint8_t temp = 0;

	temp = read_ZIGBEE_short(RXMCR);
	/* 0xF3 mask for PAN coordinator and coordinator */
	temp = temp & 0xF3;
	write_ZIGBEE_short(RXMCR, temp);

	temp = read_ZIGBEE_short(TXMCR);
	temp = temp & 0xDF;	/* 0xDF mask for CSMA-CA mode */
	write_ZIGBEE_short(TXMCR, temp);
}

/*
 * ACK request
 */
void set_ACK(void)
{
	uint8_t temp = 0;

	temp = read_ZIGBEE_short(TXNCON);
	temp = temp | 0x04;	/* 0x04 mask for set ACK */
	write_ZIGBEE_short(TXNCON, temp);
}

void set_not_ACK(void)
{
	uint8_t temp = 0;

	temp = read_ZIGBEE_short(TXNCON);
	temp = temp & (uint8_t)(~0x04);	/* 0x04 mask for set not ACK */
	write_ZIGBEE_short(TXNCON, temp);
}

/*
 *  Encrypt
 */
void set_encrypt(void)
{
	uint8_t temp = 0;

	temp = read_ZIGBEE_short(TXNCON);
	temp = temp | 0x02;	/* mask for set encrypt */
	write_ZIGBEE_short(TXNCON, temp);
}

void set_not_encrypt(void)
{
	uint8_t temp = 0;

	temp = read_ZIGBEE_short(TXNCON);
	temp = temp & (uint8_t)(~0x02);	/* mask for set not encrypt */
	write_ZIGBEE_short(TXNCON, temp);
}

/*
 * Interframe spacing
 */
void set_IFS_recomended(void)
{
	uint8_t temp = 0;

	write_ZIGBEE_short(RXMCR, 0x93);	/* Min SIFS Period */

	temp = read_ZIGBEE_short(TXPEND);
	temp = temp | 0x7C;			/* MinLIFSPeriod */
	write_ZIGBEE_short(TXPEND, temp);

	temp = read_ZIGBEE_short(TXSTBL);
	temp = temp | 0x90;			/* MinLIFSPeriod */
	write_ZIGBEE_short(TXSTBL, temp);

	temp = read_ZIGBEE_short(TXTIME);
	temp = temp | 0x31;			/* TurnaroundTime */
	write_ZIGBEE_short(TXTIME, temp);
}

void set_IFS_default(void)
{
	uint8_t temp = 0;

	write_ZIGBEE_short(RXMCR, 0x75);	/* Min SIFS Period */

	temp = read_ZIGBEE_short(TXPEND);
	temp = temp | 0x84;			/* Min LIFS Period */
	write_ZIGBEE_short(TXPEND, temp);

	temp = read_ZIGBEE_short(TXSTBL);
	temp = temp | 0x50;			/* Min LIFS Period */
	write_ZIGBEE_short(TXSTBL, temp);

	temp = read_ZIGBEE_short(TXTIME);
	temp = temp | 0x41;			/* Turnaround Time */
	write_ZIGBEE_short(TXTIME, temp);
}

/*
 * Reception mode
 */
void set_reception_mode(uint8_t r_mode)
{
	/* 1 normal, 2 error, 3 promiscuous mode */
	uint8_t temp = 0;

	switch (r_mode) {
	case 1: {
		temp = read_ZIGBEE_short(RXMCR);/* normal mode */
		temp = temp & (uint8_t)(~0x03);	/* mask for normal mode */
		write_ZIGBEE_short(RXMCR, temp);
	}
	break;

	case 2: {
		temp = read_ZIGBEE_short(RXMCR);/* error mode */
		temp = temp & (uint8_t)(~0x01);	/* mask for error mode */
		temp = temp | 0x02;		/* mask for error mode */
		write_ZIGBEE_short(RXMCR, temp);
	}
	break;

	case 3: {
		temp = read_ZIGBEE_short(RXMCR);/* promiscuous mode */
		temp = temp & (uint8_t)(~0x02);	/* mask for promiscuous mode */
		temp = temp | 0x01;		/* mask for promiscuous mode */
		write_ZIGBEE_short(RXMCR, temp);
	}
	break;
	}
}

/*
 *  Frame format filter
 */
void set_frame_format_filter(uint8_t fff_mode)
{
	/* 1 all frames, 2 command only, 3 data only, 4 beacon only */
	uint8_t temp = 0;

	switch (fff_mode) {
	case 1: {
		temp = read_ZIGBEE_short(RXFLUSH);	/* all frames */
		/* mask for all frames */
		temp = temp & (uint8_t)(~0x0E);
		write_ZIGBEE_short(RXFLUSH, temp);
	}
	break;

	case 2: {
		temp = read_ZIGBEE_short(RXFLUSH);	/* command only */
		temp = temp & (uint8_t)(~0x06);	/* mask for command only */
		temp = temp | 0x08;		/* mask for command only */
		write_ZIGBEE_short(RXFLUSH, temp);
	}
	break;

	case 3: {
		temp = read_ZIGBEE_short(RXFLUSH);	/* data only */
		temp = temp & (uint8_t)(~0x0A);	/* mask for data only */
		temp = temp | 0x04;		/* mask for data only */
		write_ZIGBEE_short(RXFLUSH, temp);
	}
	break;

	case 4: {
		temp = read_ZIGBEE_short(RXFLUSH);	/* beacon only */
		temp = temp & (uint8_t)(~0x0C);	/* mask for beacon only */
		temp = temp | 0x02;		/* mask for beacon only */
		write_ZIGBEE_short(RXFLUSH, temp);
	}
	break;
	}
}

/*
 *  Flush RX FIFO pointer
 */
void flush_RX_FIFO_pointer(void)
{
	uint8_t temp;

	temp = read_ZIGBEE_short(RXFLUSH);
	temp = temp | 0x01;	/* mask for flush RX FIFO */
	write_ZIGBEE_short(RXFLUSH, temp);
}

/*
 * Address
 */
void set_short_address(uint8_t *address)
{
	write_ZIGBEE_short(SADRL, address[0]);
	write_ZIGBEE_short(SADRH, address[1]);
}

void set_long_address(uint8_t *address)
{
	uint8_t i = 0;

	for (i = 0; i < 8; i++) {
		/* 0x05 address of EADR0 */
		write_ZIGBEE_short(EADR0 + i, address[i]);
	}
}

void set_PAN_ID(uint8_t *address)
{
	write_ZIGBEE_short(PANIDL, address[0]);
	write_ZIGBEE_short(PANIDH, address[1]);
}

/*
 * Wake
 */
void set_wake_from_pin(void)
{
	uint8_t temp = 0;

	set_wakepin(0);
	temp = read_ZIGBEE_short(RXFLUSH);
	temp = temp | 0x60;	/* mask */
	write_ZIGBEE_short(RXFLUSH, temp);

	temp = read_ZIGBEE_short(WAKECON);
	temp = temp | 0x80;
	write_ZIGBEE_short(WAKECON, temp);
}

void pin_wake(void)
{
	set_wakepin(1);
	usleep(5000);
}

/*
 * PLL
 */
void enable_PLL(void)
{
	/* mask for PLL enable */
	write_ZIGBEE_long(RFCON2, 0x80);
}

void disable_PLL(void)
{
	/* mask for PLL disable */
	write_ZIGBEE_long(RFCON2, 0x00);
}

/*
 * Tx power
 */
void set_TX_power(uint8_t power)
{
	/* 0-31 possible variants */
	if ((power < 0) || (power > 31))
		power = 31;
	/* 0 max, 31 min -> 31 max, 0 min */
	power = 31 - power;
	/* calculating power */
	power = ((power & 0b00011111) << 3) & 0b11111000;
	write_ZIGBEE_long(RFCON3, power);
}

/*
 * Init ZIGBEE module
 */
void init_ZIGBEE_basic(void)
{
	/* Initialize FIFOEN = 1 and TXONTS = 0x6 */
	write_ZIGBEE_short(PACON2, 0x98);
	/* Initialize RFSTBL = 0x9 */
	write_ZIGBEE_short(TXSTBL, 0x95);
	/* Initialize VCOOPT = 0x01 */
	write_ZIGBEE_long(RFCON1, 0x01);
	/* Enable PLL (PLLEN = 1) */
	enable_PLL();
	/* Initialize TXFIL = 1 and 20MRECVR = 1 */
	write_ZIGBEE_long(RFCON6, 0x90);
	/* Initialize SLPCLKSEL = 0x2 (100 kHz Internal oscillator) */
	write_ZIGBEE_long(RFCON7, 0x80);
	/* Initialize RFVCO = 1 */
	write_ZIGBEE_long(RFCON8, 0x10);
	/* Initialize CLKOUTEN = 1 and SLPCLKDIV = 0x01 */
	write_ZIGBEE_long(SLPCON1, 0x21);
}

void init_ZIGBEE_nonbeacon(void)
{
	init_ZIGBEE_basic();
	set_CCA_mode(1);	/* Set CCA mode to ED and set threshold */
	set_RSSI_mode(2);	/* RSSI2 mode */
	enable_interrupt();	/* Enables all interrupts */
	set_channel(11);	/* Channel 11 */
	RF_reset();
}

bool int_status(void)
{
	char data[5] = {0};
	int res;
	FILE *fp;

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

