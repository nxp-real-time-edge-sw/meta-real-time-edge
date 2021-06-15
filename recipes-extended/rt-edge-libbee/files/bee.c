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

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Init_Routines.h"
#include "ReadWrite_Routines.h"
#include "Misc_Routines.h"
#include "registers.h"

bool server_flag = false;
bool client_flag = false;
char *file = NULL;
char file_name[30] = {0};
uint8_t name_len = 0;

void draw_frame(bool server_flag)
{
	printf("BEE  Click  Board  Demo.\n");
	if (server_flag) {
		printf("This node is a server node.\n");
		printf("Waiting for a client\n");
	} else {
		printf("This node is a client node.\n");
		printf("Starting to get a file\n");
	}
}

void help_str(void)
{
	printf("Usage: bee_demo -s/-c [-f=XXX]\n");
	printf("    -s: server node\n");
	printf("    -c: client node\n");
	printf("    -f=XXX: XXX is a path of the file to send\n");
	printf("        This parameter is valid if this is  a server node.\n");
}

char *get_name(char *path)
{
	char *name = strchr(path, '/');

	if (name == NULL)
		return path;
	else
		return get_name(name + 1);
}


/*
 * -s: server
 * -c: client
 * -f=XX: file
 */
uint8_t parser_args(int argc, char *argv[])
{
	uint8_t i = 1;
	char *name = NULL;

	if (argc == 1) {
		help_str();
		return 1;
	}

	for (i = 1; i < argc; i++) {
		switch (argv[i][1]) {
		case 's':
			server_flag = true;
			break;

		case 'c':
			client_flag = true;
			break;

		case 'f':
			file = &argv[i][3];
			name = get_name(file);
			name_len = strlen(name);
			memcpy(file_name, name, name_len);
			break;

		default:
			printf("Parameter is invalid\n");
			return 2;
		}
	}

	if (server_flag && client_flag) {
		printf("‘-s’ and ‘-c’ cann‘t exist at the same time\n");
		return 3;
	}

	if (server_flag && (file == NULL)) {
		printf("Please input a file\n");
		return 4;
	}

	return 0;
}

#define MAX_TX      DATA_LENGTH
/* SEQ_NUMBER */
#define SEQ_DATA    0x0A    /* file content, server */
#define SEQ_REQ     0x0B    /* require, client */
#define SEQ_INFO    0x0C    /* total size(4bytes) + file name, server */
#define SEQ_START   0x0D    /* start, client */
#define SEQ_END     0x0E    /* finish, server */
int BUFFER_SIZE = 0;
unsigned char *file_buffer;

unsigned char get_file_size(void)
{
	struct stat buf;

	if (stat(file, &buf) < 0)
		return 1;

	BUFFER_SIZE = buf.st_size;
	file_buffer = (unsigned char *)malloc(BUFFER_SIZE + 32);
	return 0;
}


void get_file(int *readSize)
{
	int fd = 0;

	printf("Reading the content of the file\n");
	fd = open(file, O_RDONLY);
	if (fd == -1) {
		printf("Test file open faild !\n");
		*readSize = -1;
		return;
	}
	memset(file_buffer, 0, BUFFER_SIZE);
	*readSize = read(fd, file_buffer, BUFFER_SIZE);
	if (*readSize == -1) {
		printf("Read file faild !\n");
	}
	close(fd);
}

void recv_file(int file_size)
{
	int fd = 0;

	fd = open(file_name, O_WRONLY | O_CREAT, 0777);
	write(fd, file_buffer, file_size);
	close(fd);
}

