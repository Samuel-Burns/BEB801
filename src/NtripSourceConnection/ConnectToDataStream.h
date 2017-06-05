/*
 * ConnectToDataStream.h
 *
 *  Created on: 24/05/2017
 *      Author: Samuel Burns
 */

using namespace std;

#include <string>
#include "../NtripCasterConnection/SocketHandler.h"

#ifndef CONNECTTODATASTREAM_H_
#define CONNECTTODATASTREAM_H_

class ConnectToDataStream {
	public:
		// Constructors and Destructor
		ConnectToDataStream();
		ConnectToDataStream(string serverURL, SocketHandler* aNewStreamSocket, string mountPoint, string encodedCredentials);
		virtual ~ConnectToDataStream();

		// Functionality
		void CollectHeader();
		string getLineFromStream();
		string BeginDataStream();
		string readDataFromStream(unsigned int chunkSize);
		unsigned int HexToInt(string line);

	private:

		int _i, _numbytes;
		char _buffer[1000];
		FILE* _tmpFile;
		SocketHandler* streamSocket;

};

#endif /* CONNECTTODATASTREAM_H_ */
