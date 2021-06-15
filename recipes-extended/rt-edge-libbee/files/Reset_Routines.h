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

#ifndef __RESET_ROUTINES_H__
#define __RESET_ROUTINES_H__

void RF_reset(void);
void software_reset(void);
void MAC_reset(void);
void BB_reset(void);
void PWR_reset(void);
void pin_reset(void);

#endif
