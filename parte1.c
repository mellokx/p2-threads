#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_barrier_t barrera;
pthread_mutex_t lock;

static int hebras_listas;
static int num_etapa;

struct parametros{
    int num_hebra;
    int M;
    int N;
    int * tareas_por_etapa;
};

void * hebra(void * arg){
    struct parametros * p = (struct parametros *) arg;
    int r = 0;
    for(int i = 0; i<p->M; ++i){
        printf("    Hebra %d trabajando en tarea #%d ...\n",p->num_hebra,p->tareas_por_etapa[i]);
        r = rand()%13;
        sleep(r);
        printf("    Tarea #%d terminada por hebra %d en %d segundos\n",p->tareas_por_etapa[i],p->num_hebra,r);
        pthread_mutex_lock(&lock);
        ++hebras_listas;
        if(hebras_listas == p->N){
            hebras_listas = 0;
            ++num_etapa;
            printf("Etapa %d terminada.\n",num_etapa);
        }
        pthread_mutex_unlock(&lock);
        pthread_barrier_wait(&barrera);
    }
}

void sincronizacion_con_barrier(int N,int M){
    num_etapa = 0;
    hebras_listas = 0;
    pthread_t vec_hebras[N];
    /* Creacion de la barrera */
    pthread_barrier_init (&barrera, NULL, N);
    /* Arreglo de estructuras que almacenan los parametros de cada hebra */
    struct parametros * p = (struct parametros *)malloc(N*sizeof(struct parametros));
    /* Creacion de las hebras que trabajan de forma colaborativa */
    int aux;
    for(int i = 0; i<N; ++i){
        p[i].num_hebra = i + 1;
        p[i].M = M;
        p[i].N = N;
        p[i].tareas_por_etapa = (int *)malloc(M*sizeof(int));
        aux = i+1;
        for(int j = 0; j< M; ++j){
            p[i].tareas_por_etapa[j] = aux;
            aux += N;
        }
        /* Crea e inicia tareas de las hebras*/
        pthread_create (&vec_hebras[i], NULL, hebra, (void *) &p[i]);
    }
    for(int i = 0; i < N; ++i){
        pthread_join(vec_hebras[i],NULL);
    }
    printf("Se han completado exitosamente todas las etapas\n");
    /* Libera memoria de los parametros ocupados por cada hebra */
    for(int i = 0; i<N; ++i){
        free(p[i].tareas_por_etapa);
    }
    free(p);
    pthread_barrier_destroy(&barrera);
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