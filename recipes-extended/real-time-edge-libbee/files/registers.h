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

#ifndef __REGISTERS_H__
#define __REGISTERS_H__

/////////////////////////////////////////
////// short address registers  /////////
/////////////////////////////////////////

#define RXMCR     0x00
#define PANIDL    0x01
#define PANIDH    0x02
#define SADRL     0x03
#define SADRH     0x04
#define EADR0     0x05
#define EADR1     0x06
#define EADR2     0x07
#define EADR3     0x08
#define EADR4     0x09
#define EADR5     0x0A
#define EADR6     0x0B
#define EADR7     0x0C
#define RXFLUSH   0x0D
#define ORDER     0x10
#define TXMCR     0x11
#define ACKTMOUT  0x12
#define ESLOTG1   0x13
#define SYMTICKL  0x14
#define SYMTICKH  0x15
#define PACON0    0x16
#define PACON1    0x17
#define PACON2    0x18
#define TXBCON0   0x1A
#define TXNCON    0x1B
#define TXG1CON   0x1C
#define TXG2CON   0x1D
#define ESLOTG23  0x1E
#define ESLOTG45  0x1F
#define ESLOTG67  0x20
#define TXPEND    0x21
#define WAKECON   0x22
#define FRMOFFSET 0x23
#define TXSTAT    0x24
#define TXBCON1   0x25
#define GATECLK   0x26
#define TXTIME    0x27
#define HSYMTMRL  0x28
#define HSYMTMRH  0x29
#define SOFTRST   0x2A
#define SECCON0   0x2C
#define SECCON1   0x2D
#define TXSTBL    0x2E
#define RXSR      0x30
#define INTSTAT   0x31
#define INTCON_M  0x32
#define GPIO      0x33
#define TRISGPIO  0x34
#define SLPACK    0x35
#define RFCTL     0x36
#define SECCR2    0x37
#define BBREG0    0x38
#define BBREG1    0x39
#define BBREG2    0x3A
#define BBREG3    0x3B
#define BBREG4    0x3C
#define BBREG6    0x3E
#define CCAEDTH   0x3F

///////////////////////////////////////////
//////// long address registers  //////////
///////////////////////////////////////////

#define RFCON0    0x200
#define RFCON1    0x201
#define RFCON2    0x202
#define RFCON3    0x203
#define RFCON5    0x205
#define RFCON6    0x206
#define RFCON7    0x207
#define RFCON8    0x208
#define SLPCAL0   0x209
#define SLPCAL1   0x20A
#define SLPCAL2   0x20B
#define RFSTATE   0x20F
#define RSSI      0x210
#define SLPCON0   0x211
#define SLPCON1   0x220
#define WAKETIMEL 0x222
#define WAKETIMEH 0x223
#define REMCNTL   0x224
#define REMCNTH   0x225
#define MAINCNT0  0x226
#define MAINCNT1  0x227
#define MAINCNT2  0x228
#define MAINCNT3  0x229
#define ASSOEADR0 0x230
#define ASSOEADR1 0x231
#define ASSOEADR2 0x232
#define ASSOEADR3 0x233
#define ASSOEADR4 0x234
#define ASSOEADR5 0x235
#define ASSOEADR6 0x236
#define ASSOEADR7 0x237
#define ASSOSADR0 0x238
#define ASSOSADR1 0x239
#define UPNONCE0  0x240
#define UPNONCE1  0x241
#define UPNONCE2  0x242
#define UPNONCE3  0x243
#define UPNONCE4  0x244
#define UPNONCE5  0x245
#define UPNONCE6  0x246
#define UPNONCE7  0x247
#define UPNONCE8  0x248
#define UPNONCE9  0x249
#define UPNONCE10 0x24A
#define UPNONCE11 0x24B
#define UPNONCE12 0x24C

#endif
