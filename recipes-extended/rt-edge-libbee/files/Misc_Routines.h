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

#ifndef __MISC_ROUTINES_H__
#define __MISC_ROUTINES_H__

#include <stdint.h>
#include <stdbool.h>

void wakepin_init(void);
void wakepin_deinit(void);
void init_ZIGBEE_nonbeacon(void);
void init_ZIGBEE_basic(void);
void set_TX_power(uint8_t power);
void disable_PLL(void);
void enable_PLL(void);
void pin_wake(void);
void set_wake_from_pin(void);
void set_PAN_ID(uint8_t *address);
void set_PAN_ID(uint8_t *address);
void set_long_address(uint8_t *address);
void set_short_address(uint8_t *address);
void flush_RX_FIFO_pointer(void);
void set_frame_format_filter(uint8_t fff_mode);
void set_reception_mode(uint8_t r_mode);
void set_IFS_default(void);
void set_IFS_recomended(void);
void set_not_encrypt(void);
void set_encrypt(void);
void set_not_ACK(void);
void set_ACK(void);
void nonbeacon_device(void);
void nonbeacon_coordinator_device(void);
void nonbeacon_PAN_coordinator_device(void);
void set_RSSI_mode(uint8_t RSSI_mode);
void set_CCA_mode(uint8_t CCA_mode);
void set_channel(uint8_t channel_number);
void enable_interrupt(void);
bool int_status(void);

#endif
