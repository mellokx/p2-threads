#include "Monitor.h"

Monitor::Monitor(int N){
    this->N = N;
    hebras_listas = 0;
    etapa_actual = 0;
    etapa_anterior = 0;
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
        // Primera hebra en entrar por primera vez o luego de
        // liberar N - 1 hebras actualiza la etapa anterior
        if(etapa_anterior + 1 == etapa_actual){
            etapa_anterior++;
        }
        // Mientras N hebras no hayan terminado su tarea o
        // alguna hebra de la etapa anterior no haya vuelto del
        // wait, hebras restantes esperan en este ciclo
        while(etapa_actual == etapa_anterior){
            pthread_cond_wait(&barrera,&lock);
        }
    }else{
        hebras_listas = 0;
        etapa_actual++;
        printf("Etapa %d terminada.\n",etapa_actual);
        pthread_cond_broadcast(&barrera);
    }
    pthread_mutex_unlock(&lock);
}