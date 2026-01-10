#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>


#define SIZEMAX 10

typedef struct buffer{
	int *array;
	int count;
}buffer;

buffer b;
int id;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


//PRIMO MODO
int myPush(){
	int daInserire = random()%100;
	b.array[b.count] = daInserire;
	b.count++;
	return daInserire;
}

void *produttore(void* numero){
	for(int i=0; i<5; i++){
		int idElemento = 0;
		pthread_mutex_lock(&lock);
		idElemento = id;
		id++;
		printf("Produttore %d: generato ID=%d",*(int*)numero, idElemento);
		pthread_mutex_unlock(&lock);

		usleep(random()%1000000);

		pthread_mutex_lock(&lock);
		while(b.count==SIZEMAX){
			pthread_mutex_unlock(&lock);
			sched_yield();
			pthread_mutex_lock(&lock);
		}
		printf("Produttore %d: inserito ID=%d con valore %d\n",*(int*)numero, idElemento, myPush());
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}

//-------//
void *consumatore(void* numero){
	while(1){
		usleep(random()%1000000);
		pthread_mutex_lock(&lock);
		if(b.count==SIZEMAX){
			printf("CANCELLO TUTTA LA PILA PER NUOVI INSERIMENTI\n");
			b.count = 0;
		}
		pthread_mutex_unlock(&lock);

	}
	
	return NULL;
}


int main(int argc, char const *argv[]){
	if(argc<3){
		fprintf(stderr, "devi inserire altri elementi\n");
		exit(1);
	}

	int produttori = atoi(argv[1]);
	int consumatori = atoi(argv[2]);
	b.array = malloc(sizeof(int)*SIZEMAX);
	b.count = 0;
	id = 0;
	srandom(time(NULL));

	//allocazione memoria
	pthread_t *p = malloc(sizeof(pthread_t)*produttori);
	pthread_t *c = malloc(sizeof(pthread_t)*consumatori);


	//creazione id produttori && consumatori
	int *idProduttori = malloc(produttori * sizeof(int));
	int *idConsumatori = malloc(consumatori * sizeof(int));

	//creazione produttori
	for(int i=0; i<produttori; i++){
		idProduttori[i]=i+1;
		pthread_create(&p[i],NULL, produttore, &idProduttori[i]);
	}

	//creazione consumatori
	pthread_t *copia = c;
	for(int i=0; i<consumatori; i++, copia++){
		idConsumatori[i]=i+1;
		pthread_create(copia,NULL, consumatore, &idConsumatori[i]);
	}

	// aspetto la fine dei thread
	for(int i=0; i<produttori; i++){
		pthread_join(p[i], NULL);
	}
	for(int i=0; i<consumatori; i++){
		pthread_join(c[i], NULL);
	}

	free(p);
	free(c);
	free(idProduttori);
	free(idConsumatori);
	free(b.array);
}