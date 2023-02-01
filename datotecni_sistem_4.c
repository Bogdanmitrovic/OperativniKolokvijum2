
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
	
	DIR* dp;
	struct stat statbuf;
	struct dirent* dirp;
	dp=opendir(argv[1]);
	if(dp==NULL){
		printf("AJSDHLIDHASKUHFIL");
		exit(-1);
	}
	while((dirp=readdir(dp))!=NULL)
	{
		char path[256];
		strcpy(path,argv[1]);
		strcat(path,"/");
		strcat(path,dirp->d_name);
		if((stat(path,&statbuf))==-1)continue;
		if(IS_DIR(statbuf.st_mode))
		{
			printf("%s %d direktorijum %d bajtova\n",path,dirp->d_name,statbuf.st_size);
		}
	}
	return 0;
}