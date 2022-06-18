#include <iostream>
#include <string>
#include <time.h>
#include <stdio.h>
using namespace std;

void incrementa_contador(int* contador) {
    /*
    Função responsável por incrementar o valor de um determinado contador. O contador é resetado após passar de 999999. 
    Função bastante importante para gerar os NSEQs.

    Args:
        contador (pointer): endereço de memória do contador.
    */

    *contador += 1;

    if (*contador > 999999)
        *contador = 1;
}

string gera_NSEQ(int contador) {
    /*
    Função responsável por gerar um NSEQ. Primeiramente, salva-se o valor do contador em uma nova variável do tipo string e 
    calcula-se o tamanho desta. A partir daí, cria-se em um ‘for loop’ uma nova string auxiliar com a quantidade de zeros 
    necessárias para que o NSEQ tenha seis dígitos. Feito isso, concatenamos essas strings e a retornamos.

    Args:
        contador (int): valor bruto do NSEQ a ser gerado.
    Returns: string de 6 elementos.
    */

    string NSEQ;
    string contador_string = to_string(contador);
    for (int i = 0; i < 6 - contador_string.length(); i++)
        NSEQ += '0';
    NSEQ += contador_string;
    return NSEQ;
}

string gera_N(int tamanho) {
    /*
    Função responsável por gerar uma string composta por N dígitos aleatórios. 
    Com esta função gera-se os valores dos parâmetros VOL, ID e PRIORIDADE. 
    Nela cada dígito é sorteado e armazenado em strings. O número final é a concatenação dessas strings.

    Args:
        tamanho (int): quantidade de digitos do número que será gerado
    Return: string de N elementos.
    */

    int num;
    string minimo;
    string N;

    //Definição do caso improvável
    for (int i = 0; i < tamanho - 1; i++)
        minimo += "0";

    //Sorteia os N números e compara com o caso improvável
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
    /*
    Função responsável por gerar uma string composta por 5 dígitos aleatórios e um ponto na quinta posição. 
    Com esta função gera-se os valores dos parâmetros SP_PRESS, SP_TEMP, PRESSAO_T, TEMP, PRESSAO_G e  NÍVEL. 
    Funcionamento similar a função anterior.

    Args: -
    Returns: string de 6 elementos.
    */

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
    /*
    Função responsável por capturar o horário do sistema e transformá-lo em uma string. Necessária para a criação do 
    parâmetro TIMESTAMP.
    Args: None.
    Returns: string de 8 elementos.
    */

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
    /*
    Função responsável por gerar uma mensagem de otimização. Primeiramente, incrementamos o contador e a partir daí geramos 
    os valores de cada parâmetro individualmente com ajuda das funções anteriores. Feito isso, concatenamos todas essas strings 
    junto com o caractere "|" para representar a mensagem final.

    Args:
        contador (int): valor bruto do NSEQ a ser gerado.
    Returns: string de 38 caracteres.
    */

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
    /*
    Função responsável por gerar uma mensagem do SCADA referentes aos dados do processo. Primeiramente, incrementamos 
    o contador e a partir daí geramos os valores de cada parâmetro individualmente com ajuda das funções anteriores. 
    Feito isso, concatenamos todas essas strings junto com o caractere "|" para representar a mensagem final.

    Args:
        contador (int): valor bruto do NSEQ a ser gerado.
    Returns: string de 46 caracteres.
    */

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
    /*
    Função responsável por gerar uma mensagem do SCADA referentes aos alarmes. Primeiramente, incrementamos o 
    contador e a partir daí geramos os valores de cada parâmetro individualmente com ajuda das funções anteriores. 
    Feito isso, concatenamos todas essas strings junto com o caractere "|" para representar a mensagem final.

    Args:
        contador (int): valor bruto do NSEQ a ser gerado.
    Returns: string de 27 caracteres.
    */

    incrementa_contador(contador);

    string NSEQ = gera_NSEQ(*contador);
    string TIPO = "55";
    string ID = gera_N(4);
    string PRIORIDADE = gera_N(3);
    string TIMESTAMP = gera_tempo();

    return NSEQ + "|" + TIPO + "|" + ID + "|" + PRIORIDADE + "|" + TIMESTAMP;
}