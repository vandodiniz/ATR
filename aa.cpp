
#include <iostream>
#include <vector>
using namespace std;
 
vector<int> memoria;

void_retira_memoria
    memoria.delete
    signal(semaforo(100))

void envia_memoria(int i){
    wait(semaforo_100)
    memoria.push_back(i);
} 
 
int main() {
    for (int i=0; i<100; i++)
        envia_memoria(i);
    for (int i=0; i<100; i++)
        cout << memoria[i] << endl;
}