/*
	PGA309.cpp - Library for communicating with PGA309.
	Created by Lucas Grzegorczyk, 05.03.2015
*/

#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include "PGA309.h"


/*	
	Default private constructor. Inaccessible by default.
	Initialises some variables.
*/
PGA309::PGA309(){
	testPinEnabled = false;
	recData[0] = 0;
	recData[1] = 0;	
	reg1 = 0x0000;
	reg2 = 0x0000;
	reg3 = 0x0000;
	reg4 = 0x0000;
	reg6 = 0x0000;
}


/*	
	Overloaded constructor. This one should be used.
*/
PGA309::PGA309(int address):PGA309::PGA309(){
	_address = address;
}

/*	
	Simple destructor.
*/
PGA309::~PGA309(){
	//nothing to clean up
}

/*	
	Function: enableTestPin
		Enables test pin on PGA309.
*/
void PGA309::enableTestPin(){
	if (!testPinEnabled){
		pinMode(TESTPIN, OUTPUT);
		digitalWrite(TESTPIN, HIGH);
		testPinEnabled = true;
	}
}

void PGA309::disableTestPin(){
	if (testPinEnabled){
		pinMode(TESTPIN, INPUT);
		testPinEnabled = false;
	}
}

/*	
	Function: write
		Writes 2 bytes of data to PGA309 register LSB first.
	Parameters:
		rgstr - register number (int)
		data - data to be written to the register (int)
	Returns:
		Integer - when different than 0 indicates error.
*/
int PGA309::write(int rgstr,  int data) {
	Wire.beginTransmission(_address);
	Wire.write(rgstr);
	Wire.write(data & 0xFF);	//LSB
	Wire.write(data >> 8);		//MSB
	return Wire.endTransmission();   
}

/*	
	Function: read
		Reads 2 bytes of data from PGA309 register and saves it to recData array.
	Parameters:
		rgstr - register number (int)
	Returns:
		Integer - when 0 indicates success, otherwise there was error.
*/
int PGA309::read(int rgstr) {
	int error;
	Wire.beginTransmission(_address);
	Wire.write(rgstr);
	if((error = Wire.endTransmission()) != 0)
		return error;
	Wire.requestFrom(_address, 2);
	int i = 0;
	while (Wire.available()){
		recData[i] = Wire.read();
		i++;
	}
}


/*
	Function: getAddress
		Get address on which PGA works.
	Returns:
		Integer - address of the PGA309
*/
int PGA309::getAddress(){
	return _address;
}


/*
	Function: getRecDeta
		Gets received data from the PGA 309.
	Parameters:
		Array to be modified and filled with data (byte[])
*/
void PGA309::getRecData(byte data[]){
	data[0] = recData[0];
	data[1] = recData[1];
}

/*
	Function: setRegisters
		Sets values of internal registers using given parameters.
	Parameters: 
		zero_dac - Zero DAC value (float)
		gain_dac - Gain DAC value (float)
		coarse_offset - Coarse offset, it will be mostly set to 0 (float)
		front_pga - Front-end Gain setting (float)
		out_pga - Output gain setting (float)
		v_ref - Reference voltage, I'm assuming it's internal one, default value is 4.096V (float)
*/
void PGA309::setRegisters(float zero_dac, float gain_dac, float coarse_offset, float front_pga, float out_pga, float v_ref /*= 4.096*/){
	reg1 = ceil(zero_dac/(v_ref/65536));
	reg2 = ceil((gain_dac - 1/3)*(3/2)*65536);
	reg4 = calcRegCoarseOffset(coarse_offset, v_ref);
	reg4 += calcRegFrontPGA(front_pga) << 8;
	reg4 += calcRegOutPGA(out_pga) << 12;
}

/*
	Function: calcRegCoarseOffset
		Calculates settings for the registers.
	Parameters: 
		coarse_offset - Coarse offset value (float)
		v_ref - Reference voltage (float)
	Returns:
		Integer - value needed to be written to the register
*/
int calcRegCoarseOffset(float coarse_offset, float v_ref){
	int step = coarse_offset / (v_ref * 0.00085);
	if(abs(step) > 0){
		if(abs(step) > 7){
			if(step > 0){
				return (step + 1);
			} else {
				return ((abs(step) + 1) | B10000);
			}
		} else {
			if(step > 0){
				return (step);
			} else {
				return (abs(step) | B10000);
			}
		}
	} else {
		return 0;
	}
}

/*
	Function: calcRegFrontPGA
		Calculates settings for the registers.
	Parameters: 
		front_pga - value set for front PGA
	Returns:
		Integer - value needed to be written to the register
*/
int calcRegFrontPGA(float front_pga){
	int ifront_pga = (int)front_pga;
	switch(ifront_pga){
		case 4:
			return 0;
			break;
		case 8:
			return 1;
			break;
		case 16:
			return 2;
			break;
		case 23:
			return 3;
			break;
		case 32:
			return 4;
			break;
		case 42:
			return 5;
			break;
		case 64:
			return 6;
			break;
		case 128:
			return 7;
			break;
		default:
			return 0;
	}
}

	if(coarse_offset != 0){
		//waiting for the answer from Ian, might look into National Instruments VIs
	}
}
