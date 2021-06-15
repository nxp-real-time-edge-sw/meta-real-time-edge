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

#ifndef __ACILIB_H__
#define __ACILIB_H__

#define HAL_ACI_MAX_LENGTH 32

/********************* Setup command status *********************/
#define SETUP_SUCCESS                        0
#define SETUP_FAIL_COMMAND_QUEUE_NOT_EMPTY   1
#define SETUP_FAIL_EVENT_QUEUE_NOT_EMPTY     2
#define SETUP_FAIL_TIMEOUT                   3
#define SETUP_FAIL_NOT_SETUP_EVENT           4
#define SETUP_FAIL_NOT_COMMAND_RESPONSE      5

/******************** Device operation modes ********************/
#define ACI_DEVICE_INVALID	0x00
#define ACI_DEVICE_TEST		0x01
#define ACI_DEVICE_SETUP	0x02
#define ACI_DEVICE_STANDBY	0x03
#define ACI_DEVICE_SLEEP	0x04

/********************** Device address type **********************/
#define ACI_BD_ADDR_TYPE_INVALID			0x00
#define ACI_BD_ADDR_TYPE_PUBLIC				0x01
#define ACI_BD_ADDR_TYPE_RANDOM_STATIC			0x02
#define ACI_BD_ADDR_TYPE_RANDOM_PRIVATE_RESOLVABLE	0x03
#define ACI_BD_ADDR_TYPE_RANDOM_PRIVATE_UNRESOLVABLE	0x04

/*********************** ACI status codes ***********************/
/* Success */
#define ACI_STATUS_SUCCESS				0x00
/* Transaction continuation status */
#define ACI_STATUS_TRANSACTION_CONTINUE			0x01
/* Transaction completed */
#define ACI_STATUS_TRANSACTION_COMPLETE			0x02
/* Extended status, further checks needed */
#define ACI_STATUS_EXTENDED				0x03
/* Unknown error */
#define ACI_STATUS_ERROR_UNKNOWN			0x80
/* Internal error */
#define ACI_STATUS_ERROR_INTERNAL			0x81
/* Unknown command */
#define ACI_STATUS_ERROR_CMD_UNKNOWN			0x82
/* Command invalid in the current device state*/
#define ACI_STATUS_ERROR_DEVICE_STATE_INVALID		0x83
/* Invalid length */
#define ACI_STATUS_ERROR_INVALID_LENGTH			0x84
/* Invalid input parameters */
#define ACI_STATUS_ERROR_INVALID_PARAMETER		0x85
/* Busy */
#define ACI_STATUS_ERROR_BUSY				0x86
/* Invalid data format or contents */
#define ACI_STATUS_ERROR_INVALID_DATA			0x87
/* CRC mismatch */
#define ACI_STATUS_ERROR_CRC_MISMATCH			0x88
/* Unsupported setup format */
#define ACI_STATUS_ERROR_UNSUPPORTED_SETUP_FORMAT	0x89
/* Invalid sequence number during a write dynamic data sequence */
#define ACI_STATUS_ERROR_INVALID_SEQ_NO			0x8A
/* Setup data is locked and cannot be modified */
#define ACI_STATUS_ERROR_SETUP_LOCKED			0x8B
/* Setup error due to lock verification failure */
#define ACI_STATUS_ERROR_LOCK_FAILED			0x8C
/* Bond required: Local Pipes need bonded/trusted peer */
#define ACI_STATUS_ERROR_BOND_REQUIRED			0x8D
/* Command rejected as a transaction is still pending */
#define ACI_STATUS_ERROR_REJECTED			0x8E
/*
 * Pipe Error Event : Data size exceeds size specified for pipe
 *                  : Transmit failed
 */
#define ACI_STATUS_ERROR_DATA_SIZE			0x8F
/* Pipe Error Event : Invalid pipe */
#define ACI_STATUS_ERROR_PIPE_INVALID			0x90
/* Pipe Error Event : Credit not available */
#define ACI_STATUS_ERROR_CREDIT_NOT_AVAILABLE		0x91
/* Pipe Error Event : Peer device has sent an error on an pipe operation
 * on the remote characteristic
 */
