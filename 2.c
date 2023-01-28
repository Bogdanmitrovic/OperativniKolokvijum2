// Korišćenjem programskog jezika C napisati UNIX/Linux program koji svom
// procesu detetu korišćenjem redova poruka identifikovan brojem 12010
// prosleđuje karaktere unete sa tastature. Poruke su tipa 4. Proces dobijene
// vrednosti upisuje u datoteku karakteri.txt. Komunikacija se prekida kada
// korisnik unese bilo koji broj.

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <time.h>
#include <sys/wait.h>

#define QUEUE_ID 12010

struct poruka
{
    long type;
    char karakter;
};

int main(int argc, char* argv[])
{
    char c;
    int pid;
    int msgQueueId;
    struct poruka buffer;

    pid=fork();
    if(pid<0) exit(-1);
    if(pid==0)
    {
        //dete
        char x;
        FILE* f = fopen("karakteri1.txt","w");
        msgQueueId=msgget(QUEUE_ID, IPC_CREAT | 0666);
        if(msgQueueId<-1) exit(-1);
        do
        {
            msgrcv(msgQueueId, &buffer, sizeof(buffer),0, 0);
            x=buffer.karakter;
            fprintf(f, "%c\n", x);
        } while (x<'0'||x>'9');
        fclose(f);
        exit(0);
    }
    //roditelj
    msgQueueId=msgget(QUEUE_ID, IPC_CREAT | 0666);
    if(msgQueueId<-1) exit(-1);
    do
    {
        c=getc(stdin);
        buffer.type=4;
        buffer.karakter=c;
        msgsnd(msgQueueId,&buffer,sizeof(buffer),0);
    }
    while(c<'0'||c>'9');
    wait(NULL);
    msgctl(msgQueueId, IPC_RMID, 0);
    return 0;
}