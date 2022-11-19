#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

char * ultimovalor(){
    char *result;
    char linha[10];

    FILE *arquivo;
    arquivo = fopen("logpontoB.txt", "rt");

    result = fgets(linha, 10, arquivo);

    fclose(arquivo);

    printf("valor lido: %s\n", result);

    return result;
}

void registrarValor(char* valor){
    FILE *arquivo;
    arquivo = fopen("logpontoB.txt", "wt");

    fprintf(arquivo, valor);

    fclose(arquivo);
}

void medicao(){
    int sensor = rand() % 100;
    sensor = sensor+1;
    char dados[10];

    itoa(sensor, dados, 10);
    
    registrarValor(dados);
}

void novamedicao(){
    char * ultimodado = ultimovalor();

    int sensor = rand() % 7;
    int dadosint = atoi(ultimodado);

    sensor = sensor - 3 + dadosint;

    if(sensor < 0)
        sensor == 0;
    
    printf("Nova Medição Sensor B: %d\n", sensor);
    char dados[10];

    itoa(sensor, dados, 10);
    
    registrarValor(dados);

    ultimodado = ultimovalor();
}

int main(int argc, char **argv) {
    char * result;
    
    printf("Medidor de vazão no ponto B:\n");
    
    medicao();

    novamedicao();

    return 0;
}