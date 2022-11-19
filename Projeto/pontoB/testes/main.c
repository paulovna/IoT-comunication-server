#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <conio.h>

#include <winsock2.h>
#include <windows.h>

#include <ws2tcpip.h>

#define DEFAULT_PORT "12000"
#define TAMANHO_BUFFER 1024
#define IP_SERVIDOR "127.0.0.1"

int main(int argc, char **argv) {

    printf("Medidor de vazão no ponto B:\n");

    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    char sendBuffer[TAMANHO_BUFFER];

    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

    char hostname[NI_MAXHOST];
    gethostname(hostname,NI_MAXHOST);
    printf("\n Cliente: %s\n", hostname);


    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    iResult = getaddrinfo(IP_SERVIDOR, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
    printf("getaddrinfo failed: %d\n", iResult);
    WSACleanup();
    return 1;
    }

    printf("Tentando se conectar no servidor %s, porta %s", IP_SERVIDOR, DEFAULT_PORT);

    for (ptr=result; ptr!= NULL; ptr=ptr->ai_next){
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if(ConnectSocket == INVALID_SOCKET){
            printf("Socket error: %ld\n", WSAGetLastError() );
            WSACleanup();
            system("pause");
            return 1;
        }

        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
            if(iResult == SOCKET_ERROR){
                closesocket(ConnectSocket);
                ConnectSocket = INVALID_SOCKET;
                continue;
            }

        break;

    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET){
        printf("Não foi possivel conectar ao servidor\n");
        system("pause");
        WSACleanup();
        return 1;
    }

    int i=0;
    while (!kbhit()){
        printf("Enviando \"Sou o cliente %s e envio o recado no. %04d\"...\n", hostname,i);

        sprintf(sendBuffer, "Sou o cliente %s e envio o recado no. %04d\"...\n", hostname,i);

        iResult = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);

        if (iResult == SOCKET_ERROR) {
            printf("Falhou o envio com o erro: %d\n", WSAGetLastError());
            closesocket(ConnectSocket);
            WSACleanup();
            system("pause");
            return 1;
        }
        i++;
        Sleep(1000);
    }

    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("Falhou encerrar conexao com o erro: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        system("pause");
        return 1;
    }
    return 0;
}