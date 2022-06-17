#include<iostream>
#include <windows.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <conio.h>		// _getch
#include <time.h>

using namespace std;

#define NUM_THREADS	8	// Vamos disparar 8 threads concorrentes

///////////////////////////////////////////////////////////////////////////////////////////////////////

string gera_NSEQ(int* contador){
    *contador++;
    if (*contador > 999999)
        *contador = 1;
    
    string NSEQ;
    string contador_string = to_string(*contador);
    for(int i=0; i < 6-contador_string.length(); i++)
        NSEQ+= '0';
    NSEQ+=contador_string;
    return NSEQ;
}

string gera_N(int tamanho){
    int num;
    string minimo;
    string N;

    for(int i=0; i<tamanho-1; i++)
        minimo += "0";

    for(int i=0; i<tamanho; i++){
            if (N==minimo)
                num = rand()%9 + 1;
            else
                num = rand()%10;
            N += to_string(num);
        }         
    return N;
}

string gera_NNNNpN(){
    int num;
    string NNNNpN;
    for(int i=0; i<6; i++){
            if (i==4)
                NNNNpN+=".";
            else{
                num = rand()%10;
                NNNNpN += to_string(num);
            }
        }
    return NNNNpN;
}

string gera_mensagem_scada_processo(int contador){
    string NSEQ = gera_NSEQ(&contador);
    string TIPO = "22";
    string PRESSAO_T = gera_NNNNpN();
    string TEMP = gera_NNNNpN();
    string PRESSAO_G = gera_NNNNpN();
    string NIVEL = gera_NNNNpN();
    
    return NSEQ + "|" + TIPO + "|" + PRESSAO_T + "|" + TEMP + "|" + PRESSAO_G + "|" + NIVEL + "|"
}

void envia_memoria(string mensagem){
    /
    memoria[contador] = mensagem;
    /
}

int main() {
    string memoria[100];
    int status;
    int contador = 0;
	pthread_t hThreads[NUM_THREADS];

    // CRIAÇÃO THREAD 1
    status = pthread_create(&hThreads[1], NULL, comunicacaoDeDados, (void *) contador);
	if (!status) 
        printf("Thread %2d criada", 1);
	else 
        printf ("Erro na criacao da thread %d! Codigo = %d\n", 1, status);
    
    // CRIAÇÃO THREAD 2
    status = pthread_create(&hThreads[2], NULL, retiradaDadosOtimizacao, );
	if (!status) 
        printf("Thread %2d criada", 2);
	else 
        printf ("Erro na criacao da thread %d! Codigo = %d\n", 2, status);
    
    // CRIAÇÃO THREAD 3
    status = pthread_create(&hThreads[3], NULL, retiradaDadosProcesso, );
	if (!status) 
        printf("Thread %2d criada", 3);
	else 
        printf ("Erro na criacao da thread %d! Codigo = %d\n", 3, status);
    
    // CRIAÇÃO THREAD 4
    status = pthread_create(&hThreads[4], NULL, retiradaAlarmes, );
	if (!status) 
        printf("Thread %2d criada", 4);
	else 
        printf ("Erro na criacao da thread %d! Codigo = %d\n", 4, status);

    // CRIAÇÃO THREAD 5
    status = pthread_create(&hThreads[5], NULL, exibicaoDadosOtimizacao, );
	if (!status) 
        printf("Thread %2d criada", 5);
	else 
        printf ("Erro na criacao da thread %d! Codigo = %d\n", 5, status);
        
    // CRIAÇÃO THREAD 6
    status = pthread_create(&hThreads[6], NULL, exibicaoDadosProcesso, );
	if (!status) 
        printf("Thread %2d criada", 6);
	else 
        printf ("Erro na criacao da thread %d! Codigo = %d\n", 6, status);

    // CRIAÇÃO THREAD 7
    status = pthread_create(&hThreads[7], NULL, exibicaoAlarme, );
	if (!status) 
        printf("Thread %2d criada", 7);
	else 
        printf ("Erro na criacao da thread %d! Codigo = %d\n", 7, status);
        
    // CRIAÇÃO THREAD 8
    status = pthread_create(&hThreads[8], NULL, leituraTeclado, );
	if (!status) 
        printf("Thread %2d criada", 8);
	else 
        printf ("Erro na criacao da thread %d! Codigo = %d\n", 8, status);    
}

// THREADS SECUNDARIAS
void *comunicacaoDeDados(void *arg){
    int NSEQ_otimizacao=0;
    int NSEQ_scada_processos=0;
    int NSEQ_scada_alarmes=0;
    while(1){
        //

        //

        //
        Sleep(0.5);
        string mensagem_scada_processo = gera_mensagem_scada_processo(NSEQ_scada_processos);
        
    }
	
    // O comando "return" abaixo � desnecess�rio, mas presente aqui para compatibilidade
	// com o Visual Studio da Microsoft
	return (void *) NULL;
} // AlocFunc


//conquista o semaforo



void *retiradaDadosOtimizacao(void *arg){
	

	pthread_exit((void *) ValorRetornado);
	
    // O comando "return" abaixo � desnecess�rio, mas presente aqui para compatibilidade
	// com o Visual Studio da Microsoft
	return (void *) NULL;
} // AlocFunc

void *retiradaDadosProcesso(void *arg){
	

	pthread_exit((void *) ValorRetornado);
	
    // O comando "return" abaixo � desnecess�rio, mas presente aqui para compatibilidade
	// com o Visual Studio da Microsoft
	return (void *) NULL;
} // AlocFunc

void * retiradaAlarmes(void *arg){
	

	pthread_exit((void *) ValorRetornado);
	
    // O comando "return" abaixo � desnecess�rio, mas presente aqui para compatibilidade
	// com o Visual Studio da Microsoft
	return (void *) NULL;
} // AlocFunc

void *exibicaoDadosOtimizacao(void *arg){
	

	pthread_exit((void *) ValorRetornado);
	
    // O comando "return" abaixo � desnecess�rio, mas presente aqui para compatibilidade
	// com o Visual Studio da Microsoft
	return (void *) NULL;
} // AlocFunc

void *exibicaoDadosProcesso(void *arg){
	

	pthread_exit((void *) ValorRetornado);
	
    // O comando "return" abaixo � desnecess�rio, mas presente aqui para compatibilidade
	// com o Visual Studio da Microsoft
	return (void *) NULL;
} // AlocFunc

void *exibicaoAlarme(void *arg){
	

	pthread_exit((void *) ValorRetornado);
	
    // O comando "return" abaixo � desnecess�rio, mas presente aqui para compatibilidade
	// com o Visual Studio da Microsoft
	return (void *) NULL;
} // AlocFunc

void *leituraTeclado(void *arg){
	

	pthread_exit((void *) ValorRetornado);
	
    // O comando "return" abaixo � desnecess�rio, mas presente aqui para compatibilidade
	// com o Visual Studio da Microsoft
	return (void *) NULL;
} // AlocFunc