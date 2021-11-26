#include "Monitor.h"

Monitor::Monitor(int N){
    this->N = N;
    hebras_listas = 0;
    enEspera = true;
    etapa_actual = 0;
    pthread_cond_init(&barrera,NULL);
    pthread_mutex_init(&lock,NULL);
}

Monitor::~Monitor(){
    pthread_cond_destroy(&barrera);
    pthread_mutex_destroy(&lock);
}

void Monitor::establecerBarrera(){
    pthread_mutex_lock(&lock);
    ++hebras_listas;
    int aux = hebras_listas;
    while(aux < N){
        pthread_cond_wait(&barrera,&lock);
    }
    if(aux == N){
        pthread_cond_broadcast(&barrera);
        hebras_listas = 0;
    }
    pthread_mutex_unlock(&lock);
}