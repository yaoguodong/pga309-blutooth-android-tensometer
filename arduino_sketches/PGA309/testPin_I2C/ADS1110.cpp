/*
	ADS1110.cpp - Library for communicating with ADS1110.
	Created by Lucas Grzegorczyk, 05.03.2015
*/

#include <Arduino.h>
#include <Wire.h>
#include "ADS1110.h"

/*
	Default private constructor.
*/
ADS1110::ADS1110()
{
	recData[0] = 0;
	recData[1] = 0;
	recData[2] = 0;
}

/*
	Overloaded constructor.
*/
ADS1110::ADS1110(int address):ADS1110::ADS1110(){
	_address = address;
}

/*
	Function: read
		Reads from ADS110 everything available (data + setting register).
	Parameters:
		data - data written to register (byte)
	Retruns:
		Integer - 0 succesfull write, anything else error.
*/
int ADS1110::read(){
	int error;
	Wire.beginTransmission(_address);
	if((error = Wire.endTransmission()) != 0)
		return error;
	Wire.requestFrom(_address, 3);
	int i = 0;
	while (Wire.available()){
		recData[i] = Wire.read();
		i++;
	}
}

/*
	Function: write
		Writes to ADS1110 intenral register.
	Parameters:
		data - data written to register (byte)
	Retruns:
		Integer - 0 succesfull write, anything else error.
*/
int ADS1110::write(byte data){
	Wire.beginTransmission(_address);
	Wire.write(data);
	return Wire.endTransmission(); 
}

/*
	Function: getRecDeta
		Gets received data from the PGA 309.
	Parameters:
		Array to be modified and filled with data (byte[])
*/
void ADS1110::getRecData(byte data[]){
	data[0] = recData[0];
	data[1] = recData[1];
	data[2] = recData[2];
}