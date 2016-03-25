#include "StdAfx.h"
#include "ClassSerialPort.h"
#include <Windows.h>
#include <stdio.h>
#include <ctime>
#include <math.h>



#define  RE_WRITE_TIMES			5
#define  READ_LENGHT			256
#define	 DELAY_TIME				1000

ClassSerialPort::ClassSerialPort(void)
{
}


ClassSerialPort::~ClassSerialPort(void)
{
	if (m_hCom != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hCom);
	}
}


/************************************************************************/
/*初始化串口                                                            */
/************************************************************************/
bool ClassSerialPort::InitPort(unsigned int port,unsigned int baud,unsigned int dataBits,unsigned int stopBits)
{
	char szPort[32] = {0};
	sprintf_s(szPort,"COM%d",port);

	/*打开指定的串口*/
	m_hCom = CreateFileA(szPort,						//设备名称COM1,COM2
						GENERIC_READ | GENERIC_WRITE,	//访问模式,可以读和写
						0,								//访问模式为0，表示不共享
						NULL,							//安全模式一般设置为NULL
						OPEN_EXISTING,					//该参数表示设备必须存在，否则创建失败
						0,								//0表示为同步，FILE_FLAG_OVERLAPPED表示为异步IO
						NULL);							//对串口而言，该参数必须为NULL

	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	BOOL bIsSuccess = true;
	//设置串口超时时间,均设为0表示不使用超时限制
	COMMTIMEOUTS CommimeOuts;
	CommimeOuts.ReadIntervalTimeout = 0;
	CommimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommimeOuts.ReadTotalTimeoutConstant = 0;
	CommimeOuts.WriteTotalTimeoutConstant = 0;
	CommimeOuts.WriteTotalTimeoutMultiplier = 0;

	bIsSuccess = SetCommTimeouts(m_hCom,&CommimeOuts);
	if (bIsSuccess == false)
	{
		return false;
	}

	/*配置串口的状态*/
	DCB dcb;
	bIsSuccess = GetCommState(m_hCom,&dcb);
	dcb.BaudRate = baud;//波特率
	dcb.ByteSize = dataBits;//每个字节的位数，一般的为8位
	dcb.Parity = NOPARITY;//无奇偶校验位
	dcb.StopBits = stopBits;//ONESTOPBIT;停止位
	bIsSuccess = SetCommState(m_hCom,&dcb);
	if (bIsSuccess == false)
	{
		return false;
	}

	/*清空串口缓冲区*/
	//PURGE_TXABORT :中断所有写操作并立即返回，即使写操作还没有完成
	//PURGE_RXABORT :中断所有读操作并立即返回，即使读操作还没有完成
	//PURGE_TXCLEAR : 清除输出缓冲区
	//PURGE_RXCLEAR : 清除输入缓冲区
	PurgeComm(m_hCom,PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	return true;
}

/************************************************************************
进行write serial port
************************************************************************/
bool ClassSerialPort::WriteData(const char *data,unsigned int dataLength)
{
	BOOL bResult = true;
	DWORD BytesToSend = 0;
	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	
	int i = 0;
	for (i=0; i<RE_WRITE_TIMES; i++)
	{
		bResult = WriteFile(m_hCom,data,dataLength,&BytesToSend,NULL);
		if(!bResult)
		{
			PurgeComm(m_hCom,PURGE_RXCLEAR | PURGE_RXABORT);
			return false;
		}
		else if (BytesToSend != dataLength)
		{
			PurgeComm(m_hCom,PURGE_RXCLEAR | PURGE_RXABORT);
			Sleep(DELAY_TIME);
			continue;
		}
		else
		{
			break;
		}
	}
	if (i >= RE_WRITE_TIMES)
	{
		return false;
	}

	return true;
}

/************************************************************************
进行串口数据的读取 waitTime是等待的时间
************************************************************************/
bool ClassSerialPort::ReadData(char *readData,int waitTime)
{
	BOOL bResult = true;
	DWORD BytesRead = 0;
	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	long currentTime = GetMyCurrentTime();
	long lastTime = 0;
	int diffTime = 0;
	COMSTAT ComStat;
	DWORD err;
	while(1)
	{
		ClearCommError(m_hCom,&err,&ComStat);
		if (ComStat.cbInQue <= 0)//检查缓冲区是否有数据可读
		{
			Sleep(DELAY_TIME);
			continue;
		}
		bResult = ReadFile(m_hCom,readData,READ_LENGHT,&BytesRead,NULL);
		if (!bResult)
		{
			lastTime = GetMyCurrentTime();
			diffTime = abs(lastTime - currentTime);
			if (diffTime > waitTime)
			{
				PurgeComm(m_hCom,PURGE_TXCLEAR | PURGE_TXABORT);
				return false;
			}
			Sleep(DELAY_TIME);
		}
	}

	return false;
}


bool ClassSerialPort::ClosePort(void)
{
	if (m_hCom != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hCom);
	}
	return true;
}


long ClassSerialPort::GetMyCurrentTime(void)
{
	time_t now_time;
	now_time = time(NULL);
	long currentTime = (long)now_time;
	return currentTime;
}
