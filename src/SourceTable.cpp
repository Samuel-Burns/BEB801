/*
 * SourceTable.cpp
 *
 *  Created on: 17/05/2017
 *      Author: Samuel Burns
 */

#include "SourceTable.h"
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <math.h>

const string HEADER_DELIMITER = ": ";
const char STREAM_DELIMITER = ';';
const char* SOURCE_TABLE_NAME = "SourceTable.txt";

const double RADIUS_OF_EARTH = 6371.0;

// These values are determined by the Ntrip standard (v2.0)
const int HEADER_LENGTH = 8;
const int NUM_PARAMS_PER_STREAM = 19;
const int NUM_NAV_SYSTEMS = 8;
const string DATA_TYPE_CASTER = "CAS;";
const string DATA_TYPE_NETWORK = "NET;";
const string DATA_TYPE_STREAM = "STR;";

SourceTable::SourceTable() {

	fstream tableStream(SOURCE_TABLE_NAME);
	string lineContainer;
	int numLines = 0, numCasters = 0, numNetworks = 0, numStreams = 0; // unsure why numStreams comes up 1 short

	// Count the number of lines in the file
	if (tableStream.is_open()){
		while(getline(tableStream, lineContainer)){
			if(lineContainer.find(DATA_TYPE_CASTER) != string::npos){
				numCasters++;
			}if(lineContainer.find(DATA_TYPE_NETWORK) != string::npos){
				numNetworks++;
			}if(lineContainer.find(DATA_TYPE_STREAM) != string::npos){
				numStreams++;
			}
			numLines++;
		}
	}else{
		cout << "File failed to open." << endl;
	}

	numDataStreams = numStreams;

	//cout << numLines <<" "<< numCasters <<" "<< numNetworks <<" "<< numStreams << endl;

	// Reset stream
	tableStream.close();
	tableStream.clear();
	tableStream.open(SOURCE_TABLE_NAME);

	// Parse segments
	parseHeader(getSourceTableLine(tableStream, HEADER_LENGTH));
	getline(tableStream, lineContainer); // Remove blank line between header and data
	parseCaster(getSourceTableLine(tableStream, numCasters));
	parseNetworkRecords(getSourceTableLine(tableStream, numNetworks));
	parseDataStreams(getSourceTableLine(tableStream, numStreams), numStreams);

	tableStream.close();
}

void SourceTable::parseHeader(string* headerData){

	// Parse header
	string headerName(headerData[3]);
	string headerDate(headerData[4]);
	string headerTyper(headerData[6]);
	string tableLength(headerData[7]);

	serverName =  headerName.erase(0, headerName.find(HEADER_DELIMITER) + HEADER_DELIMITER.length());
	accessDate = headerDate.erase(0, headerDate.find(HEADER_DELIMITER) + HEADER_DELIMITER.length());
	type = headerTyper.erase(0, headerTyper.find(HEADER_DELIMITER) + HEADER_DELIMITER.length());
	sourceTableLength = atoi(tableLength.erase(0, tableLength.find(HEADER_DELIMITER) + HEADER_DELIMITER.length()).c_str());

	//cout << serverName << accessDate << type << sourceTableLength << endl;
}

void SourceTable::parseCaster(string* casterData){

	// Parse caster @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// Can an ntrip system have more than one caster?
	casterString = casterData[0].erase(0, casterData[0].find(DATA_TYPE_CASTER) + DATA_TYPE_CASTER.length());
	//cout << casterString << endl;
}

void SourceTable::parseNetworkRecords(string* networkData){

	// Deal with later

}

void SourceTable::parseDataStreams(string* streamData, int numStreams){

	dataStream tmpStream;
	string dataLine[NUM_PARAMS_PER_STREAM];
	int position;

	for (int index = 0; index < numStreams; index++){

		for(int index2 = 0; index2 < NUM_PARAMS_PER_STREAM; index2++){
			position = streamData[index].find(STREAM_DELIMITER);
			dataLine[index2] = streamData[index].substr(0, position);
			streamData[index].erase(0, position + 1);
		}

		tmpStream.mountpoint = dataLine[1];
		tmpStream.identifier = dataLine[2];
		tmpStream.state = parseState(dataLine[2]);
		tmpStream.format = dataLine[3];
		tmpStream.details = dataLine[4];
		tmpStream.carrier = atoi(dataLine[5].c_str());
		tmpStream.navSystem = parseNavSystems(dataLine[6]);
		tmpStream.network = dataLine[7];
		tmpStream.country = dataLine[8];
		tmpStream.latitude = atof(dataLine[9].c_str());
		tmpStream.longitude = atof(dataLine[10].c_str());
		tmpStream.nmea = atoi(dataLine[11].c_str());
		tmpStream.solution = atoi(dataLine[12].c_str());
		tmpStream.generator = dataLine[13];
		// skip compression type -> dataLine[14]
		tmpStream.authenticationType = *dataLine[15].c_str();
		tmpStream.fee = *dataLine[16].c_str();
		tmpStream.bitRate = atoi(dataLine[17].c_str());
		tmpStream.misc = dataLine[18];

		dataStreams.push_back(tmpStream);
		//cout << tmpStream.network << endl;
	}
}

string SourceTable::parseState(string ident){

	if (ident.find('(') != std::string::npos){
		unsigned first = ident.find('(');
		unsigned last = ident.find(')');

		return (ident.substr(first+1,last-first-1));
	} else {
		return ("none");
	}
}

