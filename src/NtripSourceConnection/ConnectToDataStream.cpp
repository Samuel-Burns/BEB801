/*
 * ConnectToDataStream.cpp
 *
 *  Created on: 24/05/2017
 *      Author: Samuel Burns
 */

#include "ConnectToDataStream.h"

#include <bitset>
#include <sstream>
#include <iostream>

const int MAX_DATA_SIZE = 1000;
const char* AGENT = "BEB801Client";

ConnectToDataStream::ConnectToDataStream() {
	// TODO Auto-generated constructor stub

}

ConnectToDataStream::ConnectToDataStream(string serverURL, SocketHandler* aNewStreamSocket, string mountPoint, string encodedCredentials){

	streamSocket = aNewStreamSocket;
	_buffer[MAX_DATA_SIZE] = '\0';

	_i = snprintf(_buffer, MAX_DATA_SIZE,
		"GET /%s HTTP/1.1\r\n"
		"Host: %s\r\n"
		"Ntrip-Version: Ntrip/2.0\r\n"
		"User-Agent: NTRIP %s/2.0\r\n"
		"Authorization: Basic %s\r\n"
		"Connection: close\r\n\r\n",
		mountPoint.c_str(), serverURL.c_str(), AGENT, encodedCredentials.c_str());

	if(_i > MAX_DATA_SIZE && _i < 0){
		fprintf(stderr, "Requested data too long\n");
		exit(1);
	}

	if(send(streamSocket->getSocketFD(), _buffer, _i, 0) != _i){
		perror("Message Failed.");
		exit(1);
	}

	if(send(streamSocket->getSocketFD(), _buffer, _i, 0) != _i){
		perror("send");
		exit(1);
	}
	CollectHeader();
}

void ConnectToDataStream::CollectHeader(){

	_numbytes = recv(streamSocket->getSocketFD(), _buffer, MAX_DATA_SIZE-1, 0);

	if (_numbytes != -1){
		cout << _buffer << endl;
	}
	memset(_buffer, '\0', MAX_DATA_SIZE);
}

string ConnectToDataStream::getLineFromStream(){

	string line = "";
	char tmp = '\0';
	int throwAway;

	while(recv(streamSocket->getSocketFD(), &tmp, 1, 0) != -1){

		if (tmp != '\r'){
			line += tmp;
		} else {
			// Remove \n
			throwAway = recv(streamSocket->getSocketFD(), &tmp, 1, 0);
			break;
		}
	}
	return line;
}

string ConnectToDataStream::BeginDataStream(){

	string header = "";
	string data = "";
	unsigned int chunkSize;

	header = getLineFromStream();

	if(header != ""){
		chunkSize = HexToInt(header);
		cout << chunkSize << " " << header << endl;
		data = readDataFromStream(chunkSize);
	} else {
		cout << "No data received." << endl;
		return "";
	}

	// Remove trailing \r\n\r\n
	header = readDataFromStream(4);

//	// Print check
	for(unsigned int j = 0; j < chunkSize; j++){
		cout << bitset<8>(data[j]).to_string() << endl;
	}

	return data;
}

string ConnectToDataStream::readDataFromStream(unsigned int chunkSize){

	string buffer2 = "";
	char buffer[chunkSize + 1];
	buffer[chunkSize] = '\0';

	_numbytes = recv(streamSocket->getSocketFD(), buffer, chunkSize-1, 0);

	return buffer2.append(buffer);
}

unsigned int ConnectToDataStream::HexToInt(string line){

	std::stringstream tmpStream;
	unsigned int chunkSize;

	tmpStream << std::hex << line;
	tmpStream >> chunkSize;

	return chunkSize;
}

ConnectToDataStream::~ConnectToDataStream() {
	// TODO Auto-generated destructor stub
}
