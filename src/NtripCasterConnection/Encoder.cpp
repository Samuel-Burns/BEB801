/*
 * Encoder.cpp
 *
 *  Created on: 18/05/2017
 *      Author: Samuel Burns
 */

#include "Encoder.h"

Encoder::Encoder() {
	// TODO Auto-generated constructor stub

}

void Encoder::setInput(string username, string password){

	inputString = username + ":" + password;
	inputLength = inputString.length();
}

Encoder::~Encoder() {
	// TODO Auto-generated destructor stub
}
