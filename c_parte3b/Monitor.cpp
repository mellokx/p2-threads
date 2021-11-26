#include "Monitor.h"

Monitor::Monitor(int N){
    this->N = N;
    hebras_listas = 0;
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
    hebras_listas++;
    // Hace efectivo el broadcast cuando hebras_listas se hace 0
    int etapa_hebra = etapa_actual;
    // Se genera la espera de las N-1 hebras mientras no acabe la
    // N-esima hebra y la etapa actual coincida con la etapa de la hebra
    while((hebras_listas < N) && (etapa_actual == etapa_hebra)){
        pthread_cond_wait(&barrera,&lock);
    }
    // N-esima hebra en terminar libera a las otras
    if(hebras_listas == N){
        printf("Etapa %d terminada.\n", etapa_actual+1);
        hebras_listas = 0;
        etapa_actual++;
        pthread_cond_broadcast(&barrera); 
    }
    pthread_mutex_unlock(&lock);
}