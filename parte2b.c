#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t semaforo;
pthread_mutex_t torniquete1;
pthread_mutex_t torniquete2;
static int contador = 0;

void * hebra(void * arg){
    int r;
    for(int i = 0; i<3; ++i){
        pthread_mutex_lock(&semaforo);
        contador++;
        if(contador == 5){
            pthread_mutex_lock(&torniquete2);
            pthread_mutex_unlock(&torniquete1);
        }
        pthread_mutex_unlock(&semaforo);

        pthread_mutex_lock(&torniquete1);
        pthread_mutex_unlock(&torniquete1);

        r = rand()%12 + 1;
        sleep(r);
        printf("    Tarea terminada por hebra en %d segundos\n",r);

        pthread_mutex_lock(&semaforo);
        contador--;
        if(contador == 0){
            pthread_mutex_lock(&torniquete1);
            pthread_mutex_unlock(&torniquete2);
        }
        pthread_mutex_unlock(&semaforo);

        pthread_mutex_lock(&torniquete2);
        pthread_mutex_unlock(&torniquete2);
    }
}

int main(){
    pthread_t vec_hebras[5];
    pthread_mutex_init(&semaforo, NULL);
    pthread_mutex_init(&torniquete1, NULL);
    pthread_mutex_init(&torniquete2, NULL);
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