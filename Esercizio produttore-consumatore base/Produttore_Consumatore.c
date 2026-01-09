#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define GRANDEZZAPILA 10

typedef struct pila{
	int *array;
	int inseriti;
}pila;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
pila p;

void stampa(){
	for(int i=0; i<p.inseriti; i++){
		printf("%d-", p.array[i]);
	}
	printf("\n");
}

void push(){
	int daInserire = (random()%(GRANDEZZAPILA-p.inseriti))+1;
	printf("%d\n", daInserire);

	for(int i=0; i<daInserire; i++, p.inseriti++){
		p.array[p.inseriti] = random()%100;
		printf("%d-", p.array[p.inseriti]);
	}
	printf("\n");
}

void pop(){
	int daLeggere = (random()%p.inseriti)+1;
	printf("%d\n", daLeggere);

	for(int i=0; i<daLeggere; i++, p.inseriti--){
		printf("%d-", p.array[p.inseriti-1]);
	}
	printf("\n");
}

void *produttore(void *nome){

	while(1){
		// calcol.o e attesa
		size_t tempo = random()%1000000;
		usleep(tempo);	
		

		pthread_mutex_lock(&lock);
		printf("risveglio e inizio P\n");
		while(p.inseriti==GRANDEZZAPILA){
			pthread_mutex_unlock(&lock);
			sched_yield();
			pthread_mutex_lock(&lock);
		}

		printf("%s:inserisce", (char*)nome);
		push();
		stampa();
		pthread_mutex_unlock(&lock);
	}

}

void *consumatore(void *nome){

	while(1){
		// calcol.o e attesa
		size_t tempo = random()%1000000;
		usleep(tempo);
		
		pthread_mutex_lock(&lock);
		printf("risveglio e inizio C\n");
		while(p.inseriti==0){
			pthread_mutex_unlock(&lock);
			sched_yield();
			pthread_mutex_lock(&lock);
		}

		printf("%s:elimina", (char*)nome);
		pop();
		stampa();
		pthread_mutex_unlock(&lock);
	}
}


int main(int argc, char const *argv[]){

	//imposta il seed per random
	srand(time(NULL));

	p.inseriti = 0;
	p.array = malloc(sizeof(int)*GRANDEZZAPILA);

	printf("inizio main\n");
	pthread_t prod, cons;

	pthread_create(&prod, NULL, produttore, "P");
	pthread_create(&cons, NULL, consumatore, "C");

	pthread_join(prod, NULL);
	pthread_join(cons, NULL);
	free(p.array);
}