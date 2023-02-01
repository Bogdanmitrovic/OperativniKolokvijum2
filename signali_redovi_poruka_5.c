// Korišćenjem programskog jezika C napisati UNIX/Linux program koji simulira
// problem proizvođač/potrošač korišćenjem redova poruka (message-queues).
// Glavni program se deli u dva procesa. Prvi proces (proizvođač) kreira N
// slučajnih pozitivnih celih brojeva i šalje ih drugom procesu. N se
// određuje tokom izvršenja, takođe kao slučajan pozitivan ceo broj. Po
// završetku slanja, prvi proces šalje -1 kao kod za kraj. Drugi proces
// (potrošač) preuzima poslate brojeve iz poruka i štampa ih na standardnom
// izlazu.

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>

#define QUEUE_ID 12345

struct poruka
{
    long type;
    int broj;
};

int main(int argc, char* argv[])
{
    int pid;
    struct poruka buffer;
    pid=fork();
    if(pid<0) exit(-1);
    if(pid==0)
    {
        //dete
        int msgQueueId=msgget(QUEUE_ID,IPC_CREAT | 0666);           // moze i jedan msgget iznad forka
        if(msgQueueId<0)
            printf("greska pri kreiranju msg queue\n");
        srand(time(0));
        int n=rand()%10+10;
        int broj;
        while(n>0)
        {
            broj=rand()%100+1;
            buffer.type=1;
            buffer.broj=broj;
            if(msgsnd(msgQueueId, &buffer, sizeof(buffer),0)<-1)
                printf("greska pri slanju\n");
            n--;
        }
        buffer.type=1;
        buffer.broj=-1;
        if(msgsnd(msgQueueId, &buffer, sizeof(buffer),0)<-1)
            printf("greska pri slanju\n");
        exit(0);
    }
    //roditelj
    int msgQueueId=msgget(QUEUE_ID, IPC_CREAT | 0666);
    if(msgQueueId<0)
        printf("greska pri kreiranju msg queue\n");
    do
    {
        //procitaj broj
        if(msgrcv(msgQueueId, &buffer, sizeof(buffer),0,0)<-1)
            printf("greska pri primanju\n");
        printf("%d\n",buffer.broj);
    }
    while(buffer.broj!=-1);
    wait(NULL);
    msgctl(msgQueueId, IPC_RMID, 0);
    return 0;
}