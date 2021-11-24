#include "Monitor.h"

Monitor::Monitor(int N){
    this->N = N;
    contador = 0;
    num_etapa = 0;
    contador_tareas_ejec = 0;
    contador_tareas_term = N;
    pthread_cond_init(&barrera_entrada_c, NULL);
    pthread_cond_init(&barrera_salida_c, NULL);
    pthread_mutex_init(&barrera_entrada_m, NULL);
    pthread_mutex_init(&barrera_salida_m, NULL);

}

Monitor::~Monitor(){
    pthread_cond_destroy(&barrera_entrada_c);
    pthread_cond_destroy(&barrera_salida_c);
    pthread_mutex_destroy(&barrera_entrada_m);
    pthread_mutex_destroy(&barrera_salida_m);
}

void Monitor::tareaTerminada(){
    while(contador_tareas_term < N){
        pthread_mutex_lock(&barrera_entrada_m);
        pthread_cond_wait(&barrera_entrada_c,&barrera_entrada_m);
    }
    contador_tareas_ejec++;
    while(contador_tareas_ejec < N){
        pthread_mutex_lock(&barrera_salida_m);
        pthread_cond_wait(&barrera_salida_c,&barrera_salida_m);
    }
    if(contador_tareas_term==N){
        contador_tareas_term = 0;
        num_etapa++;
    }
    pthread_cond_broadcast(&barrera_salida_c);
    pthread_mutex_unlock(&barrera_salida_m);
    contador_tareas_term++;
    if(contador_tareas_term == N){
        contador_tareas_ejec = 0;
        cout<<"Etapa "<<num_etapa<<" terminada."<<endl;
        pthread_cond_broadcast(&barrera_entrada_c);
        pthread_mutex_unlock(&barrera_entrada_m);
    }
}
