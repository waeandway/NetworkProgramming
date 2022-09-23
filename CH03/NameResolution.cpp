#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdio.h>

#define TESTNAME "www.google.com"

/*------------------- Prototype --------------------*/
void err_display(char *msg);
BOOL GetIPAddr(char *name, IN_ADDR *addr);
BOOL GetDomainName(IN_ADDR addr, char *name, int namelen);


int main(int argc, char* argv[])
{
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2,2), &wsa)!=0)
       return 1;

    printf("도메인 (변환 전) : %s\n", TESTNAME);

    // Domain -> IP
    IN_ADDR addr;
    if(GetIPAddr(TESTNAME, &addr)) {
        printf("IP (변환 후 ) : %s\n", inet_ntoa(addr));
        char name[256];
        if(GetDomainName(addr, name, sizeof(name)) {
            printf("Domain (다시 변환) : %s\n", name);
        })
    }
}


// 소켓 함수 오류 출력
void err_display(char *msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL
    );
    printf("[%s] %s", msg, (char *)lpMsgBuf);
    LocalFree(lpMsgBuf);
}

// Domain -> IPv4
BOOl GetIPAddr(char *name, IN_ADDR *addr)
{
    HOSTENT *ptr = gethostbyname(name);
    if(ptr == NULL) {
        err_display("gethostbyname()");
        return FALSE;
    }
    if(ptr->h_addrtype != AF_INET)
        return FALSE;
    memcpy(addr, ptr->h_addr, ptr->h_length);
    return TRUE;
}

// IPv4 -> Domain 
BOOL GetDomainName(IN_ADDR addr, char *name, int namelen)
{
    HOSTNET *ptr = gethostbyaddr((char *)&addr, sizeof(addr), AF_INET);
    if(ptr == NULL) {
        err_display("gethostbyaddr()");
        return FALSE;
    }
    if(ptr->h_addrtype != AF_INET)
        return FALSE;
    strncpy(name, ptr->h_name, namelen);
    return TRUE;
}