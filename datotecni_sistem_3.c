// Korišćenjem programskog jezika C napisati Linux/UNIX program koji u zadatom direktorijumu
// (naziv direktorijuma se zadaje kao prvi argument komandne linije) pronalazi i štampa nazive svih
// datoteka koje u imenu sadrže string koji se zadaje kao drugi argument komandne linije.
// Pretraživanje se obavlja rekurzivno u zadatom direktorijumu i svim njegovim poddirektorijumima

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

void funkcija(char* folder, char* str)
{
	DIR* dp;
	struct dirent* de;
	struct stat statbuf;
	dp = opendir(folder);
	printf("sad je u folder %s\n",folder);
	if(dp==NULL)
		return;
	while((de = readdir(dp))!=NULL)
	{
		char tmp [1024] = "";
		strcat(tmp, folder);
		strcat(tmp,"/");
		strcat(tmp, de->d_name);

		if(strstr(de->d_name, str)!=0)
		{
			printf("Pronadjen %s\n", de->d_name);
		}
		if(stat(tmp,&statbuf)==-1)return;

		if (S_ISDIR(statbuf.st_mode) && strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0)
		{
			funkcija(tmp, str);
		}
	}
	closedir(dp);
}
int main(int argc, char const *argv[])
{
	funkcija(argv[1], argv[2]);
	return 0;
}