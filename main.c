#include <stdio.h>
#include <time.h>
#include <pthread.h>

pthread_barrier_t   barrera;

void * hilo1(void * aux){
    sleep(5);
    pthread_barrier_wait (&barrera);
    printf("Terminado hilo 1\n");
}
void * hilo2(void * aux){
    sleep(8);
    pthread_barrier_wait (&barrera);
    printf("Terminado hilo 2\n");
}
void * hilo3(void * aux){
    sleep(10);
    pthread_barrier_wait (&barrera);
    printf("Terminado hilo 3\n");
}

int main(){
    /* Creacion de la barrera */
    pthread_barrier_init (&barrera, NULL, 3);
    pthread_create (NULL, NULL, hilo1, NULL);
    pthread_create (NULL, NULL, hilo2, NULL);
    pthread_create (NULL, NULL, hilo3, NULL);
    printf("hello world!\n");
    return 0;
}