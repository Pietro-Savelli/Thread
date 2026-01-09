#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define GRANDEZZAPILA 10

typedef struct pila{
	int *array;
	int inseriti;
};

pila p;

void stampa(){
	for(int i=0; i<p.inseriti; i++){
		printf("%d-"p.array[i]);
	}
	printf("\n");
}

void push(){
	int daInserire = (random()%p.inseriti)+1;

	for(int i=0; i<daInserire; i++ p.inseriti++){
		p.array[p.inseriti] = random()%100;
		printf("%d-"p.array[p.inseriti]);
	}
	printf("\n");
}

void pop(){
	int daLeggere = (random()%p.inseriti)+1;

	for(int i=0; i<daLeggere; i++ p.inseriti--){
		printf("%d-"p.array[p.inseriti]);
	}
	printf("\n");
}

void *produttore(void *nome){

	while(1){
		// calcol.o e attesa
		size_t tempo = random()%1000000;
		usleep(tempo);

		while(p.inseriti==GRANDEZZAPILA){
			pthread_yield();
		}

		printf("%c:inserisce" (char)nome);
		push();
		stampa();
	}

}

void *consumatore(void *nome){

	while(1){
		// calcol.o e attesa
		size_t tempo = random()%1000000;
		usleep(tempo);

		while(p.inseriti==0){
			pthread_yield();
		}

		printf("%c:elimina" (char)nome);
		pop();
		stampa();
	}
}


int main(int argc, char const *argv[]){

	//imposta il seed per random
	initstate(time(NULL));

	p.inseriti = 0;
	p.array = malloc(sizeof(int)*GRANDEZZAPILA);

	pthread_t prod, cons;

	ptread_create(&prod, NULL, produttore, "P");
	ptread_create(&cons, NULL, consumatore, "C");

	ptread_join(prod, NULL);
	ptread_join(cons, NULL);
}