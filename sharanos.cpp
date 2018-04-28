#include<stdio.h>

#include<unistd.h>

#include<stdlib.h>

#include<malloc.h>

#include<time.h>

#include<pthread.h>

#include<semaphore.h>

struct Process {

	int Allocated,id,Maximum,Completed;

	sem_t se;

};

sem_t se;

int P,Q;

int i=0;

int Safe_Flag=0,Count=0,Completed=0;

struct Process T[10];

pthread_t V[10];

pthread_mutex_t Lok;

void *Processor(struct Process *S) {

	

	sem_wait(&S->se);

	pthread_mutex_lock(&Lok);

	printf("\nProcess-%d Took the Resources",S->id);

	P+=S->Allocated;

	printf("\nTotal Number of Resources %d\n",P);

	pthread_mutex_unlock(&Lok);

	sem_post(&se);

}



void main() {

	

	printf("Enter no.of Resources Present in System :");

	scanf("%d",&P);

	printf("Enter no.of Processes :");

	scanf("%d",&Q);

	sem_init(&se,0,0);

	for(i=0;i<Q;i++) {

			printf("Enter Allocated Resources for Process-%d :",i+1);

			scanf("%d",&T[i].Allocated);

			printf("Enter Maximum Resources Needed :");

			scanf("%d",&T[i].Maximum);

			sem_init(&T[i].se,0,0);

			T[i].Completed=0;

			T[i].id=i+1;

			if(T[i].Maximum>P) {

				Safe_Flag=1;

			}

			Count+=T[i].Maximum;

	}

	if(Count>P+Q) {

		Safe_Flag=1;

	}

	if(Safe_Flag==1) {

		printf("\nSystem is not in a Safe State");

	}

	else {

		printf("\nSystem is in Safe State \nProessing Started.");

		for(i=0;i<Q;i++) {

			pthread_create(&V[i],NULL,Processor,&T[i]);

		}

		i=0;

		while(Completed!=Q) {

			if(T[i].Maximum-T[i].Allocated<=P) {

				printf("\nProcess-%d Requesting the Resources",i+1);

				sem_post(&T[i].se);

				Completed+=1;

				sem_wait(&se);

			}

			i=(i+1)%Q;

		}

		for(i=0;i<Q;i++) {

			pthread_join(V[i],NULL);

		}

		printf("\nAll Process Are Executed");

	}

} 
