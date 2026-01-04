#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


void *produttore(void *arg){

}

void *consumatore(void *arg){

}


int main(int argc, char const *argv[]){
	/* code */
	int nCosumatori = atoi(argv[1]);

	//dichiaro
	pthread_t p;
	pthread_t *c = malloc(sizeof(pthread_t) * nCosumatori);

	//inizializzo 
	pthread_create(&p, NULL, produttore, "P");// produttore

	for(int i = 0; i < nCosumatori; i++){
	    pthread_create(&c[i], NULL, consumatore, i);
	}

	//attesa
	pthread_join(p, NULL);
	for(int i=0; i<nCosumatori; i++){
		pthread_join(c[i], NULL);
	}

	return 0;
}