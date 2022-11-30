#include "..\..\Common.h"

#define PORTNAME _T("\\\\.\\COM10") 
#define BUFSIZE  512

int main(int argc, char *argv[])
{
	int retval;

	HANDLE hComm = CreateFile(PORTNAME, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, 0, NULL);
	if (hComm == INVALID_HANDLE_VALUE)
		err_quit("CreateFile()");

	DCB dcb;
	if (!GetCommState(hComm, &dcb))
		err_quit("GetCommState()");

	dcb.BaudRate = CBR_57600;
	dcb.ByteSize = 8;
	dcb.fParity = FALSE;
	dcb.StopBits = ONESTOPBIT;
	if (!SetCommState(hComm, &dcb))
		err_quit("SetCommState()");

	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	if (!SetCommTimeouts(hComm, &timeouts))
		err_quit("SetCommTimeouts()");

	char buf[BUFSIZE + 1];
	DWORD BytesRead, BytesWritten;

	while (1) {
		retval = ReadFile(hComm, buf, BUFSIZE, &BytesRead, NULL);
		if (retval == 0) err_quit("ReadFile()");

		buf[BytesRead] = '\0';
		printf("[���� ������] %s\n", buf);

		retval = WriteFile(hComm, buf, BUFSIZE, &BytesWritten, NULL);
		if (retval == 0) err_quit("WriteFile()");
	}

	CloseHandle(hComm);
	return 0;
}