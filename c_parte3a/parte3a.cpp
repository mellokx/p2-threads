#include "Monitor.h"

struct parametros{
    int num_hebra;
    int M;
    Monitor * m;
    int * tareas_por_etapa;
};

void * hebra(void * arg){
    struct parametros * p = (struct parametros *) arg;
    int r = 0;
    for(int i = 0; i<p->M; ++i){
        r = rand()%13;
        //sleep(r);
        printf("    Tarea #%d terminada por hebra %d en %d segundos\n",p->tareas_por_etapa[i],p->num_hebra,r);
        p->m->tareaTerminada();
    }
    return (void *)arg;
}

void sincronizacion_con_monitor(int N,int M){
    Monitor * m = new Monitor(N);
    pthread_t vec_hebras[N];
    struct parametros * p = (struct parametros *)malloc(N*sizeof(struct parametros));
    /* Creacion de las hebras que trabajan de forma colaborativa */
    int aux;
    for(int i = 0; i<N; ++i){
        p[i].num_hebra = i + 1;
        p[i].M = M;
        p[i].m = m;
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
    free(p);
    for(int i = 0; i<N; ++i){
        free(p[i].tareas_por_etapa);
    }
    delete m;
}

int main(){
    srand(time(NULL));
    int N,M;
    printf("Ingrese N hebras y M etapas\n");
    cin>>N;
    cin>>M;
    sincronizacion_con_monitor(N,M);
    return 0;
}