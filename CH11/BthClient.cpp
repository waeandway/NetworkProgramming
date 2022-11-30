#include "..\..\Common.h"
#include <ws2bth.h>
#include <initguid.h>
#include <locale.h>

#define SERVERADDR _T("E0:06:E6:AF:78:9F") 
#define BUFSIZE    512

DEFINE_GUID(BthServer_Service, 0x4672de25, 0x588d, 0x48af,
	0x80, 0x73, 0x5f, 0x2b, 0x7b, 0x0, 0x60, 0x1f);

int main(int argc, char *argv[])
{
	int retval;

	setlocale(LC_ALL, "");

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;


	WSAQUERYSET *qs = (WSAQUERYSET *)malloc(qslen);
	memset(qs, 0, qslen);
	qs->dwSize = qslen;
	qs->dwNameSpace = NS_BTH;
	qs->lpServiceClassId = (GUID *)&BthServer_Service;
	qs->lpszContext = (LPTSTR)SERVERADDR;
	DWORD flags = LUP_FLUSHCACHE | LUP_RETURN_ADDR;

	HANDLE hLookup;
	retval = WSALookupServiceBegin(qs, flags, &hLookup);
	if (retval == SOCKET_ERROR) {
		printf("[����] �߰ߵ� �������� ��ġ ����!\n");
		exit(1);
	}

	SOCKADDR_BTH *sa = NULL;
	int serverport = 0;
	bool done = false;
	while (!done) {
		retval = WSALookupServiceNext(hLookup, flags, &qslen, qs);
		if (retval == NO_ERROR) {
			sa = (SOCKADDR_BTH *)qs->lpcsaBuffer->RemoteAddr.lpSockaddr;
			serverport = sa->port;
			break;
		}
		else {
			if (WSAGetLastError() == WSAEFAULT) {
				free(qs);
				qs = (WSAQUERYSET *)malloc(qslen);
			}
			else {
				done = true;
			}
		}
	}
	if (sa == NULL) {
		_tprintf(_T("[����] %s���� ���� ���� ���� ����!\n"), SERVERADDR);
		exit(1);
	}

	WSALookupServiceEnd(hLookup);
	free(qs);

	SOCKET sock = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_BTH serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	int addrlen = sizeof(serveraddr);
	WSAStringToAddress((LPTSTR)SERVERADDR, AF_BTH, NULL,
		(struct sockaddr *)&serveraddr, &addrlen);
	serveraddr.port = serverport;
	retval = connect(sock, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	char buf[BUFSIZE + 1];
	int len;

	while (1) {
		printf("\n[���� ������] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;

		len = (int)strlen(buf);
		if (buf[len - 1] == '\n') buf[len - 1] = '\0';
		if (strlen(buf) == 0) break;

		retval = send(sock, buf, (int)strlen(buf), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		printf("[�������� Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);
	}

	closesocket(sock);

	WSACleanup();
	return 0;
}