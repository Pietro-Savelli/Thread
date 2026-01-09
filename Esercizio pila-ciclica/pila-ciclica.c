#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


typedef struct Buffer{
    int testa;
    int coda;
    int dim;
    int count;
    int *lista;
}buffer;

//variabili condivisa
buffer b;
int grandezzaLista;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;




void stampa(){
    if(b.count == 0){
        printf("VUOTO\n");
        return;  // Meglio fare return qui
    }
    for(int i=0; i<b.count; i++){
        int idx = (b.coda + i) % b.dim;
        printf("-%d-", b.lista[idx]);
    }
    printf("\n");
}

void My_add(int disponibili){
    
    if(disponibili==0){
        return;
    }

    int scelti = random()%disponibili+1;

    for(int i=0; i<scelti; i++){

        b.lista[b.testa] = random()%100;
        b.testa = (b.testa+1) % b.dim;
    }
    b.count += scelti;
    //printf("%d\n", disponibili);
}


void stampante(int disponibili){
    if(disponibili==0)
        return;
    int scelti = random()%disponibili+1;

    b.coda = (b.coda+scelti) % b.dim;
    b.count -= scelti;
    //printf("%d\n", disponibili);
}


void *consumatore(void *arg){ // STAMPANTE

    while(1){
        usleep(500000);

        pthread_mutex_lock(&lock);
        while(b.count==0){
            pthread_mutex_unlock(&lock);
            sched_yield();
            pthread_mutex_lock(&lock);
        }

        stampante(b.count);
        stampa();
        pthread_mutex_unlock(&lock);
    }
    
}


void *produttore(void *arg){  // GENERATORE

    while(1){
        usleep(500000);
        
        pthread_mutex_lock(&lock);

        while(b.count==b.dim){
            pthread_mutex_unlock(&lock);
            sched_yield();
            pthread_mutex_lock(&lock);
        }

        My_add(b.dim - b.count);
        stampa();
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
    b.count = 0;
    b.coda = 0;// ultimo dall'altraparte 
    b.dim = grandezzaLista;

    //inizializone seed
    srand(time(NULL));

    printf("Main: inizio\n");
    // dichiaro thread
    pthread_t c, p;

    // creazione dei thread
    pthread_create(&c, NULL, consumatore, NULL);
    pthread_create(&p, NULL, produttore, NULL);

    // attesa dei thread
    pthread_join(p, NULL);
    pthread_join(c, NULL);

    printf("Main: fine\n");
    free(b.lista);
}   
