#include <iostream>
#include <string>
#include <time.h>
#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <vector>
#include <sysinfoapi.h>
#include <conio.h>

using namespace std;

#define NTHREADS    8
#define	ESC			0x1B
#define TeclaC      0x63
#define TeclaO      0x6F
#define TeclaP	    0x70
#define	TeclaA		0x61
#define TeclaT      0x74
#define TeclaR      0x72
#define TeclaL	    0x6C
#define TeclaZ      0x7A

HANDLE HevCDD;
HANDLE HevRDO;
HANDLE HevRDP;
HANDLE HevRDA;
HANDLE HevEDO;
HANDLE HevEDP;
HANDLE HevEDA;
HANDLE HevLimparConsoleEDA;

BOOL comunicacaoDeDadosLigado = 1;
BOOL retiradaDadosOtimizacaoLigado = 1;
BOOL retiradaDadosProcessoLigado = 1;
BOOL retiradaDadosAlarmesLigado = 1;
BOOL exibicaoDadosOtimizacaoLigado = 1;
BOOL exibicaoDadosProcessoLigado = 1;
BOOL exibicaoDadosAlarmesLigado = 1;

int  tecla = 0;      // Caracter digitado no teclado

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
    HevCDD = OpenEvent(EVENT_ALL_ACCESS, TRUE, TEXT("evCDD"));
    do {
        //
        WaitForSingleObject(HevCDD, INFINITE);
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
    } while (tecla != ESC );
    return 0;
}

