#include <iostream>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <vector>

using namespace std;

class Monitor{
    private:
        pthread_cond_t barrera;
        pthread_mutex_t lock;
        bool done;
        int hebras_listas;
        int etapa_actual;
        int etapa_anterior;
        int N;
    public:
        Monitor(int N);
        ~Monitor();
        void establecerBarrera(int num_hebra);
};