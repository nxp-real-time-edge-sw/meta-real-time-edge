## real-time-edge-modbus-simulator
### real-time-edge-modbus-simulator Introduction

MODBUS is a communication protocol located at the application layer of the OSI seven-layer model, that 
provides client/server communication between devices connected on different types of buses or networks. 

real-time-edge-modbus-simulator is a Modbus simulator running on Linux, I.MX8MM  and I.MX8MP boards.

It is based on the libmodbus open source library and implements Modbus-device-simulator and Modbus-client-simulator. 

real-time-edge-modbus-simulator supports both TCP and RTU modes, and each mode supports the following functions.

#### Features supported by TCP:
	-Get CPU temperature of device.
	-Get the status of the LED light of device.
	-Modify the status of the LED light of device.

#### Features supported by RTU:
	-Get CPU temperature of device.
	-Get the status of the LED light of device.
	-Modify the status of the LED light of device.
	-Modify the slave address of the device.
	-Modify the baud rate of the device.

### real-time-edge-modbus-simulator Usage

#### SYNOPSIS
	{modbus_device_simulator|modbus_client_simulator} [OPTION]… {RTU-PARAMS|TCP-PARAMS}… {SERIALPORT|HOST}… [WRITE-DATA]… 
	
#### DESCRIPTION
	The parameter of [Option] is the general parameter of TCP and RTU startup.
	--debug, 	show debug information.
	-m, 	connection Type TCP/RTU, optional parameter: {tcp|rtu}.
	-a,	slave address.
	-c,	read and write data number.
	-t,	function codes, the following function codes are available.
		(0x01) Read Coils, (0x02) Read Discrete Inputs, (0x05) Write Single Coil
		(0x03) Read Holding Registers, (0x04) Read Input Registers, (0x06) WriteSingle Register
	-r,	register start address.
	-o,	response timeout(ms).
	The parameter of RTU-PARAMS is the general parameter of RTU startup.
	-b,	baud rate, optional parameter: {4800|9600|19200|115200}.
		NOTE: when running the modbus_device_simulator, directly select the above parameters, when running the modbus_client_simulator, select the index of the 
		parameters:  {0|1|2|3}.
	-d,	data bits, optional parameter: {7|8}.
	-s,	stop bits, optional parameter: {1|2}.
	-p,	verify type, optional parameter: {none | even | odd}.
	The parameter of TCP-PARAMS is the general parameter of TCP startup.
	-p,	port.

#### EXAMPLES
	Examples of TCP modbus_device_simulator and modbus_client_simulator startup are as follows:
	
	modbus_device_simulator --debug -m tcp -p 1502 0.0.0.0
				Start modbus_device_simulator locally with port 1502.
				
	modbus_client_simulator --debug -m tcp -a 1 -t 0x05 -r 0 -p 1502 127.0.0.1 0xFF00
				Start modbus_client_simulator and connect to the modbus_device_simulator with 
				127.0.0.1 and port 1502, function code is Write Single Coil.
				Function: Change the status of the LED light to 1.
				
	Examples of RTU modbus_device_simulator and modbus_client_simulator startup are as follows:
	
	modbus_device_simulator --debug -m rtu -a 1 -b 115200 -p none /dev/ttyUSB1
				Start modbus_device_simulator serial connection, slave address is 1, baud rate 
				is 115200, verify type is none, device is /dev/ttyUSB1.
				
	modbus_client_simulator --debug -m rtu -a 1 -t 0x06 -r 1 -b 1 -p none /dev/ttyUSB0 3
				Start modbus_client_simulator serial connection, slave address is 1, function code is 
				Write Single Register, register start address is 1, baud rate is 115200, 
				verify type is none, device is /dev/ttyUSB0, write date is 1.
				Function: Change the baud rate of /dev/ttyUSB1 from 9600 to 115200.
				
#### COMMANDS FOR ALL FEATRUES
	The commands for all featrues of TCP are as follows:
	
	modbus_client_simulator --debug -m tcp -a 1 -t 0x01 -r 0 -p 1502 127.0.0.1
				Read the status of the LED light.
	
	modbus_client_simulator --debug -m tcp -c 1 -t 0x05 -r 0 -p 1502 127.0.0.1 {0xFF00|0x0000}
				Change the status of the LED light.
				
	modbus_client_simulator --debug -m tcp -a 1 -t 0x04 -r 0 -p 1502 127.0.0.1
				Read the temperature of CPU.
				
	The commands for all featrues of RTU are as follows: 
	
	modbus_client_simulator --debug -m rtu -a 1 -t 0x01 -r 0 -b 3 -p none /dev/ttyUSB0
				Read the status of the LED light.
	
	modbus_client_simulator --debug -m rtu -a 1 -t 0x05 -r 0 -b 3 -p none /dev/ttyUSB0 {0xFF00|0x0000}
				Change the status of the LED light.
				
	modbus_client_simulator --debug -m rtu -a 1 -t 0x04 -r 0 -b 3 -p none /dev/ttyUSB0
				Read the temperature of CPU.
				
	modbus_client_simulator --debug -m rtu -a 1 -t 0x06 -r 0 -b 3 -p none /dev/ttyUSB0 6
				Change modbus_device_simulator slave address from 1 to 6.
				
	modbus_client_simulator --debug -m rtu -a 6 -t 0x06 -r 1 -b 3 -p none /dev/ttyUSB0 1
				Change modbus_device_simulator baud rate from 115200 to 9600.

### real-time-edge-modbus-simulator Test
Use two i.MX8MP boards to test the function of the modebus-simulator.
![10000001](_images\10000001.png)

    Note: Learn about the startup parameters before starting.

#### Test TCP functions
Read the status of the LED light on the board1.

	1. Enter board1 and start up modbus_device_simulator.
	modbus_device_simulator --debug -m tcp -p 1502 0.0.0.0
	
	2. Enter board2 and start up modbus_client_simulator.
	modbus_client_simulator --debug -m tcp -a 1 -t 0x01 -r 0 -p 1502 10.193.20.171

![10000002](_images\10000002.png)
	
#### Test RTU functions
Preparation: Connect the serial ports on the two boards, as shown below.
![10000003](_images\10000003.png)

The pin connection information of the two boards is as follows:

	- 5V  <---> 5V
	- GND <---> GND
	- TXD <---> RXD
	- RXD <---> TXD

Read the temperature of CPU on the board1.

	1. Enter board1 and start up modbus_device_simulator.
	modbus_device_simulator --debug -m rtu -a 1 -b 115200 -p none /dev/ttyUSB1
	
	2. Enter board2 and start up modbus_client_simulator.
	modbus_client_simulator --debug -m rtu -a 1 -t 0x04 -r 0 -b 3 -p none /dev/ttyUSB0
