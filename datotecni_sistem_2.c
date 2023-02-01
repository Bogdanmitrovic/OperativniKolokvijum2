// Korišćenjem programskog jezika C napisati UNIX/Linux program
// koji prikazuje statistiku za zadati direktorijum (broj
// poddirektorijuma, regularnih datoteka i linkova). Putanja do
// direktorijuma (apsolutna ili relativna) se zadaje kao argument
// komandne linije.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>


int main(int argc, char const *argv[])
{
	char path[256];
	struct stat statbuf;
	if (argc!=2)
	{
		printf("ripcina prijatelji...\n");
		exit(-1);
	}
	DIR* dp;
	struct dirent* de;
	dp=opendir(argv[1]);
	int brfldr = 0; 
	int brdat = 0;
	int brlnk = 0;
	while((de=readdir(dp))!=NULL)
	{
		strcpy(path,argv[1]);
		strcat(path,"/");
		strcat(path,de->d_name);
		
		printf("%s\n",path);

		if ((stat(path,&statbuf))==-1)
		{
			printf("coc\n");
			break;
		}
		if (S_ISDIR(statbuf.st_mode))
		{
			printf("Objekat pod imenom %s je direktorijum\n", de->d_name);
			brfldr++;
		}
		if (S_ISREG(statbuf.st_mode))
		{
			printf("Objekat pod imenom %s je regularan\n", de->d_name);
			brdat++;
		}
		if (S_ISLNK(statbuf.st_mode))
		{
			printf("Objekat pod imenom %s je link\n", de->d_name);
			brlnk++;
		}
	}
	printf("Statistika od %s je %d foldera, %d regularnih objekata i %d linkova\n", argv[1],brfldr,brdat,brlnk);
	return 0;
}