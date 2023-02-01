// Korišćenjem programskog jezika C napisati UNIX/Linux program
// koji u zadatom direktorijumu lista nazive svih datoteka koje
// u svom nazivu sadrže zadati string. Putanja do direktorijuma
// i string koj se traži zadaju se kao argument komandne linije.
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char const *argv[])
{
	if(argc!=3)
	{
		printf("los broj arg\n");
		exit(-1);
	}
	DIR* dp;
	struct dirent* de;
	dp=opendir(argv[1]);
	if(dp==NULL)
	{
		printf("nema direktorijum\n");
		exit(-1);
	}
	while((de=readdir(dp))!=NULL)
	{
		if(strstr(de->d_name,argv[2])!=NULL)
		{
			printf("%s\n",de->d_name);
		}
	}
	return 0;
}