unsigned __stdcall retiradaDadosOtimizacao(void* arg) {
    HevRDO = OpenEvent(EVENT_ALL_ACCESS, TRUE, TEXT("evRDO"));
    do {
        WaitForSingleObject(HevRDO, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        Sleep(1000);
        int tam = MEMORIA.size();
        for (int i = 0; i < tam; i++) {
            char status = MEMORIA[i][7];
            if (status == '1') {
                MEMORIA.erase(MEMORIA.begin() + i);
                ReleaseSemaphore(semaforo_memoria, 1, NULL);
                cout << "Retirando mensagem do tipo " << status << " Tamanho atual da memoria: " << MEMORIA.size() << endl;
                break;
            }
        }
        ReleaseSemaphore(mutex, 1, NULL);
    } while (tecla != ESC);
    return 0;
}

unsigned __stdcall retiradaDadosProcesso(void* arg) {
    HevRDP = OpenEvent(EVENT_ALL_ACCESS, TRUE, TEXT("evRDP"));
    do {
        WaitForSingleObject(HevRDP, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        Sleep(1000);
        int tam = MEMORIA.size();
        for (int i = 0; i < tam; i++) {
            char status = MEMORIA[i][7];
            if (status == '2') {
                MEMORIA.erase(MEMORIA.begin() + i);
                ReleaseSemaphore(semaforo_memoria, 1, NULL);
                cout << "Retirando mensagem do tipo " << status << " Tamanho atual da memoria: " << MEMORIA.size() << endl;
                break;
            }
        }
        ReleaseSemaphore(mutex, 1, NULL);
    } while (tecla != ESC);
    return 0;
}

unsigned __stdcall retiradaDadosAlarme(void* arg) { 
    HevRDA = OpenEvent(EVENT_ALL_ACCESS, TRUE, TEXT("evRDA"));
    do {
        WaitForSingleObject(HevRDA, INFINITE);
        WaitForSingleObject(mutex, INFINITE);
        Sleep(1000);
        int tam = MEMORIA.size();
        for (int i = 0; i < tam; i++) {
            char status = MEMORIA[i][7];
            if (status == '5') {
                MEMORIA.erase(MEMORIA.begin() + i);
                ReleaseSemaphore(semaforo_memoria, 1, NULL);
                cout << "Retirando mensagem do tipo " << status << " Tamanho atual da memoria: " << MEMORIA.size() << endl;
                break;
            }
        }
        ReleaseSemaphore(mutex, 1, NULL);
    } while (tecla != ESC );
    return 0;
}

unsigned __stdcall exibicaoDadosOtimizacao(void* arg) {
    HevEDO = OpenEvent(EVENT_ALL_ACCESS, TRUE, TEXT("evEDO"));
    do {
        //
        WaitForSingleObject(HevEDO, INFINITE);
    } while (tecla != ESC);
    return 0;
}

unsigned __stdcall exibicaoDadosProcesso(void* arg) {
    HevEDP = OpenEvent(EVENT_ALL_ACCESS, TRUE, TEXT("evEDP"));
    do {
        //
        WaitForSingleObject(HevEDP, INFINITE);
    } while (tecla != ESC);
    return 0;
}
unsigned __stdcall exibicaoDadosAlarme(void* arg) {
    HevEDA = OpenEvent(EVENT_ALL_ACCESS, TRUE, TEXT("evEDA"));
    do {
        //
        WaitForSingleObject(HevEDA, INFINITE);
    } while (tecla != ESC);
    return 0;
}

unsigned __stdcall leituraDeDados(void* arg) {
    do {
    } while (tecla != ESC);
    return 0;
}

int main() {
    HANDLE hThreads[NTHREADS];
    unsigned threadID[NTHREADS];

    semaforo_memoria = CreateSemaphore(NULL, 100, 100, NULL);

    mutex = CreateSemaphore(NULL, 1, 1, NULL);


    // Criando Eventos
    HevCDD = CreateEvent(NULL, TRUE, TRUE, TEXT("evCDD"));
    HevRDO = CreateEvent(NULL, TRUE, TRUE, TEXT("evRDO"));
    HevRDP = CreateEvent(NULL, TRUE, TRUE, TEXT("evRDP"));
    HevRDA = CreateEvent(NULL, TRUE, TRUE, TEXT("evRDA"));
    HevEDO = CreateEvent(NULL, TRUE, TRUE, TEXT("evEDO"));
    HevEDP = CreateEvent(NULL, TRUE, TRUE, TEXT("evEDP"));
    HevEDA = CreateEvent(NULL, TRUE, TRUE, TEXT("evEDA"));
    HevLimparConsoleEDA = CreateEvent(NULL, TRUE, TRUE, TEXT("evLimparConsoleEDA"));

    //Criaçao de threads
    printf("Criando threads...\n");
    hThreads[0] = (HANDLE)_beginthreadex(NULL, 0, &comunicacaoDeDados, NULL, 0, &threadID[0]);
    hThreads[1] = (HANDLE)_beginthreadex(NULL, 0, &retiradaDadosOtimizacao, NULL, 0, &threadID[1]);
    hThreads[2] = (HANDLE)_beginthreadex(NULL, 0, &retiradaDadosProcesso, NULL, 0, &threadID[2]);
    hThreads[3] = (HANDLE)_beginthreadex(NULL, 0, &retiradaDadosAlarme, NULL, 0, &threadID[3]);
    hThreads[4] = (HANDLE)_beginthreadex(NULL, 0, &exibicaoDadosOtimizacao, NULL, 0, &threadID[4]);
    hThreads[5] = (HANDLE)_beginthreadex(NULL, 0, &exibicaoDadosProcesso, NULL, 0, &threadID[5]);
    hThreads[6] = (HANDLE)_beginthreadex(NULL, 0, &exibicaoDadosAlarme, NULL, 0, &threadID[6]);
    hThreads[7] = (HANDLE)_beginthreadex(NULL, 0, &leituraDeDados, NULL, 0, &threadID[7]);

    do {
        tecla = _getch();

        if (tecla == TeclaC) {
            if (comunicacaoDeDadosLigado == 1) {
                ResetEvent(HevCDD);
                cout << "Comunicação de Dados bloqueada" << endl;
                comunicacaoDeDadosLigado = 0;
            }
            else {
                SetEvent(HevCDD);
                cout << "Comunicação de Dados desbloqueada" << endl;
                comunicacaoDeDadosLigado = 1;
            }
        }

        if (tecla == TeclaO ) {
            if (retiradaDadosOtimizacaoLigado == 1) {
                ResetEvent(HevRDO);
                cout << "Retirada Dados de Otimização bloqueada" << endl;
                retiradaDadosOtimizacaoLigado = 0;
            }
            else {
                SetEvent(HevRDO);
                cout << "Retirada Dados de Otimização desbloqueada" << endl;
                retiradaDadosOtimizacaoLigado = 1;
            }
        }

        if (tecla == TeclaP ) {
            if (retiradaDadosProcessoLigado == 1) {
                ResetEvent(HevRDP);
                cout << "Retirada Dados de Processo bloqueada" << endl;
                retiradaDadosProcessoLigado = 0;
            }
            else {
                cout << "Retirada Dados de Processo desbloqueada" << endl;
                SetEvent(HevRDP);
                retiradaDadosProcessoLigado = 1;
            }
        }

        if (tecla == TeclaA ) {
            cout << "A clicado" << endl;
            if (retiradaDadosAlarmesLigado == 1) {
                ResetEvent(HevRDA);
                cout << "Retirada Dados de Alarmes bloqueada" << endl;
                retiradaDadosAlarmesLigado = 0;
            }
            else {
                SetEvent(HevRDA);
                cout << "Retirada Dados de Alarmes desbloqueada" << endl;
                retiradaDadosAlarmesLigado = 1;
            }
        }

        if (tecla == TeclaT ) {
            if (exibicaoDadosOtimizacaoLigado == 1) {
                ResetEvent(HevEDO);
                cout << "Exibição Dados de Otimização bloqueada" << endl;
                exibicaoDadosOtimizacaoLigado = 0;
            }
            else {
                SetEvent(HevEDO);
                cout << "Exibição Dados de Otimização desbloqueada" << endl;
                exibicaoDadosOtimizacaoLigado = 1;
            }
        }

        if (tecla == TeclaR ) {
            if (exibicaoDadosProcessoLigado == 1) {
                ResetEvent(HevEDP);
                cout << "Exibição Dados de Processo bloqueada" << endl;
                exibicaoDadosProcessoLigado = 0;
            }
            else {
                SetEvent(HevEDP);
                cout << "Exibição Dados de Processo desbloqueada" << endl;
                exibicaoDadosProcessoLigado = 1;
            }
        }

        if (tecla == TeclaL ) {
            if (exibicaoDadosAlarmesLigado == 1) {
                ResetEvent(HevEDA);
                cout << "Exibição Dados de Alarmes bloqueada" << endl;
                exibicaoDadosAlarmesLigado = 0;
            }
            else {
                SetEvent(HevEDA);
                cout << "Exibição Dados de Alarmes desbloqueada" << endl;
                exibicaoDadosAlarmesLigado = 1;
            }
        }

        if (tecla == TeclaZ) {
            SetEvent(HevLimparConsoleEDA);
            cout << "Exibição Dados de Alarmes Console Limpo" << endl;
        }
    }while (tecla != ESC);

    //Execução das threads
    WaitForMultipleObjects(NTHREADS, hThreads, TRUE, INFINITE);
    //Encerramento das threads
    for (int i = 0; i < NTHREADS; i++)
        CloseHandle(hThreads[i]);

    return 0;
}