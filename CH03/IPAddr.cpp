#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    WSADATA wsa;
    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
       return 1;

    /* IPv4 변환 */
    /* 기존 IP */
    char *ipv4test = "147.46.114.70";
    printf("IPv4 변환 전 : %s\n", ipv4test);
    
    printf("IPv4 변환 후 : 0x%x\n", inet_addr(ipv4test));

    IN_ADDR ipv4num;
    ipv4num s_addr = inet_addr(ipv4test);
    printf("IPv4 다시 변환 : %s\n", inet_ntoa(ipv4num));

    printf("\n");

    /* IPv6 변환 */
    /* 기존 IP */
    char *ipv6test = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";
    printf("IPv6 변환 전 = %s\n", ipv6test);

    SOCKADDR_IN6 ipv6num;
    int addrlen = sizeof(ipv6num);
    WSAStringToAddress(ipv6test, AF_INET6, NULL, (SOCKADDR*)&ipv6num, addrlen);
    printf("IPv6 변환 후 = 0x");
    for(int i=0; i<16; i++)
       printf("%02x", ipv6num.sin6_addr.u.Byte[i]);
    printf("\n");

    char ipaddr[50];
    DWORD ipaddr[50];
    DWORD ipaddrlen = sizeof(ipaddr);
    WSAAddressToString((SOCKADDR*)&ipv6num, sizeof(ipv6num), NULL, ipaddr, &ipaddrlen);
    printf("IPv6 변환 후 = %s\n", ipaddr);

    WSACleanup();
    return 0;
}