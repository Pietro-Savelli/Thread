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
void pop();

void *produttore(void* arg){
	while(1){
		usleep(random()%1000000);

		pthread_mutex_lock(&lock);
		while(b.count==DIMENSIONE_BUFFER){
			pthread_mutex_unlock(&lock);
			sched_yield();
			pthread_mutex_lock(&lock);
		}

		push();
		pthread_mutex_unlock(&lock);
	}
}

void *consumatore(void* arg){
	while(1){
		usleep(random()%1000000);

		pthread_mutex_lock(&lock);
		while(b.count==0){
			pthread_mutex_unlock(&lock);
			sched_yield();
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

		sleep(1);

		pthread_mutex_lock(&lock);
		Buffer copia = b;
		pthread_mutex_unlock(&lock);


		printf("elementi Buffer\n");
		for(int i=0; i<copia.count; i++){
			printf("%d-", copia.buffer[copia.coda]);
			somma += copia.buffer[copia.coda];
			copia.coda = (copia.coda+1)%DIMENSIONE_BUFFER;
		}
		if (copia.count > 0){
			media = somma / copia.count;
		}
		printf("\nMedia: %d\n", media);
		printf("Prodotti: %d\n", copia.prodotto);
		printf("Consumati: %d\n", copia.consumato);
		printf("------------------------------\n");
	}
}


void push(){
	int quanti = (random()%(DIMENSIONE_BUFFER-b.count))+1;
	for(int i=0; i<quanti; i++){
		int daInserire = random()%100;
		b.buffer[b.testa] = daInserire;
		b.testa = (b.testa+1)%DIMENSIONE_BUFFER;
		b.prodotto++;
		b.count++;
	}

}

void pop(){
	int quanti = (random()%b.count)+1;
	b.coda = (b.coda+quanti)%DIMENSIONE_BUFFER;
	b.count -= quanti;
	b.consumato+=quanti;
}

int main(int argc, char const *argv[]){

	//imposta il seed per random
	srand(time(NULL));

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
}


