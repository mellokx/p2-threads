#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define N_HEBRAS 5
#define M_ETAPAS 3
static int contador = 0;
sem_t mutex;
sem_t torniquete1;
sem_t torniquete2;


void * hebra(void * arg){
    int r;
    for(int i = 0; i<M_ETAPAS; ++i){
        sem_wait(&mutex);//lock
        contador++;
        if(contador == N_HEBRAS){
            sem_wait(&torniquete2);
            sem_post(&torniquete1);//signal
        }
        sem_post(&mutex);//signal/unlock

        sem_wait(&torniquete1);
        sem_post(&torniquete1);//signal

        r = rand()%12 + 1;
        sleep(r);
        printf("    Tarea terminada por hebra en %d segundos\n",r);
        
        sem_wait(&mutex);//lock
        contador--;
        if(contador == 0){
            sem_wait(&torniquete1);
            sem_post(&torniquete2);//signal
        }
        sem_post(&mutex);//signal/unlock

        sem_wait(&torniquete2);
        sem_post(&torniquete2);//signal
    }
}

int main(){
    pthread_t vec_hebras[N_HEBRAS];
    sem_init(&mutex, 0, 1); // semaforo inicializado en 1
    sem_init(&torniquete1, 0, 0); // semaforo inicializado en 0
    sem_init(&torniquete2, 0, 1); // semaforo inicializado en 1

    srand(time(NULL));
    
    for(int i = 0; i < N_HEBRAS; ++i){
        /* Crea e inicia tareas de las hebras*/
        pthread_create (&vec_hebras[i], NULL, hebra, NULL);
    }


    for(int i = 0; i < N_HEBRAS; ++i){
        pthread_join(vec_hebras[i],NULL);
    }

    return 0;
}