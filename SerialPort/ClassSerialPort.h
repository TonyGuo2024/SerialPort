#pragma once
class ClassSerialPort
{
public:
	ClassSerialPort(void);
	~ClassSerialPort(void);
	bool InitPort(unsigned int port,unsigned int baud,unsigned int dataBits,unsigned int stopBits);
	bool WriteData(const char *data,unsigned int dataLength);
	bool ReadData(char *readData,int waitTime);
	bool ClosePort(void);
private:
		HANDLE m_hCom;
public:
	long GetMyCurrentTime(void);
};

