/*
 * Encoder.h
 *
 *  Created on: 18/05/2017
 *      Author: Samuel Burns
 */

using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/types.h>

#ifndef ENCODER_H_
#define ENCODER_H_


class Encoder {
public:
	Encoder();
	virtual ~Encoder();

	void setInput(string username, string password);

	virtual string encodeString() = 0;

protected:
	string outputString;
	string inputString;
	unsigned int inputLength;
};

#endif /* ENCODER_H_ */
