/*
 * Base64Encode.h
 *
 *  Created on: 22/05/2017
 *      Author: Samuel Burns
 */

#ifndef BASE64ENCODE_H_
#define BASE64ENCODE_H_

#include "Encoder.h"

class Base64Encode: public Encoder {

public:
	Base64Encode();
	virtual ~Base64Encode();

	unsigned int calculateOutputLength();

	string encodeString();

private:

	unsigned int outputLength;
};

#endif /* BASE64ENCODE_H_ */
