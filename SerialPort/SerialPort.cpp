// SerialPort.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "SerialPort.h"
#include "ClassSerialPort.h"

ClassSerialPort mySerialPort;

MYDLL bool __stdcall InitialPort(unsigned int port,unsigned int baud,unsigned int dataBits,unsigned int stopBits)
{
	bool bResutl = true;
	bResutl = mySerialPort.InitPort(port,baud,dataBits,stopBits);
	if (!bResutl)
	{
		return false;
	}
	return bResutl;
}


MYDLL bool __stdcall WriteDataToPort(const char *data,unsigned int dataLength)
{
	bool bResult = true;
	bResult = mySerialPort.WriteData(data,dataLength);
	if (!bResult)
	{
		return false;
	}
	return bResult;
}


MYDLL bool __stdcall ReadDataFromPort(char *readData,int waitTime)
{
	bool bResult = true;
	bResult = mySerialPort.ReadData(readData,waitTime);
	if (!bResult)
	{
		return false;
	}
	return bResult;
}


MYDLL bool __stdcall ClosePort()
{
	bool bResult = true;
	bResult = mySerialPort.ClosePort();
	if (!bResult)
	{
		return false;
	}
	return true;
}
