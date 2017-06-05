/*
 * SourceTable.h
 *
 *  Created on: 17/05/2017
 *      Author: Samuel Burns
 */

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <vector>

#ifndef SOURCETABLE_H_
#define SOURCETABLE_H_

// Navigation systems
enum navSystems {
	GPS = 1 << 0,
	GLO = 1 << 1,
	GAL = 1 << 2,
	BDS = 1 << 3,
	QZS = 1 << 4,
	QZSS = 1 << 5,
	SBAS = 1 << 6,
	MISC = 1 << 7,
	NONE = 1 << 8

};

// Contains information about a single CORS data stream
struct dataStream{
	string mountpoint;
	string identifier;
	string state;
	string format;
	string details;
	int carrier;
	navSystems navSystem;
	string network;
	string country;
	double latitude;
	double longitude;
	double distanceFromUser;
	int nmea;
	int solution;
	string generator;
	bool compression;
	char authenticationType;
	char fee;
	int bitRate;
	string misc;
};

class SourceTable {
	public:

		SourceTable();
		virtual ~SourceTable();

		// Functionality
		void parseHeader(string* headerData);
		void parseCaster(string* casterData);
		void parseNetworkRecords(string* etworkData);
		void parseDataStreams(string* streamData, int numStreams);
		string parseState(string ident);
		navSystems parseNavSystems(string navString);
		navSystems stringToNavSystem(string navSys);
		void calculateDistances(double longitude, double latitude);
		string* getSourceTableLine(fstream& tableStream, int dataLength);

		// Accessors
		dataStream getDataStream(int index);
		string getServerName();
		string getAccessDate();
		string getType();
		int getSourceTableLength();
		string getCasterString();

		// Search streams
		vector<int> getStreamsByNetwork(string corNetwork);
		vector<int> getStreamsByState(string tmpState);
		vector<int> getStreamsByCarrier(int carrierType);
		vector<int> getStreamsByNavSystem(string navSys);
		vector<int> getStreamsByDistance(double distance);
		void sortStreamsByDistance();

	private:

		// Network of datastreams
		vector<dataStream> dataStreams;

		// Ntrip caster details
		string serverName;
		string accessDate;
		string type;
		int sourceTableLength;
		string casterString;
		int numDataStreams;

};

#endif /* SOURCETABLE_H_ */
