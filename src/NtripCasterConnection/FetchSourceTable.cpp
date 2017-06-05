/*
 * FetchSourceTable.cpp
 *
 *  Created on: 18/05/2017
 *      Author: Samuel Burns
 */

#include "FetchSourceTable.h"

const char* AGENT2 = "BEB801Client";

FetchSourceTable::FetchSourceTable() {
	// TODO Auto-generated constructor stub

}

FetchSourceTable::FetchSourceTable(string serverURL, SocketHandler* aCasterSocket) {

	casterSocket = aCasterSocket;

	_i = snprintf(_buffer, MAX_DATA_SIZE,
	"GET / HTTP/1.1\r\n"
	"Host: $s\r\n"
	"Ntrip-Version: Ntrip/2.0\r\n"
	"User-Agent: NTRIP %s\r\n"
	"Connection: close\r\n\r\n"
	, serverURL, AGENT2);

	if(send(casterSocket->getSocketFD(), _buffer, _i, 0) != _i){
		perror("Message Failed.");
		exit(1);
	}

	downloadSourceTable();
}

void FetchSourceTable::downloadSourceTable(){

	 _tmpFile = fopen("SourceTable.txt", "w");

	 if (_tmpFile != NULL){
		while((_numbytes = recv(casterSocket->getSocketFD(), _buffer, MAX_DATA_SIZE-1, 0)) != -1){
			fwrite(_buffer, _numbytes, 1, _tmpFile);
			if(!strncmp("ENDSOURCETABLE\r\n", _buffer + _numbytes - 16, 16)){
				break;
			}
		}
	 }

	 fclose(_tmpFile);
}

FetchSourceTable::~FetchSourceTable() {
	// TODO Auto-generated destructor stub
}