#define ACI_STATUS_ERROR_PEER_ATT_ERROR			0x92
/* Connection was not established before the BTLE advertising was stopped */
#define ACI_STATUS_ERROR_ADVT_TIMEOUT			0x93
/* Peer has triggered a Security Manager Protocol Error */
#define ACI_STATUS_ERROR_PEER_SMP_ERROR			0x94
/* Pipe Error Event : Pipe type invalid for the selected operation */
#define ACI_STATUS_ERROR_PIPE_TYPE_INVALID		0x95
/* Pipe Error Event : Pipe state invalid for the selected operation */
#define ACI_STATUS_ERROR_PIPE_STATE_INVALID		0x96
/* Invalid key size provided */
#define ACI_STATUS_ERROR_INVALID_KEY_SIZE		0x97
/* Invalid key data provided */
#define ACI_STATUS_ERROR_INVALID_KEY_DATA		0x98
/* Reserved range start */
#define ACI_STATUS_RESERVED_START			0xF0
/* Reserved range end */
#define ACI_STATUS_RESERVED_END				0xFF

/********************* system command code *********************/
/* Enter test mode */
#define ACI_CMD_TEST			0x01
/* Echo (loopback) test command */
#define ACI_CMD_ECHO			0x02
/* Send a BTLE DTM command to the radio */
#define ACI_CMD_DTM_CMD			0x03
/* Put the device to sleep */
#define ACI_CMD_SLEEP			0x04
/* Wakeup the device from deep sleep */
#define ACI_CMD_WAKEUP			0x05
/*
 * Replace the contents of the internal database with
 * user provided data
 */
#define ACI_CMD_SETUP			0x06
/* Read the portions of memory required to be restored after a power cycle */
#define ACI_CMD_READ_DYNAMIC_DATA	0x07
/* Write back the data retrieved using ACI_CMD_READ_DYNAMIC_DATA */
#define ACI_CMD_WRITE_DYNAMIC_DATA	0x08
/* Retrieve the device's version information */
#define ACI_CMD_GET_DEVICE_VERSION	0x09
/* Request the Bluetooth address and its type */
#define ACI_CMD_GET_DEVICE_ADDRESS	0x0A
/* Request the battery level measured by nRF8001 */
#define ACI_CMD_GET_BATTERY_LEVEL	0x0B
/* Request the temperature value measured by nRF8001 */
#define ACI_CMD_GET_TEMPERATURE		0x0C
/* Write to the local Attribute Database */
#define ACI_CMD_SET_LOCAL_DATA		0x0D
/* Reset the baseband and radio and go back to idle */
#define ACI_CMD_RADIO_RESET		0x0E
/* Start advertising and wait for a master connection */
#define ACI_CMD_CONNECT			0x0F
/* Start advertising and wait for a master connection */
#define ACI_CMD_BOND			0x10
/* Start advertising and wait for a master connection */
#define ACI_CMD_DISCONNECT		0x11
/* Throttles the Radio transmit power */
#define ACI_CMD_SET_TX_POWER		0x12
/* Trigger a connection parameter update */
#define ACI_CMD_CHANGE_TIMING		0x13
/* Open a remote pipe for data reception */
#define ACI_CMD_OPEN_REMOTE_PIPE	0x14
/* Transmit data over an open pipe */
#define ACI_CMD_SEND_DATA		0x15
/* Send an acknowledgment of received data */
#define ACI_CMD_SEND_DATA_ACK		0x16
/* Request data over an open pipe */
#define ACI_CMD_REQUEST_DATA		0x17
/* NACK a data reception */
#define ACI_CMD_SEND_DATA_NACK		0x18
/* Set application latency */
#define ACI_CMD_SET_APP_LATENCY		0x19
/* Set a security key */
#define ACI_CMD_SET_KEY			0x1A
/* Open Advertising Pipes */
#define ACI_CMD_OPEN_ADV_PIPE		0x1B
/* Start non-connectable advertising */
#define ACI_CMD_BROADCAST		0x1C
/* Start a security request in bonding mode */
#define ACI_CMD_BOND_SECURITY_REQUEST	0x1D
/* Start Directed advertising towards a Bonded Peer */
#define ACI_CMD_CONNECT_DIRECT		0x1E
/* Close a previously opened remote pipe */
#define ACI_CMD_CLOSE_REMOTE_PIPE	0x1F
/* Invalid ACI command opcode */
#define ACI_CMD_INVALID			0xFF

