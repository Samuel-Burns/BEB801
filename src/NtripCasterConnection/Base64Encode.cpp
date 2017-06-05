/*
 * Base64Encode.cpp
 *
 *  Created on: 22/05/2017
 *      Author: Samuel Burns
 */

#include "Base64Encode.h"
#include <iostream>
#include <bitset>

const char* BASE_64_TABLE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const int INPUTBITMAX = 7; // 8 bits per input set
const int OUTPUTBITMAX = 5; // 6 bits per output set

Base64Encode::Base64Encode() {
	// TODO Auto-generated constructor stub

}

string Base64Encode::encodeString(){

	outputLength = calculateOutputLength();
	int intputBit = INPUTBITMAX, outputBit = OUTPUTBITMAX;
	unsigned int inputByte = 0, outputByte = 0;
	char tmpOutput[outputLength];
	cout << outputLength << " " << inputLength << endl;

	bitset<8> inputBitArray[inputLength];
	bitset<6> outputBitArray[outputLength];

	// Convert input string to bits
	for (size_t i = 0; i < inputLength; i++){;
		inputBitArray[i] = bitset<8>(inputString[i]);
	}

	// Regroup from 8 bits per set to 6 bits per set
	for (inputByte = 0; inputByte < inputLength; inputByte++){
		while(intputBit >= 0){
			if (outputBit < 0){
				outputBit = OUTPUTBITMAX;
				outputByte++;
			}
			outputBitArray[outputByte].set(outputBit, inputBitArray[inputByte].test(intputBit));
			intputBit--;
			outputBit--;
		}
		intputBit = INPUTBITMAX;
	}

	// Convert bits back to a string with padding
	if (inputLength % 3 == 1){
		for (size_t i = 0; i < outputLength-2; i++){
			tmpOutput[i] = BASE_64_TABLE[(int)outputBitArray[i].to_ulong()];
		}
		tmpOutput[outputLength-2] = '=';
		tmpOutput[outputLength-1] = '=';
	} else if (inputLength % 3 == 2){
		for (size_t i = 0; i < outputLength-1; i++){
			tmpOutput[i] = BASE_64_TABLE[(int)outputBitArray[i].to_ulong()];
		}
		tmpOutput[outputLength-1] = '=';
	} else {
	// Convert bits back to a string without padding
		for (size_t i = 0; i < outputLength; i++){
			tmpOutput[i] = BASE_64_TABLE[(int)outputBitArray[i].to_ulong()];
		}
	}

	return (string)tmpOutput;
}

unsigned int Base64Encode::calculateOutputLength(){

	int oL;
	int intputMod3 = inputLength % 3;

	// Check if input length can be factored by 3, pad if necessary
	if (intputMod3 != 0){
		oL = (4.0 / 3.0) * (inputLength - intputMod3) + 4;
	} else {
		oL = (4.0 / 3.0) * inputLength;
	}

	return oL;
}

Base64Encode::~Base64Encode() {
	// TODO Auto-generated destructor stub
}
