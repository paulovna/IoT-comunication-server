#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

#include <conio.h>

#include <winsock2.h>
#include <windows.h>

#include <ws2tcpip.h>

//Definicoes iniciais
#define DEFAULT_PORT "12000"
#define TAMANHO_BUFFER 1024
#define IP_SERVIDOR "127.0.0.1"

//Variáveis Globais do relogio
char hora [20] = "1";
char minuto [20] = "1";
char segundo[20] = "1";

int enviar = 0;

int atualizacaoClock()
{
    int aux = rand() % 5;
    aux = aux-2;
    return aux;
}

int atualizacaoCiclo()
{
    int aux = rand() % 4;
    aux = aux+7;
    aux = aux*1000;
    return aux;
}


void *Tclock(void *arg) 
{
    int ciclo;

    struct tm *atual; 
    time_t segundos;

    time(&segundos);
    atual = localtime(&segundos);
    int h = atual->tm_hour;//hora
    int m = atual->tm_min;//minuto
    int s = atual->tm_sec;//segundo  
    
    char horal [10];
    char minutol [10];
    char segundol [10];

    sprintf(segundol, "%d", s);
    sprintf(minutol, "%d", m);
    sprintf(horal, "%d", h);

    strcpy(hora, horal);
    strcpy(minuto, minutol);
    strcpy(segundo, segundol);

    while(1)
    {

        ciclo = atualizacaoCiclo(); 
        Sleep(ciclo);

        s = atualizacaoClock();

        if(s>=60)
        {
            s = s-60;
            m=m+1;
            if(m==60)
            {
                m=0;
                h=h+1;
                if(h==24)
                {
                    h=0;
                }
            }
        }

        else if(s<0)
        {
            s=60+s;
            m=m-1;
            if(m<0)
            {
                m=60+m;
                h=h-1;
                if(h<0)
                {
                    h=23;
                }
            }    
        }

        sprintf(segundol, "%d", s);
        sprintf(minutol, "%d", m);
        sprintf(horal, "%d", h);

        strcpy(hora, horal);
        strcpy(minuto, minutol);
        strcpy(segundo, segundol);

        enviar = 1;

    }
    pthread_exit(arg);
}


int medicao(){
    int sensor = rand() % 100;
    sensor = sensor + 1;
    return sensor;
}

int novamedicao(int ultimovalor){
        int sensor = rand() % 7;

        sensor = sensor - 3 + ultimovalor;

        if(sensor<0){
            sensor = 0;
        }
        if(sensor>100){
            sensor = 100;
        }

        return sensor;
    }

void *sensor(void *arg) {

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
    }

    printf("Tentando se conectar no servidor %s, porta %s", IP_SERVIDOR, DEFAULT_PORT);

    for (ptr=result; ptr!= NULL; ptr=ptr->ai_next){
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if(ConnectSocket == INVALID_SOCKET){
            printf("Socket error: %ld\n", WSAGetLastError() );
            WSACleanup();
            system("pause");
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
    }

    sprintf(sendBuffer, "B");

    iResult = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);

    if (iResult == SOCKET_ERROR) {
        printf("Falhou o envio com o erro: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        system("pause");
    }

    while (hora == "1" && minuto == "1" && segundo == "1")
    {
        continue;
    }

    int x = 0;

    int sensornum = medicao();

    char valor[10];
    sprintf(valor, "%d", sensornum);

    sprintf(sendBuffer, "\n%s:%s:%s*%s%%", hora, minuto, segundo, valor);
    printf("%s\n", sendBuffer);

    iResult = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);

    if (iResult == SOCKET_ERROR) {
        printf("Falhou o envio com o erro: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        system("pause");
    }

    while (x<10){
        if(enviar == 1)//So vai enviar o dado novamente depois que mudar o ciclo, controlado pela Thread Clock
        {
            sensornum = novamedicao(sensornum);

            sprintf(valor, "%d", sensornum);

            sprintf(sendBuffer, "\n%s:%s:%s*%s%%", hora, minuto, segundo, valor);

            iResult = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);

            if (iResult == SOCKET_ERROR) {
                printf("Falhou o envio com o erro: %d\n", WSAGetLastError());
                closesocket(ConnectSocket);
                WSACleanup();
                system("pause");
            }
            
            x = x+1;
            enviar = 0;
        }
    }

    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("Falhou encerrar conexao com o erro: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        system("pause");
    }
    return NULL;
    pthread_exit(arg);
}

void *Tsyncr(void *arg) {
    while (1){
        printf("\n%02s:%02s:%02s", hora, minuto, segundo);
        system("sleep 01");
    }
    


    return NULL;
    pthread_exit(arg);
}

int main(int argc, char **argv) {

    printf("Medidor de vazão no ponto B:\n");

    pthread_t t1, t2, t3;
    void *thread_res;
    char *teste[100];
    
    pthread_create(&t1, NULL, Tclock, (void *)(&teste));

    pthread_create(&t2, NULL, sensor, (void *)(&teste));

    pthread_join(t3, &thread_res);
    pthread_join(t1, &thread_res);
    pthread_join(t2, &thread_res);
    
    return 0;
}