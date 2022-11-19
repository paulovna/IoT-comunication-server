import random
from socket import *
from datetime import *
import random
import logging
from threading import *
import time
import _thread

import time

#Variáveis Globais do relógio (inteiros)
horas = 0
minutos = 0
segundos = 0

#Variáveis Globais do processo de sincronização
horasB = "0"
horasC = "0"

#Registro dos dados dos sensores B e C
def registrarValorA(valor):
    nomearquivo = "Logs\logpontoA.txt"
    with open(nomearquivo, "a") as arquivo:
        arquivo.write("\n"+valor)
def registrarValorB(valor):
    nomearquivo = "Logs\logpontoB.txt"
    with open(nomearquivo, "a") as arquivo:
        arquivo.write("\n"+valor)
def registrarValorC(valor):
    nomearquivo = "Logs\logpontoC.txt"
    with open(nomearquivo, "a") as arquivo:
        arquivo.write("\n"+valor)


def sync(connectionSocket):
    global segundos, minutos, horas, horasB, horasC
    if(horasB!="0" and horasC!="0"):      
        connectionSocket.sendall(str.encode("Its time to sync\n"))



def conexao(connectionSocket, cliente):
    global horasB, horasC, horas, minutos, segundos
    solicitacao = connectionSocket.recv(1024)
    solicitacao = solicitacao.decode().strip()
    name = solicitacao
    tempo = 1
    print(name)
    while True:
        solicitacao = connectionSocket.recv(1024)
        if (not solicitacao):
            break
        solicitacao = solicitacao.decode().strip()
        
        print("\nCliente: ",cliente, " Ponto: ", name, " Solicitação: ", solicitacao)

        #Se quiser conferir a diferença do tempo de recebimento dos dados

        if (tempo == 1):
            time_1 = time.time()
            tempo = tempo+1

        if (tempo == 2):
            time_2 = time.time()
            tempo = tempo+1
            time_interval = time_2 - time_1
            print("intervalo: ")
            print(time_interval)

        if(tempo >= 3):
            time_1 = time_2
            time_2 = time.time()
            time_interval = time_2 - time_1
            print("intervalo: ")
            print(time_interval)

        solicitsep = solicitacao.split('*')

        if(name == "B"):
            registrarValorB(solicitacao)     
            horasB = solicitsep[0]
        
        if(name == "C"):
            registrarValorC(solicitacao)     
            horasC = solicitsep[0]

        sync(connectionSocket)

    
    print('\nFinalizando conexao do cliente', cliente)
    connectionSocket.close()
    _thread.exit()

def thread_server():#Servidor TCP concorrente para recebimento dos dados
    global conectionlist
    serverName = "localhost" #IP
    serverPort = 12000 #porta
    serverSocket = socket(AF_INET,SOCK_STREAM)
    serverSocket.bind((serverName,serverPort))
    serverSocket.listen(1)

    print('O servidor está funcionando no IP: ', serverName, " na porta: ", serverPort)

    while True:
        #Conexao sendo aceita
        connectionSocket, cliente = serverSocket.accept()

        print('Conectado com '+str(cliente))

        # Abrindo uma nova thread para cada conexão
        _thread.start_new_thread(conexao, tuple([connectionSocket, cliente]))

    serverSocket.close()


def atualizacaoClock():
    aux = random.randint(0, 4)
    aux = aux-2
    return aux

def thread_clock():
    relogio = datetime.now()
    h = int(relogio.strftime("%H"))
    m = int(relogio.strftime("%M"))
    s = int(relogio.strftime("%S"))

    while True:
        ciclo = random.randint(7, 10)
        time.sleep(ciclo)
        s=s+atualizacaoClock()
        if(s>=60):
            s=s-60
            m=m+1
            if(m==60):
                m=0
                h=h+1
                if(h==24):
                    h=0
        elif(s<0):
            s=60+s
            m=m-1
            if(m<0):
                m=60+m
                h=h-1
                if(h<0):
                    h=23

        global segundos
        global minutos
        global horas

        segundos = s
        minutos = m
        horas = h


def definirHoraAtual():
    horaatual = ""
    global segundos
    global minutos
    global horas
    horaatual = horaatual+str(horas)
    horaatual = horaatual+":"
    horaatual = horaatual+str(minutos)
    horaatual = horaatual+":"
    horaatual = horaatual+str(segundos)

    return horaatual

def medicao(ultimovalor):
    sensor = random.randint(0, 7)
    sensor = sensor - 3 + ultimovalor

    if(sensor < 0):
        sensor = 0
    elif(sensor > 100):
        sensor = 100

    return sensor

def thread_sensor():
    ultimovalor = random.randint(1, 100)
    horaatual = definirHoraAtual()
    dados = horaatual+"*"+str(ultimovalor)+"%"
    registrarValorA(dados)
    time.sleep(1)

    while True:
        ultimovalor = medicao(ultimovalor)
        horaatual = definirHoraAtual()
        dados = horaatual+"*"+str(ultimovalor)+"%"+"\n"
        registrarValorA(dados)
        time.sleep(1)

def createlogs():
    nomearquivo = "Logs\logpontoA.txt"
    with open(nomearquivo, "w") as arquivo:
        arquivo.write("")
    arquivo.close()

    nomearquivo = "Logs\logpontoB.txt"
    with open(nomearquivo, "w") as arquivo:
        arquivo.write("")
    arquivo.close()

    nomearquivo = "Logs\logpontoC.txt"
    with open(nomearquivo, "w") as arquivo:
        arquivo.write("")
    arquivo.close()

def main():
    createlogs()

    #Subir Servidor de recebimento de dados
    t1 = Thread(target=thread_server, args=())
    t1.start()

    #Iniciando Thread do relógio
    t2 = Thread(target=thread_clock, args=())
    t2.start()

    #Iniciando Thread da geração de dados
    t3 = Thread(target=thread_sensor, args=())
    t3.start()
    
main()