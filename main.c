#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_barrier_t barrera;

struct parametros{
    int num_hebra;
    int M;
    int * tareas_por_etapa;
};

void * hebra(void * arg){
    struct parametros * p = (struct parametros *) arg;
    int r = 0;
    for(int i = 0; i<p->M; ++i){
        r = rand()%11 + 1;
        sleep(r);
        printf("    Tarea #%d terminada por hebra %d en %d segundos\n",p->tareas_por_etapa[i],p->num_hebra,r);
        pthread_barrier_wait (&barrera);
    }
}

void sincronizacion_con_barrier(int N,int M){
    pthread_t vec_hebras[N];
    /* Creacion de la barrera */
    pthread_barrier_init (&barrera, NULL, N + 1);
    /* Arreglo de estructuras que almacenan los parametros de cada hebra */
    struct parametros * p = (struct parametros *)malloc(N*sizeof(struct parametros));
    /* Creacion de las hebras que trabajan de forma colaborativa*/
    int aux = 0;
    for(int i = 0; i<N; ++i){
        p[i].num_hebra = i + 1;
        p[i].M = M;
        p[i].tareas_por_etapa = (int *)malloc(M*sizeof(int));
        for(int j = 0; j< M; ++j){
            p[i].tareas_por_etapa[j] = aux;
            aux++;
        }
        pthread_create (&vec_hebras[i], NULL, hebra, (void *) &p[i]);
        printf("%d\n",vec_hebras[i]);
    }
    for(int i = 0; i<M; ++i){
        printf("--------------------\n",i+1);
        printf("Etapa %d iniciada...\n",i+1);
        pthread_barrier_wait (&barrera);
        printf("Etapa %d terminada.\n",i+1);
    }
    printf("Se han completado exitosamente todas las etapas\n");
    free(p);
    for(int i = 0; i<N; ++i){
        free(p[i].tareas_por_etapa);
    }
}

int main(){
    srand(time(NULL));
    int N,M;
    printf("Ingrese N hebras y M etapas\n");
    scanf("%d",&N);
    scanf("%d",&M);
    sincronizacion_con_barrier(N,M);
    return 0;
}
/*
for(int i = 0; i < N; ++i){
        if(pthread_create(&hilo[i], NULL, hilo1, NULL) != 0){
            perror("Error al crear thread!");
        }
    }
    for(int i = 0; i<M; ++i){
        printf("Etapa %d iniciada...\n",i+1);
        for(int j = 0; j < N; ++j){
            if(pthread_join(hilo[i], NULL) != 0){
                perror("Error al crear thread!");
            }
        }
        pthread_barrier_wait (&barrera);
        printf("Etapa %d terminada.\n",i + 1);
    }*/