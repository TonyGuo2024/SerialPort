#include "stdafx.h"

#define  MYDLL	extern "C" __declspec(dllexport)

MYDLL bool __stdcall InitialPort(unsigned int port,unsigned int baud,unsigned int dataBits,unsigned int stopBits);

MYDLL bool __stdcall WriteDataToPort(const char *data,unsigned int dataLength);

MYDLL bool __stdcall ReadDataFromPort(char *readData,int waitTime);

MYDLL bool __stdcall ClosePort();