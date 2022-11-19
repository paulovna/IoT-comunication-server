#include <stdio.h>
#include <winsock2.h>

int main(){
    char buff[129];
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
}