/*********************** system event code ***********************/
/* Invalid event code */
#define ACI_EVT_INVALID		0x00
/* Sent every time the device starts */
#define ACI_EVT_DEVICE_STARTED	0x81
/* Mirrors the ACI_CMD_ECHO */
#define ACI_EVT_ECHO		0x82
/* Asynchronous hardware error event */
#define ACI_EVT_HW_ERROR	0x83
/* Event opcode used as a event response for all commands */
#define ACI_EVT_CMD_RSP		0x84
/* Link connected */
#define ACI_EVT_CONNECTED	0x85
/* Link disconnected */
#define ACI_EVT_DISCONNECTED	0x86
/* Bond completion result */
#define ACI_EVT_BOND_STATUS	0x87
/* Pipe bitmap for available pipes */
#define ACI_EVT_PIPE_STATUS	0x88
/*
 * Sent to the application when the radio enters a connected state
 * or when the timing of the radio connection changes
 */
#define ACI_EVT_TIMING		0x89
/*
 * Notification to the application that transmit credits are
 * available
 */
#define ACI_EVT_DATA_CREDIT	0x8A
/* Data acknowledgement event */
#define ACI_EVT_DATA_ACK	0x8B
/* Data received notification event */
#define ACI_EVT_DATA_RECEIVED	0x8C
/* Error notification event */
#define ACI_EVT_PIPE_ERROR	0x8D
/* Display Passkey Event */
#define ACI_EVT_DISPLAY_PASSKEY	0x8E
/* Security Key request */
#define ACI_EVT_KEY_REQUEST	0x8F

/********** the ACI_CMD_CONNECT ACI command parameters **********/
struct cmd_params_connect {
	/* 0x0000 (no timeout) to 0x3FFF */
	uint16_t timeout;
	/* 16 bits of advertising interval for general discovery */
	uint16_t adv_interval;
};

/********************* setup command struct *********************/
struct setup_data {
	uint8_t status_byte;
	uint8_t buffer[HAL_ACI_MAX_LENGTH];
};

/******************** device address content *********************/
struct getaddr_response {
	uint8_t addr[6];
	uint8_t addr_type;
};

/******************** device version content *********************/
struct getversion_response {
	uint16_t configuration_id;
	uint8_t  aci_version;
	uint8_t  setup_format;
	uint32_t setup_id;
	uint8_t  setup_status;
};

/***************** CommandResponseEvent content *****************/
struct evt_params_response {
	uint8_t cmd_opcode;
	uint8_t cmd_status;
	union {
		struct getaddr_response get_device_address;
		uint8_t response_data[27];
	} content;
};

/****************** DeviceStartedEvent content ******************/
struct evt_params_started {
	uint8_t device_mode;
	uint8_t hw_error;
	uint8_t credit_available;/*Number of DataCommand buffers available*/
};

/********** The ACI_EVT_TIMING event return parameters **********/
struct evt_params_timing {
	/* rf_interval = conn_rf_interval * 1.25 ms Range:0x0006 to 0x0C80 */
	uint16_t conn_rf_interval;
	/* Number of RF events the slave can skip */
	uint16_t conn_slave_rf_latency;
	/*
	 * Timeout as a multiple of 10ms
	 * i.e timeout = conn_rf_timeout * 10ms Range: 0x000A to 0x0C80
	 */
	uint16_t conn_rf_timeout;
};

/******** The ACI_EVT_PIPE_ERROR event return parameters ********/
struct evt_params_pipe_error {
	uint8_t pipe_number;
	uint8_t error_code;
	uint8_t error_data[27];
};

/********************* ACI packet structure *********************/
struct aci_packet {
	uint8_t dump;
	uint8_t length;
	uint8_t opcode;
	union {
		struct evt_params_started device_started;
		struct evt_params_response cmd_rsp;
		uint8_t payload[30];
	} params;
};
#define REV_PKG_SIZE	sizeof(struct aci_packet)

struct spi_config {
	uint32_t mode;
	uint32_t speed;
	char *device;
	uint8_t bits;
};

typedef void (*hook_func)(struct aci_packet *packet_rx);

extern hook_func event_get_addr_hook;
extern hook_func event_get_version_hook;
extern hook_func event_timing_hook;
extern hook_func event_data_receive_hook;

extern struct cmd_params_connect adv_params;

void spidev_init(struct spi_config *config);
void aci_send_cmd(uint8_t cmd, void *param, uint8_t len);
uint8_t do_aci_setup(void);
void aci_event_handle_loop(void);
void close_spidev(void);

#endif
