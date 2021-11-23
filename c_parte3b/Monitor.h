#include <iostream>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <vector>

using namespace std;

class Monitor{
    private:
        pthread_mutex_t barrera_entrada_m;
        pthread_mutex_t barrera_salida_m;
        pthread_cond_t barrera_entrada_c;
        pthread_cond_t barrera_salida_c;
        int contador_tareas_ejec;
        int contador_tareas_term;
        int N;
    public:
        Monitor(int N);
        ~Monitor();
        void tareaTerminada();
        int getNum();
};