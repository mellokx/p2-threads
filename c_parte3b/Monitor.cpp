#include "Monitor.h"

Monitor::Monitor(int N){
    this->N = N;
    hebras_listas = 0;
    num_etapa = 0;
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
    if(hebras_listas < N){
        pthread_cond_wait(&barrera,&lock);
    }else{
        //Caso en que tenemos N hebras listas
        ++num_etapa;
        cout<<"Etapa "<<num_etapa<<" terminada."<<endl;
        hebras_listas = 0;
        pthread_cond_broadcast(&barrera);
    }
    pthread_mutex_unlock(&lock);
}