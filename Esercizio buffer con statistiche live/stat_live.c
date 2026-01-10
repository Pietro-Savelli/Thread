#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define DIMENSIONE_BUFFER 10

typedef struct stat_live{
	int buffer[DIMENSIONE_BUFFER];
	int testa;
	int coda;
	int count;
	int prodotto;
	int consumato;
}Buffer;


Buffer b;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *consumatore(void* arg);
void *produttore(void* arg);
void *statistiche(void* arg);
void push();

void* consumatore(void* arg){
	while(1){
		usleep(random()%1000000);

		pthread_mutex_lock(&lock);
		while(count==DIMENSIONE_BUFFER){
			pthread_mutex_unlock(&lock);
			sched_yeald();
			pthread_mutex_lock(&lock);
		}

		push();
		pthread_mutex_unlock(&lock);
	}
}

void* consumatore(void* arg){
	while(1){
		usleep(random()%1000000);

		pthread_mutex_lock(&lock);
		while(count==0){
			pthread_mutex_unlock(&lock);
			sched_yeald();
			pthread_mutex_lock(&lock);
		}

		pop();
		pthread_mutex_unlock(&lock);
	}
}

void* statistiche(void* arg){
	while(1){
		int somma=0;
		int media=0;

		sleep(2);

		pthread_mutex_lock(&lock);
		Buffer copia = b;
		pthread_mutex_unlock(&lock);


		printf("elementi Buffer\n");
		for(int i=0; i<copia.count; i++){
			printf("%d-", copia.buffer[copia.testa]);
			somma += copia.buffer[copia.testa];
			copia.testa = (copia.testa+1)%DIMENSIONE_BUFFER;
		}
		if (copia.count > 0)
    		media = somma / copia.count;

		printf("Media: %d\n", media);
		printf("Prodotti: %d\n", copia.prodotto);
		printf("Consuato: %d\n", copia.consumato);
		printf("------------------------------\n");
	}
}


void push(){
	int daInserire = random()%100;
	b.buffer[testa] = daInserire;
	testa = (testa+1)%DIMENSIONE_BUFFER;
	b.prodotto++;
}

void pop(){
	coda = (coda-1)%DIMENSIONE_BUFFER;
	b.consumato++;
}

int main(int argc, char const *argv[]){

	//imposta il seed per random
	srand(time(NULL));

	b.buffer = malloc(sizeof(int)*DIMENSIONE_BUFFER);
	b.testa = 0;
	b.coda = 0;
	b.count = 0;
	b.prodotto = 0;
	b.consumato = 0;

	printf("inizio main\n");
	pthread_t prod, cons, stat;

	pthread_create(&prod, NULL, produttore, "P");
	pthread_create(&cons, NULL, consumatore, "C");
	pthread_create(&stat, NULL, statistiche, "S");

	pthread_join(prod, NULL);
	pthread_join(cons, NULL);
	pthread_join(stat, NULL);
	free(b.buffer);
}


