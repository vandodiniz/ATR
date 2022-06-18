#include <iostream>
#include <string>
#include <time.h>
#include <stdio.h>
using namespace std;

void incrementa_contador(int *contador){
    *contador+=1;

    if (*contador > 999999)
        *contador = 1;
}

string gera_NSEQ(int contador){    
    string NSEQ;
    string contador_string = to_string(contador);
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

string gera_tempo(){
    string TIMESTAMP;
    time_t result = time(nullptr);
    string tempo = asctime(localtime(&result));
    for (int i = 11; i<19; i++)
        TIMESTAMP += tempo[i];
    return TIMESTAMP;
}

string gera_mensagem_otimizacao(int* contador){

    incrementa_contador(contador);

    string NSEQ = gera_NSEQ(*contador);
    string TIPO = "11";
    string SP_PRESS = gera_NNNNpN();
    string SP_TEMP = gera_NNNNpN();
    string VOL = gera_N(5);
    string TIMESTAMP = gera_tempo();
    
    return NSEQ + "|" + TIPO + "|" + SP_PRESS + "|" + SP_TEMP + "|" + VOL + "|" + TIMESTAMP;
}

string gera_mensagem_scada_processo(int* contador){

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

string gera_mensagem_scada_alarmes(int* contador){

    incrementa_contador(contador);

    string NSEQ = gera_NSEQ(*contador);
    string TIPO = "55";
    string ID = gera_N(4);
    string PRIORIDADE = gera_N(3);
    string TIMESTAMP = gera_tempo();
    
    return NSEQ + "|" + TIPO + "|" + ID + "|" + PRIORIDADE + "|" + TIMESTAMP;
}