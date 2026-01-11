#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define SIZEMAX 10

typedef struct Buffer{
	int *buffer;
	int count;
	int testa;
	int coda;
}Buffer;

Buffer b1;
Buffer b2;
int consumatori;
int produttori;
int produttoriFiniti;
int tuttoFinito;

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

int spazioMinimo();

void inizializzaBuffer(Buffer *b){
	b->count = 0;
	b->testa = 0;
	b->coda = 0;
	b->buffer = malloc(sizeof(int)*SIZEMAX);
}


void *produttore(void *id){
	for(int i=0; i<5; i++){
		usleep(random()%100000);
		int daInserire = 0;

		pthread_mutex_lock(&lock1);
		while(b1.count==SIZEMAX){
			pthread_mutex_unlock(&lock1);
			sched_yield();
			pthread_mutex_lock(&lock1);
		}
		if(SIZEMAX-b1.count!=0){
			daInserire = (random() % (SIZEMAX-b1.count))+1;
		}
		for(int y=0; y<daInserire; y++){
			b1.buffer[b1.testa] = random()%100;
			b1.testa = (b1.testa+1)%SIZEMAX;
			b1.count++;
		}
		pthread_mutex_unlock(&lock1);
	}

	pthread_mutex_lock(&lock1);
	produttoriFiniti++;
	printf("PRODUTTORE %d FINITO\n", *(int*)id);
	pthread_mutex_unlock(&lock1);

	return NULL;
}

// se b2 pieno stampa e cancella una parte di b2 per poter inserrie nuovi elementi
void *consumatore(void *id){

	while(1){
		usleep(random()%1000000);

		pthread_mutex_lock(&lock1);
		pthread_mutex_lock(&lock2);
		if(produttoriFiniti==produttori && b1.count==0){
			tuttoFinito = 1;
		}
		pthread_mutex_unlock(&lock2);
		pthread_mutex_unlock(&lock1);

		//se b2 vuoto e produzione finita
		if(b2.count==0 && tuttoFinito){
			break;
		}

		if(b2.count!=0){
			int daPulire = (random()%b2.count)+1;
			printf("PULISCO PARZIALEMENTE IL B2:");
			for(int i=0; i<(daPulire); i++){
				printf("%d-", b2.buffer[b2.coda]);
				b2.coda = (b2.coda+1)%SIZEMAX;
				b2.count--;
			}
			printf("\n");
		}

		pthread_mutex_unlock(&lock2);
	}
	return NULL;
}

// ogni 2 sec sposto il buffer b1 dentro il buffer b2
void *tramite(void *nome){
	while(1){
		usleep(random()%1000000);
		int nSposta = 0;

		pthread_mutex_lock(&lock1);
		pthread_mutex_lock(&lock2);

		if (produttoriFiniti == produttori && b1.count == 0) {
			pthread_mutex_unlock(&lock2);
			pthread_mutex_unlock(&lock1);
			break;; // ESCI DAL THREAD
		}

		while(b2.count==SIZEMAX  && b1.count>0){
			pthread_mutex_unlock(&lock2);
			pthread_mutex_unlock(&lock1);
			sched_yield();
			pthread_mutex_lock(&lock1);
			pthread_mutex_lock(&lock2);
		}

		if(b1.count>0 && b2.count<SIZEMAX){
			if(spazioMinimo()!=0){
				nSposta = (random()%spazioMinimo())+1;
			}
			
			if(nSposta>0){
				for(int y=0; y<nSposta; y++){
					int appoggio = b1.buffer[b1.coda];
					b1.coda = (b1.coda+1)%SIZEMAX;
					b1.count--;

					b2.buffer[b2.testa] = appoggio;
					b2.testa = (b2.testa+1)%SIZEMAX;
					b2.count++;
				}					
			}
		
		}
		pthread_mutex_unlock(&lock2);
		pthread_mutex_unlock(&lock1);
	}
	return NULL;
}


int spazioMinimo(){
	int spazio_libero_b2 = SIZEMAX - b2.count;

    if(spazio_libero_b2 > b1.count){
        return b1.count; 
    }
    
    return spazio_libero_b2;
}

int main(int argc, char const *argv[]){

	if(argc<3){
		fprintf(stderr, "ERRORE: devi inserire altri dati\n");
		exit(1);
	}

	srandom(time(NULL));
	produttori = atoi(argv[1]);
	consumatori = atoi(argv[2]);
	produttoriFiniti = 0;
	tuttoFinito = 0;
	inizializzaBuffer(&b1);
	inizializzaBuffer(&b2);


	pthread_t *prod = malloc(sizeof(pthread_t)*produttori);
	pthread_t *cons = malloc(sizeof(pthread_t)*consumatori);
	pthread_t tram;

	int *idProd = malloc(sizeof(int)*produttori);
	int *idCons = malloc(sizeof(int)*consumatori);

	printf("creazione thread\n");
	for(int i=0; i<produttori; i++){
		idProd[i]=i+1;
		pthread_create(&prod[i], NULL, produttore, &idProd[i]);
	}
	for(int i=0; i<consumatori; i++){
		idCons[i]=i+1;
		pthread_create(&cons[i], NULL, consumatore, &idCons[i]);
	}
	pthread_create(&tram, NULL, tramite, "T:");


	for(int i=0; i<produttori; i++){
		pthread_join(prod[i], NULL);
	}
	for(int i=0; i<consumatori; i++){
		pthread_join(cons[i], NULL);
	}
	pthread_join(tram, NULL);


	free(prod);
	free(cons);
	free(idCons);
	free(idProd);
	free(b1.buffer);
	free(b2.buffer);
}