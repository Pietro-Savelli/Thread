#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "queue.h"

#define QUANTUM 200000
#define SLEEP 100000
#define SIZEMAX 1000

int cpu;
int processiEsecuzione;
Queue *q;
int count;
int totaliProdotti;
int totaliFiniti;
pthread_mutex_t lock =  PTHREAD_MUTEX_INITIALIZER;
float TTtot;
float TRtot;


void *produttore(void* nome){
	for(int i=0; i<processiEsecuzione; i++){

		pthread_mutex_lock(&lock);
		while(count>=SIZEMAX){
			pthread_mutex_unlock(&lock);
			sched_yield();
			pthread_mutex_lock(&lock);
		}

		Process *p = malloc(sizeof(Process));
		p->id = totaliProdotti;
		p->exec_time = random()%1000000;
		gettimeofday(&(p->arrival), NULL);
		p->start.tv_sec = 0; //mi serve per vedere se e' la prima esecuzione
		p->end.tv_sec = 0;
		totaliProdotti++;

		enqueue(q, p);
		count++;
		printf("[PRODUTTORE] Ho creato il job %ld con execTime %ld ns\n", p->id, p->exec_time);
		//rilascia
		pthread_mutex_unlock(&lock);
		usleep(SLEEP);
	}

	return NULL;
}



void *consumatore(void* id){
	while(1){
		usleep(random()%100000);
		int tempoEsecuzione = 0;
		int finito = 0;
		float RT = 0;
		float TT = 0;
		int is_first_run = 0;

		pthread_mutex_lock(&lock);
		//casi limite
		if(count==0 && totaliProdotti==processiEsecuzione){
			pthread_mutex_unlock(&lock);
			break;
		}

		while(count==0){
			pthread_mutex_unlock(&lock);
			sched_yield();
			pthread_mutex_lock(&lock);
			
			// ricontrollo la condizione di terminazione dopo il risveglio
            if(count==0 && totaliProdotti==processiEsecuzione){
                pthread_mutex_unlock(&lock);
                return NULL;
            }
		}

		// presa del processo 
		Process *p = dequeue(q);
		count--;
		if(p->exec_time>QUANTUM){
			tempoEsecuzione = QUANTUM;
			p->exec_time = ((p->exec_time)-QUANTUM);
		}
		else{
			tempoEsecuzione = p->exec_time;
			finito = 1;
			totaliFiniti++;
		}
		pthread_mutex_unlock(&lock);
		// rilascio + esecuzione
		usleep(tempoEsecuzione);

		if(p->start.tv_sec==0){// controllo se e' la prima volta che viene selezionato
			gettimeofday(&(p->start), NULL);
			RT = (p->start.tv_sec - p->arrival.tv_sec)*1000000 + p->start.tv_usec - p->arrival.tv_usec;
			printf("Response time di processo id: %ld e' %f\n", p->id, RT/1000000);
			is_first_run = 1;
		}

		if(finito){
			gettimeofday(&(p->end), NULL);
			TT = (p->end.tv_sec - p->arrival.tv_sec)*1000000 + p->end.tv_usec - p->arrival.tv_usec;
			printf("Turnaround time di processo id: %ld e' %f\n", p->id, TT/1000000);
		}

		pthread_mutex_lock(&lock);
		if(is_first_run) TRtot += RT;
        if(finito)       TTtot += TT;
		if(!finito){
			enqueue(q, p);
			count++;
		}
		pthread_mutex_unlock(&lock);
	}

	return NULL;
}	



int main(int argc, char const *argv[]){

	if(argc<3){
		fprintf(stderr, "inserisci altri parametri\n");
		exit(1);
	}

	srandom(time(NULL));
	processiEsecuzione = atoi(argv[1]);
	cpu = atoi(argv[2]);
	count = 0;
	totaliProdotti = 0;
	totaliFiniti = 0;
	TTtot = 0;
	TRtot = 0;

	q = malloc(sizeof(Queue));
    initializeQueue(q);

	pthread_t prod;
	pthread_t *cons = malloc(sizeof(pthread_t)*cpu);

	pthread_create(&prod, NULL, produttore, "P");

	int *idCpu = malloc(sizeof(int)*cpu);
	for(int i=0; i<cpu; i++){
		idCpu[i] = i+1;
		pthread_create(&cons[i], NULL, consumatore, &idCpu[i]);
	}

	pthread_join(prod, NULL);
	for(int i=0; i<cpu; i++){
		pthread_join(cons[i], NULL);
	}

	free(idCpu);
	free(cons);

	printf("TT medio:%f secondi, RT medio:%f secondi\n", TTtot/(totaliFiniti*1000000), TRtot/(totaliFiniti*1000000));
}


