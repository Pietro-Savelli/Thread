
//PROVA A RIFARE CON #define BUFFER_SIZE 10 un buffer limitato a 10(ciclico)


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

# define SIZEMAX 10

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t disponibile = PTHREAD_COND_INITIALIZER;


int *buffer;
int coda;
int testa;
int elementiAttuali;
int daInserire;
int letti;
int totali;
int scartatitotali;

void *produttore(void *arg){
	int id = *(int*)arg; 
	int scartati = 0;

	for(int i=0; i<daInserire; i++){
		usleep(random()%99999);
		int valore = rand() % 100;
		int tentativo = 0;
		int inserito = 0;

		for(; tentativo<3; tentativo++){
			pthread_mutex_lock(&lock);

			if(elementiAttuali == SIZEMAX){
				// Se non è l'ultimo tentativo, aspetta un po'
                if(tentativo < 3-1){
					pthread_mutex_unlock(&lock);
                    usleep(50000);  // Aspetta 50ms prima di riprovare
                }
				pthread_mutex_unlock(&lock);
				continue;
			}
			else{
				buffer[coda] = valore;
				coda = (coda+1) % SIZEMAX;
				elementiAttuali++;
				printf("produttore %d: elemento %d con valore %d al tentativo %d\n", id, i+1, valore, tentativo+1);
				inserito = 1;
				pthread_mutex_unlock(&lock);
				break;
			}
		}
		
		if(!inserito){
			pthread_mutex_lock(&lock);
			totali--;
			scartatitotali++;
			pthread_mutex_unlock(&lock);
            scartati++;
            printf("Produttore %d: SCARTATO elemento %d (valore=%d) dopo %d tentativi\n", id, i+1, valore, 3);
        }
	}
	return NULL;
}

void *consumatore(void *arg){
	int id = *(int*)arg;

	while(1){
		usleep(random()%99999);

		pthread_mutex_lock(&lock);

		//attendi se il buffer è vuoto e la produzione non è temrminata
		while(elementiAttuali==0 && letti<totali){
			pthread_mutex_unlock(&lock);
			sched_yield();
			pthread_mutex_lock(&lock);
		}

		if(elementiAttuali==0 && letti>=totali){
			pthread_mutex_unlock(&lock);
			break;
		}

		printf("consumatore %d: toglie valore %d\n", id, buffer[testa]);
		testa = (testa+1)%SIZEMAX;
		elementiAttuali--;

		letti++;
		pthread_mutex_unlock(&lock);
	}

	return NULL;
}


int main(int argc, char const *argv[]){

 	srand(time(NULL));
	

	int produttori = atoi(argv[1]);
	int consumatori = atoi(argv[2]);
	daInserire = atoi(argv[3]);

	elementiAttuali = 0;
	testa = 0;
	coda = 0;
	buffer = malloc(sizeof(int)*SIZEMAX);
	letti = 0;
	totali = daInserire*produttori;


	pthread_t *p = malloc(sizeof(pthread_t)*produttori);
	pthread_t *c = malloc(sizeof(pthread_t)*consumatori);

	//creazione produttori && consumatori
	int *idProduttori = malloc(produttori * sizeof(int));
	int *idConsumatori = malloc(consumatori * sizeof(int));
	
	for(int i=0; i<produttori; i++){
		idProduttori[i] = i+1;
		pthread_create(&p[i], NULL, produttore, &idProduttori[i]);
	}
	for(int i=0; i<consumatori; i++){
		idConsumatori[i] = i+1;
		pthread_create(&c[i], NULL, consumatore, &idConsumatori[i]);
	}

	//attesa
	for(int i=0; i<produttori; i++){
		pthread_join(p[i], NULL);
	}
	for(int i=0; i<consumatori; i++){
		pthread_join(c[i], NULL);
	}

	printf("elementi prodotti totali inizali:%d --->letti totali:%d --->scartati:%d", produttori*daInserire, letti, scartatitotali);

	free(buffer);
    free(p);
    free(c);
    pthread_mutex_destroy(&lock);
}
