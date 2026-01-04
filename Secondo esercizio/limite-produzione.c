
//PROVA A RIFARE CON #define BUFFER_SIZE 10 un buffer limitato a 10(ciclico)


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int elementi;
int *array;
int indice;

void *produttore(void *arg){
	for(int i=0; i<elementi; i++){
		unsleep(random()%1000000);

		int valore = rand() % 100;

		pthread_mutex_lock(&lock);
		array[indice] = valore;
		indice++;
		pthread_mutex_unlock(&lock);
	}
}

void *consumatore(void *arg){
	usleep(random()%1000000);
	while(indice!=0){
		pthread_mutex_lock(&lock);
		indice--;
		pthread_mutex_unlock(&lock);
	}
}


int main(int argc, char const *argv[]){

 	srand(time(NULL));

	int produttori = atoi(argv[1]);
	int consumatori = atoi(argv[2]);
	elementi = atoi(argv[3]);

	array = malloc(sizeof(int)*produttori*elementi);
	indice = 0;
	pthread_t *p = malloc(sizeof(pthread_t)*produttori);
	pthread_t *c = malloc(sizeof(pthread_t)*consumatori);

	//creazione produttori
	for(int i=0; i<produttori; i++){
		pthread_create(&p[i], NULL, produttore, NULL);
	}
	for(int i=0; i<consumatori; i++){
		pthread_create(&c[i], NULL, consumatore, NULL);
	}

	//attesa
	for(int i=0; i<produttori; i++){
		pthread_join(p[i], NULL);
	}
	for(int i=0; i<consumatori; i++){
		pthread_join(c[i], NULL);
	}

	free(buffer);
    free(produttori);
    free(consumatori);
    pthread_mutex_destroy(&lock);
}