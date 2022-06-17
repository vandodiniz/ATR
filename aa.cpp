#include<iostream>
#include<string>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <vector>

struct tm newtime;
__time32_t aclock;

//#include <semaphore.h>

using namespace std;

#define NTHREADS 2

vector<string> MEMORIA;
HANDLE semaforo_memoria;
int vagas_memoria = 5;

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

string gera_tempo(){
    string TIMESTAMP;
    time_t result = time(nullptr);
    string tempo = asctime(localtime(&result));
    for (int i = 11; i<19; i++)
        TIMESTAMP += tempo[i];
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
    DWORD dwWaitResult = WaitForSingleObject(
        semaforo_memoria,   // handle to semaphore
        0L);           // zero-second time-out interval

    switch (dwWaitResult)
    {
        // The semaphore object was signaled.
    case WAIT_OBJECT_0:
        MEMORIA.push_back(mensagem);
        vagas_memoria--;
        cout << MEMORIA.size() << endl;
    }
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
        Sleep((rand() % 5 + 1) * 1000);
        mensagem_otimizacao = gera_mensagem_scada_processo(&NSEQ_otimizacao);
        envia_memoria(mensagem_otimizacao);
        //
        Sleep(500);
        mensagem_scada_processo = gera_mensagem_scada_processo(&NSEQ_scada_processos);
        envia_memoria(mensagem_scada_processo);
        //
        Sleep((rand() % 5 + 1) * 1000);
        mensagem_scada_alarmes = gera_mensagem_scada_alarmes(&NSEQ_scada_alarmes);
        envia_memoria(mensagem_scada_alarmes);
    }
    return 0;
}

unsigned __stdcall teste(void* arg) {
    while (1) {
        Sleep(3000);
        cout << "oi" << endl;
    }
    return 0;
}

int main() {
    HANDLE hThreads[NTHREADS];
    unsigned threadID[2];


    semaforo_memoria = CreateSemaphore(
        NULL,           // default security attributes
        0,  // initial count
        100,  // maximum count
        NULL);          // unnamed semaphore
    //int status = sem_init(&semaforo_memoria, 0, 100); //sempre retorna 0

    //Criaçao de threads
    printf("Criando threads...\n");
    hThreads[0] = (HANDLE)_beginthreadex(NULL, 0, &comunicacaoDeDados, NULL, 0, &threadID[0]);
    hThreads[1] = (HANDLE)_beginthreadex(NULL, 0, &teste, NULL, 0, &threadID[1]);
    cout << "criou threads";

    //Execução das threads
    WaitForMultipleObjects(NTHREADS, hThreads, TRUE, INFINITE);

    //Encerramento das threads
    for (int i = 0; i < NTHREADS; i++)
        CloseHandle(hThreads[i]);

    return 0;
}
