/*
 * socketHandler.cpp
 *
 *  Created on: 16/05/2017
 *      Author: Samuel Burns
 */

#include "SocketHandler.h"

SocketHandler::SocketHandler(void){
	// default constructor
}

SocketHandler::SocketHandler(char* address, int port) {

	_address = address;
	_port = port;

	initialiseConnection();
}

void SocketHandler::initialiseConnection(){

	_SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (_SocketFD == -1) {
		perror("Socket failed");
		exit(EXIT_FAILURE);
	}

	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(_port);
	_res = inet_pton(AF_INET, _address, &sa.sin_addr);

	// Connect returns 0 if no error is detected
	if (connect(_SocketFD, (struct sockaddr *)&sa, sizeof sa) == -1) {
		perror("Connection failed");
		close(_SocketFD);
		exit(EXIT_FAILURE);
	}
}

int SocketHandler::getSocketFD(){

	return _SocketFD;
}

void SocketHandler::closeSocketHandler(){
	close(_SocketFD);
}

SocketHandler::~SocketHandler() {
	// TODO Auto-generated destructor stub
}
