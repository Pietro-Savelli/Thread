#include <stdio.h>
#include <stdlib.h>
#include <queue.h>
#include <pthread.h>
#include <unistd.h>
#include <>


typedef struct Buffer{
    int testa;
    int coda;
    int dim;
    int *lista;
}buffer;

//variabili condivisa
buffer b;
int grandezzaLista;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;





void My_add(int disponibili){
    int scelti = random()%disponibili+1;

    for(int i=0; i<scelti; i++, b.testa++){

        if(b.testa==grandezzaLista){
            b.esta = 0;
            b.lista[b.testa] = random()%100;
        }

        else{
            b.lista[b.testa] = random()%100;
        }
    }
    disponibili -= scelti;
}


void stampa(int disponibili){
    int scelti = random()%disponibili+1;

    for(int i=0; i<scelti; i++,b.coda++){

        if(b.coda==grandezzaLista){
            b.coda = 0;
        }
    }
    disponibili += scelti;

}


void *consumatore(void *arg){ // STAMPANTE

    while(1){
        usleep(500000);

        pthread_mutex_lock(&lock);
        if(b.dim==0){
            while(b.dim==0){
                pthread_mutex_unlock(&lock);
                sched_yield();
                pthread_mutex_lock(&lock);
            }
            continue;
        }
        My_add(grandezzaLista-b.dim);
        pthread_mutex_unlock(&lock);
    }
    
}


void *produttore(void *arg){  // GENERATORE

    while(1){
        usleep(500000);

        pthread_mutex_lock(&lock);
        if(b.dim==grandezzaLista){
            while(b.dim==grandezzaLista){
                pthread_mutex_unlock(&lock);
                sched_yield();
                pthread_mutex_lock(&lock);
            }
            continue;
        }
        stampa(b.dim);
        pthread_mutex_unlock(&lock);
    }
}   

int main(int argc, char** argv){

    if(argc<2){
        fprintf(stderr, "inserisci un altro dato\n");
        exit(1);
    }

    grandezzaLista = atoi(argv[1]);

    // inizializzione buffer
    b.lista = malloc(sizeof(int)*grandezzaLista);
    b.testa = 0;
    b.coda = grandezzaLista;// ultimo dall'altraparte 
    b.dim = grandezzaLista;

    printf("Main: inizio\n");
    // dichiaro thread
    pthread_t c, p;

    // creazione dei thread
    pthread_create(&c, NULL, consumatore, 'C');
    pthread_create(&p, NULL, produttore, 'P');

    // attesa dei thread
    pthread_join(p, NULL);
    pthread_join(c, NULL);

    printf("Main: fine\n");
}   
