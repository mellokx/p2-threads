#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define N_HEBRAS 5
#define M_ETAPAS 3
static int contador = 0;
static int num_etapa = 0;
sem_t mutex;
sem_t torniquete1;
sem_t torniquete2;

struct parametros{
    int num_hebra;
    int * tareas_por_etapa;
};

void * hebra(void * arg){
    struct parametros * p = (struct parametros *) arg;
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

        printf("    Hebra %d trabajando en tarea #%d ...\n",p->num_hebra,p->tareas_por_etapa[i]);
        r = rand()%13;
        sleep(r);
        printf("    Tarea #%d terminada por hebra %d en %d segundos\n",p->tareas_por_etapa[i],p->num_hebra,r);
        
        sem_wait(&mutex);//lock
        contador--;
        if(contador == 0){
            ++num_etapa;
            printf("Etapa %d terminada.\n",num_etapa);
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
    
    struct parametros * p = (struct parametros *)malloc(N_HEBRAS*sizeof(struct parametros));
    int aux;
    for(int i = 0; i < N_HEBRAS; ++i){
        p[i].num_hebra = i + 1;
        p[i].tareas_por_etapa = (int *)malloc(M_ETAPAS*sizeof(int));
        aux = i+1;
        /* Establecemos las tareas que deberá realizar una hebra en cada etapa */
        for(int j = 0; j< M_ETAPAS; ++j){
            p[i].tareas_por_etapa[j] = aux;
            aux += N_HEBRAS;
        }
        /* Crea e inicia tareas de las hebras*/
        pthread_create (&vec_hebras[i], NULL, hebra, (void *) &p[i]);
    }


    for(int i = 0; i < N_HEBRAS; ++i){
        pthread_join(vec_hebras[i],NULL);
    }

    printf("Se han completado exitosamente todas las etapas\n");
    /* Libera memoria de los parametros ocupados por cada hebra */
    free(p);
    for(int i = 0; i<N_HEBRAS; ++i){
        free(p[i].tareas_por_etapa);
    }

    return 0;
}