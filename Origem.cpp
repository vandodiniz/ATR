#include<iostream>
#include<string>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <vector>
#include <semaphore.h>
#include <sysinfoapi.h>

using namespace std;

#define NTHREADS 4

HANDLE semaforo_memoria;
HANDLE mutex;
vector<string> MEMORIA;

void incrementa_contador(int* contador) {
    *contador += 1;

    if (*contador > 999999)
        *contador = 1;
}

string gera_NSEQ(int contador) {
    string NSEQ;
    string contador_string = to_string(contador);
    for (int i = 0; i < 6 - contador_string.length(); i++)
        NSEQ += '0';
    NSEQ += contador_string;
    return NSEQ;
}

string gera_N(int tamanho) {
    int num;
    string minimo;
    string N;

    for (int i = 0; i < tamanho - 1; i++)
        minimo += "0";

    for (int i = 0; i < tamanho; i++) {
        if (N == minimo)
            num = rand() % 9 + 1;
        else
            num = rand() % 10;
        N += to_string(num);
    }
    return N;
}

string gera_NNNNpN() {
    int num;
    string NNNNpN;
    for (int i = 0; i < 6; i++) {
        if (i == 4)
            NNNNpN += ".";
        else {
            num = rand() % 10;
            NNNNpN += to_string(num);
        }
    }
    return NNNNpN;
}

string gera_tempo() {
    SYSTEMTIME st = { 0 };

    GetLocalTime(&st);

    string TIMESTAMP;
    TIMESTAMP += to_string(st.wHour);
    TIMESTAMP += ":";
    TIMESTAMP += to_string(st.wMinute);
    TIMESTAMP += ":";
    TIMESTAMP += to_string(st.wSecond % 02);
    return TIMESTAMP;
}
string gera_mensagem_otimizacao(int* contador) {

    incrementa_contador(contador);

    string NSEQ = gera_NSEQ(*contador);
    string TIPO = "11";
    string SP_PRESS = gera_NNNNpN();
    string SP_TEMP = gera_NNNNpN();
    string VOL = gera_N(5);
    string TIMESTAMP = gera_tempo();

    return NSEQ + "|" + TIPO + "|" + SP_PRESS + "|" + SP_TEMP + "|" + VOL + "|" + TIMESTAMP;
}

string gera_mensagem_scada_processo(int* contador) {

    incrementa_contador(contador);

    string NSEQ = gera_NSEQ(*contador);
    string TIPO = "22";
    string PRESSAO_T = gera_NNNNpN();
    string TEMP = gera_NNNNpN();
    string PRESSAO_G = gera_NNNNpN();
    string NIVEL = gera_NNNNpN();
    string TIMESTAMP = gera_tempo();

    return NSEQ + "|" + TIPO + "|" + PRESSAO_T + "|" + TEMP + "|" + PRESSAO_G + "|" + NIVEL + "|" + TIMESTAMP;
}

string gera_mensagem_scada_alarmes(int* contador) {

    incrementa_contador(contador);

    string NSEQ = gera_NSEQ(*contador);
    string TIPO = "55";
    string ID = gera_N(4);
    string PRIORIDADE = gera_N(3);
    string TIMESTAMP = gera_tempo();

    return NSEQ + "|" + TIPO + "|" + ID + "|" + PRIORIDADE + "|" + TIMESTAMP;
}

void envia_memoria(string mensagem) {
    WaitForSingleObject(semaforo_memoria, INFINITE);
    MEMORIA.push_back(mensagem);
    cout << "Enviando mensagem do tipo " << mensagem[7] << " Tamanho atual da memoria: " << MEMORIA.size() << endl;
}

unsigned __stdcall comunicacaoDeDados(void* arg) {
    int NSEQ_otimizacao = 0;
    int NSEQ_scada_processos = 0;
    int NSEQ_scada_alarmes = 0;
    string mensagem_otimizacao;
    string mensagem_scada_processo;
    string mensagem_scada_alarmes;
    while (1) {
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

unsigned __stdcall retiradaDadosOtimizacao(void* arg) {
    while (1) {
        WaitForSingleObject(mutex, INFINITE);
        Sleep(1000);
        int tam = MEMORIA.size();
        for (int i = 0; i < tam; i++) {
            char status = MEMORIA[i][7];
            if (status == '1') {
                MEMORIA.erase(MEMORIA.begin() + i);
                cout << "Retirando mensagem do tipo " << status << " Tamanho atual da memoria: " << MEMORIA.size() << endl;
                break;
            }
        }
        ReleaseSemaphore(mutex, 1, NULL);
    }
    return 0;
}

unsigned __stdcall retiradaDadosProcesso(void* arg) {
    while (1) {
        WaitForSingleObject(mutex, INFINITE);
        Sleep(1000);
        int tam = MEMORIA.size();
        for (int i = 0; i < tam; i++) {
            char status = MEMORIA[i][7];
            if (status == '2') {
                MEMORIA.erase(MEMORIA.begin() + i);
                cout << "Retirando mensagem do tipo " << status << " Tamanho atual da memoria: " << MEMORIA.size() << endl;
                break;
            }
        }
        ReleaseSemaphore(mutex, 1, NULL);
    }
    return 0;
}

unsigned __stdcall retiradaDadosAlarme(void* arg) {
    while (1) {
        WaitForSingleObject(mutex, INFINITE);
        Sleep(1000);
        int tam = MEMORIA.size();
        for (int i = 0; i < tam; i++) {
            char status = MEMORIA[i][7];
            if (status == '5') {
                MEMORIA.erase(MEMORIA.begin() + i);
                cout << "Retirando mensagem do tipo " << status << " Tamanho atual da memoria: " << MEMORIA.size() << endl;
                break;
            }
        }
        ReleaseSemaphore(mutex, 1, NULL);
    }
    return 0;
}

int main() {
    HANDLE hThreads[NTHREADS];
    unsigned threadID[NTHREADS];

    semaforo_memoria = CreateSemaphore(NULL, 100, 100, NULL);

    mutex = CreateSemaphore(NULL, 1, 1, NULL);

    //Cria�ao de threads
    printf("Criando threads...\n");
    hThreads[0] = (HANDLE)_beginthreadex(NULL, 0, &comunicacaoDeDados, NULL, 0, &threadID[0]);
    hThreads[1] = (HANDLE)_beginthreadex(NULL, 0, &retiradaDadosOtimizacao, NULL, 0, &threadID[1]);
    hThreads[2] = (HANDLE)_beginthreadex(NULL, 0, &retiradaDadosProcesso, NULL, 0, &threadID[2]);
    hThreads[3] = (HANDLE)_beginthreadex(NULL, 0, &retiradaDadosAlarme, NULL, 0, &threadID[3]);

    //Execu��o das threads
    WaitForMultipleObjects(NTHREADS, hThreads, TRUE, INFINITE);

    //Encerramento das threads
    for (int i = 0; i < NTHREADS; i++)
        CloseHandle(hThreads[i]);

    return 0;
}