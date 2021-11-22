#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t semaforo;
pthread_mutex_t torniquete;
static int contador = 0;

void * hebra(void * arg){
    int r;
    for(int i = 0; i<3; ++i){
        pthread_mutex_lock(&semaforo);
        contador++;
        pthread_mutex_unlock(&semaforo);
        if(contador == 5){
            pthread_mutex_unlock(&torniquete);
        }
        pthread_mutex_lock(&torniquete);
        pthread_mutex_unlock(&torniquete);
        r = rand()%12 + 1;
        sleep(r);
        printf("    Tarea terminada por hebra en %d segundos\n",r);
        pthread_mutex_lock(&semaforo);
        contador--;
        pthread_mutex_unlock(&semaforo);
        if(contador == 0){
            pthread_mutex_lock(&torniquete);
        }
        //pthread_barrier_wait (&barrera);
    }
}

int main(){
    pthread_t vec_hebras[5];
    pthread_mutex_init(&semaforo, NULL);
    pthread_mutex_init(&torniquete, NULL);
    for(int i = 0; i < 5; ++i){
        /* Crea e inicia tareas de las hebras*/
        pthread_create (&vec_hebras[i], NULL, hebra, NULL);
    }
    for(int i = 0; i < 5; ++i){
        pthread_join(vec_hebras[i],NULL);
    }
    pthread_mutex_destroy(&semaforo);
    return 0;
}