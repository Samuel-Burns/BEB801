/*
 * FetchSourceTable.h
 *
 *  Created on: 18/05/2017
 *      Author: Samuel Burns
 */

using namespace std;

#include <string>
#include <time.h>
#include "socketHandler.h"

#ifndef FETCHSOURCETABLE_H_
#define FETCHSOURCETABLE_H_

const int MAX_DATA_SIZE = 1000;

class FetchSourceTable {
	public:
		FetchSourceTable();
		FetchSourceTable(string serverURL, SocketHandler* aCasterSocket);
		void downloadSourceTable();
		virtual ~FetchSourceTable();

	private:
		int _i, _numbytes;
		char _buffer[MAX_DATA_SIZE];
		FILE * _tmpFile;
		SocketHandler* casterSocket;

};

#endif /* FETCHSOURCETABLE_H_ */
