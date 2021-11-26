#include "Monitor.h"

Monitor::Monitor(int N){
    this->N = N;
    hebras_listas = 0;
    numero_etapa = 0;
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
    int aux = numero_etapa;

    while(hebras_listas < N && numero_etapa == aux){
        pthread_cond_wait(&barrera,&lock);
    }

    if(hebras_listas == N){
        printf("Fin de etapa %d\n", numero_etapa+1);
        hebras_listas = 0;
        numero_etapa++;
        pthread_cond_broadcast(&barrera); 
    }
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