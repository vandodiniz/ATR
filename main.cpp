#include <iostream>
#include <string>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <vector>
#include <semaphore.h>
#include "uteis.cpp"

using namespace std;

#define NTHREADS 4

HANDLE semaforo_memoria;
HANDLE mutex;
vector<string> MEMORIA;

void envia_memoria(string mensagem){
    WaitForSingleObject( semaforo_memoria, INFINITE);
    MEMORIA.push_back(mensagem);
    cout << "Enviando mensagem do tipo "<< mensagem[7] << " Tamanho atual da memoria: "<< MEMORIA.size() << endl;
} 

unsigned __stdcall comunicacaoDeDados(void *arg){
    int NSEQ_otimizacao=0;
    int NSEQ_scada_processos=0;
    int NSEQ_scada_alarmes=0;
    string mensagem_otimizacao;
    string mensagem_scada_processo;
    string mensagem_scada_alarmes;
    while(1){
        //
        Sleep(1000);
        mensagem_otimizacao = gera_mensagem_otimizacao(&NSEQ_otimizacao);
        envia_memoria(mensagem_otimizacao);
        //
        Sleep(500);
        mensagem_scada_processo = gera_mensagem_scada_processo(&NSEQ_scada_processos);
        envia_memoria(mensagem_scada_processo);
        //
        Sleep(1000);
        mensagem_scada_alarmes = gera_mensagem_scada_alarmes(&NSEQ_scada_alarmes);
        envia_memoria(mensagem_scada_alarmes);
    }
    return 0;
} 

unsigned __stdcall retiradaDadosOtimizacao(void *arg){
    while(1){
        WaitForSingleObject(mutex, INFINITE);
        Sleep(1000);
        int tam = MEMORIA.size();
        for(int i=0; i<tam; i++){
            char status = MEMORIA[i][7];
            if (status=='1'){
                MEMORIA.erase(MEMORIA.begin()+i);
                cout << "Retirando mensagem do tipo "<< status << " Tamanho atual da memoria: "<< MEMORIA.size() << endl;
                break;
            }
        }
        ReleaseSemaphore(mutex, 1, NULL);
    }
    return 0;
} 

unsigned __stdcall retiradaDadosProcesso(void *arg){
    while(1){
        WaitForSingleObject(mutex, INFINITE);
        Sleep(1000);
        int tam = MEMORIA.size();
        for(int i=0; i<tam; i++){
            char status = MEMORIA[i][7];
            if (status=='2'){
                MEMORIA.erase(MEMORIA.begin()+i);
                cout << "Retirando mensagem do tipo "<< status << " Tamanho atual da memoria: "<< MEMORIA.size() << endl;
                break;
            }
        }
        ReleaseSemaphore(mutex, 1, NULL);
    }
    return 0;
}

unsigned __stdcall retiradaDadosAlarme(void *arg){
    while(1){
        WaitForSingleObject(mutex, INFINITE);
        Sleep(1000);
        int tam = MEMORIA.size();
        for(int i=0; i<tam; i++){
            char status = MEMORIA[i][7];
            if (status=='5'){
                MEMORIA.erase(MEMORIA.begin()+i);
                cout << "Retirando mensagem do tipo "<< status << " Tamanho atual da memoria: "<< MEMORIA.size() << endl;
                break;
            }
        }
        ReleaseSemaphore(mutex, 1, NULL);
    }
    return 0;
} 

int main(){
    HANDLE hThreads[NTHREADS];
    unsigned threadID[NTHREADS];

    semaforo_memoria = CreateSemaphore( NULL, 100, 100, NULL); 
        
    mutex = CreateSemaphore(NULL, 1, 1, NULL);          

    //Criaçao de threads
    printf( "Criando threads...\n" );
    hThreads[0] = (HANDLE)_beginthreadex( NULL, 0, &comunicacaoDeDados, NULL, 0, &threadID[0] );
    hThreads[1] = (HANDLE)_beginthreadex( NULL, 0, &retiradaDadosOtimizacao, NULL, 0, &threadID[1] );
    hThreads[2] = (HANDLE)_beginthreadex( NULL, 0, &retiradaDadosProcesso, NULL, 0, &threadID[2] );
    hThreads[3] = (HANDLE)_beginthreadex( NULL, 0, &retiradaDadosAlarme, NULL, 0, &threadID[3] );

    //Execução das threads
    WaitForMultipleObjects(NTHREADS, hThreads, TRUE, INFINITE);

    //Encerramento das threads
    for(int i = 0; i < NTHREADS; i++)
      CloseHandle(hThreads[i]);

    return 0;
}