navSystems SourceTable::parseNavSystems(string navString){

	navSystems tmp;
	int position;
	int  numSystems = 1 + count(navString.begin(), navString.end(), '+');

	//cout << navString << endl;

	if (numSystems == 1){
		return stringToNavSystem(navString);
	} else {
		for(int index = 0; index < numSystems; index++){
			position = navString.find('+');
			tmp = (navSystems)(tmp | (stringToNavSystem(navString.substr(0, position))));
			//cout << " " << navString.substr(0, position) << endl;
			navString.erase(0, position + 1);
		}
		return tmp;
	}
}

navSystems SourceTable::stringToNavSystem(string navSys){

	if (!navSys.compare("GPS")){
		return GPS;
	}else if (!navSys.compare("GLO")){
		return GLO;
	}else if (!navSys.compare("GAL")){
		return GAL;
	}else if (!navSys.compare("BDS")){
		return BDS;
	}else if (!navSys.compare("QZS")){
		return QZS;
	}else if (!navSys.compare("QZSS")){
		return QZSS;
	}else if (!navSys.compare("SBAS")){
		return SBAS;
	}else if (!navSys.compare("MISC")){
		return MISC;
	} else {
		return NONE;
	}
}

// Uses Haversine formula
void SourceTable::calculateDistances(double longitude, double latitude){

	double deltLat;
	double deltLong;

	for (int i = 0; i < numDataStreams; i++){
		deltLat = dataStreams[i].latitude - latitude;
		deltLong = dataStreams[i].longitude - longitude;

		dataStreams[i].distanceFromUser = 2.0 * RADIUS_OF_EARTH
											* asin(pow(sin(deltLat / 2.0), 2.0)
											+ cos(dataStreams[i].latitude) * cos(latitude)
											* pow(sin(deltLong / 2.0), 2.0));
	}
}

string* SourceTable::getSourceTableLine(fstream& tableStream, int dataLength){

	string* tableData = new string[dataLength];
	int index;

	// Extract data line by line
	for (index = 0; index < dataLength; index++){
		getline(tableStream, tableData[index]);
	}

	return tableData;
}

vector<int> SourceTable::getStreamsByState(string tmpState){

	vector<int> indexes;

	for (int i = 0; i < numDataStreams; i++){
		if (!dataStreams[i].state.compare(tmpState)){
			indexes.push_back(i);
		}
	}

	// If nothing is found insert value greater than number of streams
	if (indexes.empty()){
		indexes.push_back(numDataStreams + 1);
	}

	return indexes;
}

vector<int> SourceTable::getStreamsByCarrier(int carrierType){

	vector<int> indexes;

	for (int i = 0; i < numDataStreams; i++){
		if (dataStreams[i].carrier == carrierType){
			indexes.push_back(i);
		}
	}

	// If nothing is found insert value greater than number of streams
	if (indexes.empty()){
		indexes.push_back(numDataStreams + 1);
	}

	return indexes;
}

vector<int> SourceTable::getStreamsByNavSystem(string navSys){

	vector<int> indexes;

	for (int i = 0; i < numDataStreams; i++){
		if (dataStreams[i].navSystem & stringToNavSystem(navSys)){
			indexes.push_back(i);
		}
	}

	// If nothing is found insert value greater than number of streams
	if (indexes.empty()){
		indexes.push_back(numDataStreams + 1);
	}

	return indexes;
}

vector<int> SourceTable::getStreamsByNetwork(string corNetwork){

	vector<int> indexes;

	for (int i = 0; i < numDataStreams; i++){
		if (!dataStreams[i].network.compare(corNetwork)){
			indexes.push_back(i);
		}
	}

	// If nothing is found insert value greater than number of streams
	if (indexes.empty()){
		indexes.push_back(numDataStreams + 1);
	}

	return indexes;
}

// Must use calculateDistances(double longitude, double latitude) first
vector<int> SourceTable::getStreamsByDistance(double distance){

	vector<int> indexes;

	for (int i = 0; i < numDataStreams; i++){
		if (dataStreams[i].distanceFromUser <= distance){
			indexes.push_back(i);
		}
	}

	// If nothing is found insert value greater than number of streams
	if (indexes.empty()){
		indexes.push_back(numDataStreams + 1);
	}

	return indexes;
}


// Must use calculateDistances(double longitude, double latitude) first
void SourceTable::sortStreamsByDistance(){

	bool sFlag = true;
	int count = numDataStreams;
	dataStream tmp;

	// Bubble sort index values based on distances
	while(sFlag){
		sFlag = false;
		for(int i = 0; i < count; i++){
			if (dataStreams[i + 1].distanceFromUser < dataStreams[i].distanceFromUser){
				sFlag = true;
				tmp = dataStreams[i + 1];
				dataStreams[i + 1] = dataStreams[i];
				dataStreams[i] = tmp;
			}
		}
		count--;
	}
}

dataStream SourceTable::getDataStream(int index){
	return dataStreams[index];
}

string SourceTable::getServerName(){
	return serverName;
}

string SourceTable::getAccessDate(){
	return accessDate;
}

string SourceTable::getType(){
	return type;
}

int SourceTable::getSourceTableLength(){
	return sourceTableLength;
}

string SourceTable::getCasterString(){
	return casterString;
}

SourceTable::~SourceTable() {
	// TODO Auto-generated destructor stub
}
