#include "Monitor.h"

Monitor::Monitor(int N){
    this->N = N;
    contador = 0;
    num_etapa = 0;
    pasoUno = false;
    pthread_mutex_init(&barrera,NULL);
    pthread_mutex_init(&torniquete,NULL);
}

Monitor::~Monitor(){
    pthread_mutex_destroy(&barrera);
    pthread_mutex_destroy(&torniquete);
}

void Monitor::tareaTerminada(){
    pthread_mutex_lock(&torniquete);
    if(!pasoUno){
        reiniciarBarrera();
        pasoUno = true;
    }
    contador++;
    pthread_mutex_unlock(&torniquete);
    if(contador == N){
        contador = 0;
        num_etapa++;
        pthread_mutex_unlock(&barrera);
        printf("Etapa %d terminada.\n",num_etapa);
    }else{
        pthread_mutex_lock(&barrera);
    }
    pthread_mutex_unlock(&barrera);
    pasoUno = false;
}

void Monitor::reiniciarBarrera(){
    pthread_mutex_lock(&barrera);
}