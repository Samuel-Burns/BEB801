/*
 * socketHandler.h
 *
 *  Created on: 16/05/2017
 *      Author: Samuel Burns
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#ifndef SOCKETHANDLER_H_
#define SOCKETHANDLER_H_

class SocketHandler {
public:

	// Constructors & destructors
	SocketHandler();
	SocketHandler(char* _address, int _port);
	virtual ~SocketHandler();

	// Functionality
	void initialiseConnection();
	void closeSocketHandler();

	// Set and Get
	int getSocketFD();

private:

	struct sockaddr_in sa;

	char *_address, *_user, *_password;
	int _SocketFD, _port, _res;

};

#endif /* SOCKETHANDLER_H_ */
