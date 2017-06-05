//============================================================================
// Name        : BEB801_Ntrip_Client.cpp
// Author      : Samuel Burns
// Version     : 0.1
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <thread>

#include "NtripCasterConnection/FetchSourceTable.h"
#include "NtripCasterConnection/Base64Encode.h"
#include "NtripSourceConnection/ConnectToDataStream.h"
#include "NtripCasterConnection/SocketHandler.h"

#include "SourceTable.h"

using namespace std;

void encodeUserCredentials(string user, string pass, string* encoded, char encodeType);

int main() {

	// Geoscience Australia CORS data stream
	string SERVERURL = "auscors.ga.gov.au";
	char* IPADDRESS = "52.64.64.80";
	int PORT = 2101;

	// Login credentials
	string USERNAME = "qutUni";
	string PASSWORD = "KfqP3mBWS5";
	string encodedCredentials = "cXV0VW5pOktmcVAzbUJXUzU=";

	// Create caster socket
	SocketHandler casterSocket(IPADDRESS, PORT);

	// Fetch source table from AUSCORS Ntrip caster
	FetchSourceTable sourceTableRaw(SERVERURL, &casterSocket);
	casterSocket.closeSocketHandler();

	// Parse source table
	SourceTable ausCorsTable;

	// Search source table for different attributes
	vector<int> indices = ausCorsTable.getStreamsByNavSystem("QZS");
	vector<int> indices  = ausCorsTable.getStreamsByState("TAS");
	vector<int> indices  = ausCorsTable.getStreamsByCarrier(0);
	vector<int> indices  = ausCorsTable.getStreamsByNetwork("ARGN");

	// Connect to data streams
	int numConnections = indices.size();
	string tmp;

	// Select desired mountpoints
	string mountPoint[numConnections];
	SocketHandler streamSocket[numConnections];
	ConnectToDataStream testDatastream[numConnections];

	// Create connections and read in mountpoint headers
	for(int i = 0; i < numConnections; i++){
		mountPoint[i] = ausCorsTable.getDataStream(indices[i]).mountpoint;
		streamSocket[i] = SocketHandler(IPADDRESS, PORT);
		//encodeUserCredentials(USERNAME, PASSWORD, &encodedCredentials, ausCorsTable.getDataStream(NavIndexes[i]).authenticationType);
		testDatastream[i] = ConnectToDataStream(SERVERURL, &streamSocket[i], mountPoint[i], encodedCredentials);
	}

	// Read data streams
	for(int i = 0; i < numConnections; i++){
		for(int i = 0; i < numConnections; i++){
			tmp = testDatastream[i].BeginDataStream();
		}
	}

	// Close connections
	for(int i = 0; i < numConnections; i++){
		streamSocket[i].closeSocketHandler();
	}

	return 0;
}

void encodeUserCredentials(string user, string pass, string* encoded, char encodeType){

	if (encodeType == 'B'){
		// Encode credentials
		Base64Encode encoder;
		encoder.setInput(user, pass);
		*encoded = encoder.encodeString();
	} else if (encodeType == 'D'){
		// Digest encoding
		*encoded = "";
	} else if (encodeType == 'N'){
		*encoded = "";
	}
}



