#include "Monitor.h"

Monitor::Monitor(int N){
    this->N = N;
    hebras_listas = 0;
    hebras_anteriores = 0;
    num_etapa = 0;
    pthread_cond_init(&barrera_entrada,NULL);
    pthread_cond_init(&barrera_salida,NULL);
    pthread_mutex_init(&lock,NULL);
}

Monitor::~Monitor(){
    pthread_cond_destroy(&barrera_entrada);
    pthread_cond_destroy(&barrera_salida);
    pthread_mutex_destroy(&lock);
}

void Monitor::iniciaTarea(){
    pthread_mutex_lock(&lock);
    while((hebras_anteriores < N)&&(hebras_listas == N)){
        pthread_cond_wait(&barrera_entrada,&lock);
    }
    if(hebras_anteriores == N){
        hebras_listas = 0;
        hebras_anteriores = 0;
    }
    pthread_cond_signal(&barrera_entrada);
    pthread_mutex_unlock(&lock);
}

void Monitor::terminaTarea(){
    pthread_mutex_lock(&lock);
    ++hebras_listas;
    while(hebras_listas < N){
        pthread_cond_wait(&barrera_salida,&lock);
    }
    if(hebras_anteriores == 0){
        ++num_etapa;
        cout<<"Etapa "<<num_etapa<<" terminada."<<endl;
    }
    ++hebras_anteriores;
    pthread_cond_signal(&barrera_salida);
    pthread_mutex_unlock(&lock);
}