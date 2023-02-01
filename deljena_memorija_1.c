// Korišćenjem programskog jezika C napisati UNIX/Linux program
// koji se deli u dva procesa (proizvođač - potrošač) koji
// komuniciraju koristeći ograničeni kružni bafer u deljivoj
// memoriji. Kružni bafer je veličine 10 celih brojeva. Ovi
// procesi sinhronizuju svoje aktivnosti (upis u bafer i čitanje
// iz bafera) koristeći semafore.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define FULL_KEY 100001
#define EMPTY_KEY 10002
#define MUTEX_KEY 10003
#define MEM_KEY 10004
#define N 10
union semun
{
	int val;
	struct semid_ds *buf;
	ushort *array;
	struct seminfo * __buf;
	void * __pad;
};
int main()
{
	int fullid, emptyid, mutexid, memid;
	int* shmem;
	int i;
	struct sembuf lock={0,-1,0};
	struct sembuf unlock={0,1,0};
	union semun opts;

	fullid=semget(FULL_KEY,1,IPC_CREAT|0666);
	if(fullid<0)
	{
		printf("Greska: kreiranje full\n");
		exit(1);
	}
	opts.val=0;
	if(semctl(fullid,0,SETVAL,opts)<0)
	{
		printf("Greska: inicijalizacija full\n");
		exit(1);
	}
	emptyid = semget(EMPTY_KEY,1,IPC_CREAT|0666);
	if (emptyid<0)
	{
		printf("Greska: kreiranje empty\n");
		exit(1);
	}
	opts.val=N;
	if(semctl(emptyid,0,SETVAL,opts)<0)
	{
		printf("Greska: inicijalizacija empty");
		exit(1);
	}
	mutexid=semget(MUTEX_KEY,1,IPC_CREAT|0666);
	if (mutexid<0)
	{
		printf("Greska: kreiranje mutex-a\n");
		exit(1);
	}
	opts.val = 1;
	if (semctl(mutexid,0,SETVAL,opts)<0)
	{
		printf("Greska: inicijalizacija mutex-a\n");
		exit(1);
	}
	memid = shmget(MEM_KEY,N*sizeof(int),IPC_CREAT|0666);
	if (memid<0)
	{
		printf("Greska: kreiranje deljene memorije\n");
		exit(1);
	}	
	int pid=fork();
	if (pid==0)
	{
		printf("uso u dete\n");
		int write_pos=0;
		shmem = shmat(memid,NULL,0);
		if(shmem==NULL)
		{
			printf("Greska: mapiranje memorije\n");
			exit(1);
		}
		for (i = 0; i < 20; ++i)
		{
			semop(emptyid, &lock, 1);
			semop(mutexid, &lock, 1);
			int p=rand()%100+1;
			shmem[write_pos]=p;
			write_pos=(write_pos+1)%N;
			printf("%d upisan\n",p);
			semop(mutexid, &unlock, 1);
			semop(fullid, &unlock, 1);
			sleep(rand()%2);
		}
		shmdt(shmem);
		exit(0);
	}
	printf("uso u roditelja\n");
	int read_pos=0;
	shmem=shmat(memid,NULL,0);
	if(shmem==NULL)
	{
		printf("Greska: mapiranje memorije\n");
		exit(1);
	}
	for(i=0;i<20;i++)
	{
		semop(fullid, &lock, 1);
		semop(mutexid, &lock, 1);
		int r=shmem[read_pos];
		printf("%d procitan\n",r);
		read_pos=(read_pos+1)%N;
		semop(mutexid, &unlock, 1);
		semop(emptyid, &unlock, 1);
		sleep(rand()%5);
	}
	shmdt(shmem);
	wait(NULL);
	semctl(fullid, 0, IPC_RMID, 0);
	semctl(emptyid,0, IPC_RMID, 0);
	semctl(mutexid,0, IPC_RMID, 0);
	shmctl(memid, IPC_RMID, 0);
	return 0;
}