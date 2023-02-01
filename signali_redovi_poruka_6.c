// Korišćenjem programskog jezika C napisati UNIX/Linux programkoji učitava podatke iz tekstualne
// datoteke cela (red po red) i zatim korišćenjem reda poruka sve parne redove šalje procesu koji
// konvertuje sva slova u velika i zapisuje ih u datoteku pola1, a sve neparne redove procesu koji
// konvertuje sva slova u mala i zapisuje ih u datoteku pola2

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

#define KLJUC_PORUKE 10379
#define MAX_SIZE 100

struct poruka
{
	long type;
	char red[MAX_SIZE];
};

int main(int argc, char const *argv[])
{
	FILE* fp;
	struct poruka bafer;
	int msgid=msgget(KLJUC_PORUKE, IPC_CREAT | 0666);

	int f=fork();
	if(f==0)
	{
		//dete
		fp = fopen("pola1.txt", "w");
		do
		{
			msgrcv(msgid, &bafer, sizeof(bafer), 1, 0);
			for(int j=0; j<strlen(bafer.red); j++)
			{
				bafer.red[j]=toupper(bafer.red[j]);
			}
			fputs(bafer.red, fp);
		}
		while(strcmp(bafer.red, "END")!=0);
		fclose(fp);
		exit(0);
	}
	f=fork();
	if(f==0)
	{
		//drugo dete
		fp = fopen("pola2.txt", "w");
		do
		{
			msgrcv(msgid, &bafer, sizeof(bafer), 2, 0);
			for(int j=0; j<strlen(bafer.red); j++)
			{
				bafer.red[j]=tolower(bafer.red[j]);
			}
			fputs(bafer.red, fp);
		}
		while(strcmp(bafer.red, "end")!=0);
		fclose(fp);
		exit(0);
	}
	fp = fopen("zacitanje.txt", "r");
	int i=0;
	while(!feof(fp))
	{
		fgets(bafer.red, MAX_SIZE, fp);
		bafer.type=1+i%2;
		msgsnd(msgid, &bafer, sizeof(bafer), 0);
		i++;
	}
	fclose(fp);
	strcpy(bafer.red, "END");
	bafer.type=1;
	msgsnd(msgid, &bafer, sizeof(bafer), 0);
	strcpy(bafer.red, "END");
	bafer.type=2;
	msgsnd(msgid, &bafer, sizeof(bafer), 0);
	wait(NULL);
	wait(NULL);
	msgctl(msgid, IPC_RMID, 0);
	return 0;
}