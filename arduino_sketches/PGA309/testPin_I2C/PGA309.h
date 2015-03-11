/*
	PGA309.h - Library for communicating with PGA309.
	Created by Lucas Grzegorczyk, 05.03.2015
*/

#ifndef PGA309_h
#define PGA309_h

#define TESTPIN 0x34
#define ARRAY_SIZE 2

#include <Arduino.h>

class PGA309
{
	public:
		PGA309(int address);
		~PGA309();
		void enableTestPin();
		void disableTestPin();
		int writePGA309Register(int rgstr,  int data);
		int readPGA309Register(int rgstr);
		String binaryFormat(int n, int numOfPlaces);
		int getAddress();
		byte* getRecData();
	private:
		PGA309();
		int _address;
		bool testPinEnabled;
		byte recData[ARRAY_SIZE];
};

#endif