bool send_file(int file_size, int *send_size)
{
	int remain_size;

	if (*send_size == 0)
		printf("Start to send the file\n");
	if (*send_size < file_size) {
		remain_size = file_size - (*send_size);
		if (remain_size >= MAX_TX) {
			write_TX_normal_FIFO(&file_buffer[*send_size], MAX_TX,
					     SEQ_DATA, server_flag);
			*send_size += MAX_TX;
		} else {
			write_TX_normal_FIFO(&file_buffer[*send_size],
					    remain_size, SEQ_DATA, server_flag);
			*send_size += remain_size;
		}
		return false; /* continue */
	} else {
		*send_size = 0;
		write_TX_normal_FIFO(DATA_TX, 1, SEQ_END, server_flag);
		printf("It's completed to send a file.\n");
		return true; /* finish */
	}
}

void send_cmd(uint8_t cmd, int arg)
{
	uint8_t data_len = 0;

	switch (cmd) {
	case SEQ_REQ:   /* client */
	case SEQ_START: /* client */
		data_len = 1;
		break;

	case SEQ_INFO: /* server */
		memcpy(DATA_TX, &arg, 4);
		memcpy(&DATA_TX[4], &name_len, 1);
		memcpy(&DATA_TX[5], file_name, name_len);
		data_len = 5 + name_len;
		break;
	}
	write_TX_normal_FIFO(DATA_TX, data_len, cmd, server_flag);
}

/*   client                   server
 *     |        require         |
 *     |----------------------->|
 *     |  file_info(size+name)  |
 *     |<-----------------------|
 *     |         start          |
 *     |----------------------->|
 *     |         data0          |
 *     |<-----------------------|
 *     |         start          |
 *     |----------------------->|
 *     |         dataN          |
 *     |<-----------------------|
 *     |         start          |
 *     |----------------------->|
 *     |      send_finish       |
 *     |<-----------------------|
 * */
int main(int argc, char *argv[])
{
	int file_size = 0, send_size = 0, recv_size = 0;
	uint8_t temp, try = 0;
	uint8_t *buffer = DATA_RX;

	if (parser_args(argc, argv))
		return 1;

	Initialize(server_flag);
	draw_frame(server_flag);

	if (server_flag) {
		if (get_file_size()) {
			printf("Failed to obtain file size\n");
			return 4;
		}
		get_file(&file_size);
		if (file_size == -1)
			return 2;
	} else {
		printf("Send the SEQ_REQ command.\n");
		send_cmd(SEQ_REQ, 0);
	}

	/* Infinite loop */
	for (;;) {
		if (int_status()) {
			/* Read and flush register INTSTAT */
			temp = read_ZIGBEE_short(INTSTAT);
			read_RX_FIFO(buffer);	/* Read receive data */
			switch (SEQ_NUMBER) {
			case SEQ_REQ:	/* server */
				send_size = 0;
				printf("Send the SEQ_INFO command.\n");
				send_cmd(SEQ_INFO, file_size);
				break;

			case SEQ_INFO:	/* client */
				memcpy(&file_size, buffer, 4);
				memcpy(&name_len, &buffer[4], 1);
				memcpy(file_name, &buffer[5], name_len);
				file_name[name_len] = '\0';
				file_buffer = (unsigned char *)
						malloc(file_size + 32);
				recv_size = 0;
				buffer = file_buffer;
				printf("Send the SEQ_START command.\n");
				send_cmd(SEQ_START, 0);
				break;

			case SEQ_START:	/* server */
				send_file(file_size, &send_size);
				break;

			case SEQ_DATA: /* client */
				recv_size += RX_SIZE - HEADER_LENGTH - 2;
				buffer += recv_size;
				printf("Send the SEQ_START command.\n");
				send_cmd(SEQ_START, 0);
				break;

			case SEQ_END: /* client */
				if (file_size == recv_size) {
					buffer = DATA_RX;
					recv_file(file_size);
					free(file_buffer);
					return 0;
				} else {
					try++;
					printf("Received file is invalid\n");
					if (try > 3) {
						printf("Try again\n");
						send_cmd(SEQ_REQ, 0);
					} else {
						printf("Exit!\n");
						return 3;
					}
				}
				break;
			}
		}

		usleep(500000);
	}
}
