#include "Monitor.h"

Monitor::Monitor(int N){
    this->N = N;
    hebras_listas = 0;
    etapa_anterior = 0;
    etapa_actual = 0;
    pthread_cond_init(&barrera,NULL);
    pthread_mutex_init(&lock,NULL);
}

Monitor::~Monitor(){
    pthread_cond_destroy(&barrera);
    pthread_mutex_destroy(&lock);
}

void Monitor::establecerBarrera(int num_hebra){
    pthread_mutex_lock(&lock);
    hebras_listas++;
    if(hebras_listas < N){
        printf("entro al if hebra: %d\n",num_hebra);
        pthread_cond_wait(&barrera,&lock);
    }else{
        printf("entro al else hebra: %d\n",num_hebra);
        //Caso en que tenemos N hebras listas
        ++etapa_actual;
        printf("Etapa %d terminada.\n",etapa_actual);
        hebras_listas = 0;
        pthread_cond_broadcast(&barrera);
    }
    printf("hebra %d hace unlock\n",num_hebra);
    pthread_mutex_unlock(&lock);
}

/*
void Monitor::establecerBarrera(int num_hebra){
    pthread_mutex_lock(&lock);
    hebras_listas++;
    if(etapa_actual > 0){
        printf("hebras listas dps de 1ra etapa : %d\n",hebras_listas);
    }
    if(hebras_listas < N){
        printf("entro al if hebra: %d\n",num_hebra);
        if((etapa_anterior + 1) == etapa_actual){
            ++etapa_anterior;
        }
        while(etapa_anterior == etapa_actual){
            pthread_cond_wait(&barrera,&lock);
        }
    }else{
        printf("entro al else hebra: %d\n",num_hebra);
        //Caso en que tenemos N hebras listas
        ++etapa_actual;
        printf("Etapa %d terminada.\n",etapa_actual);
        hebras_listas = 0;
        pthread_cond_broadcast(&barrera);
    }
    printf("hebra %d hace unlock\n",num_hebra);
    pthread_mutex_unlock(&lock);
    pthread_mutex_unlock(&lock);
    pthread_mutex_unlock(&lock);
}*/