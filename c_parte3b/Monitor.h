#include <iostream>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <vector>

using namespace std;

class Monitor{
    private:
        pthread_cond_t barrera_entrada;
        pthread_cond_t barrera_salida;
        pthread_mutex_t lock;
        int hebras_listas;
        int hebras_anteriores;
        int num_etapa;
        int N;
    public:
        Monitor(int N);
        ~Monitor();
        void iniciaTarea();
        void terminaTarea();
};