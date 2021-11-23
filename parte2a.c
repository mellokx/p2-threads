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
sem_t torniquete;


void * hebra(void * arg){
    int r;
    for(int i = 0; i<M_ETAPAS; ++i){
        sem_wait(&mutex);//lock
        contador++;
        sem_post(&mutex);//signal/unlock

        if(contador == N_HEBRAS){
            sem_post(&torniquete);//signal
        }

        sem_wait(&torniquete);
        sem_post(&torniquete);//signal

        r = rand()%12 + 1;
        sleep(r);
        printf("    Tarea terminada por hebra en %d segundos\n",r);
        
        sem_wait(&mutex);//lock
        contador--;
        sem_post(&mutex);//signal/unlock
        
        if(contador == 0){
            sem_wait(&torniquete);
        }
    }
}

int main(){
    pthread_t vec_hebras[N_HEBRAS];
    sem_init(&mutex, 0, 1); // semaforo inicializado en 1
    sem_init(&torniquete, 0, 0); // semaforo inicializado en 0

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