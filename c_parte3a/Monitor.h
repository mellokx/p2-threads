#include <iostream>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <vector>

using namespace std;

class Monitor{
    private:
        pthread_mutex_t barrera;
        pthread_mutex_t torniquete;
        int contador;
        int num_etapa;
        int N;
        bool pasoUno;
        void reiniciarBarrera();
    public:
        Monitor(int N);
        ~Monitor();
        void tareaTerminada();
        int getNum();
};