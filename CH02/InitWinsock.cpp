/* 소켓 생성과 닫기 */
#pragma comment(lib, 'ws2_32')
#include <winsock2.h>

voit err_equit(char *msg);

int main(int arcg, char*argv[])
{
    // 윈속 초기화 
    WSADATA wsa;
    if(WSAStartUp(MAKEWORD(2,2), &wsa)!=0)
      return 1;
    MessageBox(NULL, "Windows Socket init", "Notice", MB_OK);

    // socket()
    SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(tcp_sock == INVALID_SOCKET) err_quit("socket()");
    MessageBox(NULL, "TCP Socket init", "Notice", MB_OK);

    // closesocket()
    closesocket(tcp_sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}

//소켓 함수 오류 출력 후 종료 
void err_quit(char *msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, 
            NULL, WSAGetLastError(), 
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&lpMsgBuf, 0, NULL);
        MessageBox(NULL, (LPCTSTR)lpMsgBuf), msg, MB_ICONERROR);
        LocalFree(lpMsgBuf);
        exit(1);
}