#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <error.h>

int i=0;

struct Data
{
    char filename[255];
    long filesize;
};

struct Data niz[20] = {0}; 

void fun(char* path)
{
    if(strcmp(path,"..") == 0 || strcmp(path,".") == 0) 
    {
        puts("Izlazimo");
        return;
    }
    char stari[255];
    getcwd(stari,255);
    chdir(path);
    DIR* dp = opendir(".");
    if (!dp)
    {
        perror(path);
    }
    
    struct dirent* current;
    while (1)
    {
        current = readdir(dp);
        if(!current)
        {
            break;
        }
        struct stat s;
        if(stat(current->d_name,&s) == -1)
        {
            printf("%s\n",current->d_name);
            perror("STAT GRESKA");
            exit(1);
        }
        if(S_ISREG(s.st_mode))
        {
            if (strchr(current->d_name,'p'))
            {
                strcpy(niz[i].filename,current->d_name);
                niz[i].filesize = s.st_size;
                i++;
                printf("Dodao sam\n");
            }
        }
        else if (S_ISDIR(s.st_mode))
        {
            printf("Ulazimo u novi direktorijum %s\n",current->d_name);
            fun(current->d_name);
        }
    }
    closedir(dp);
    chdir(stari);
}

int main(int argc,char* argv[])
{
    fun(argv[1]);
    for(int j=0;j<i;j++)
    {
        for(int k=0; k<i; k++)
        {
            if (niz[k].filesize<niz[k+1].filesize)
            {
                struct Data save = niz[k+1];
                niz[k+1] = niz[k];
                niz[k]=save;
            }
        }
    }
    for(int m=0;m<i;m++)
    {
        printf("%s\t%ld\n",niz[m].filename,niz[m].filesize);
    }
    return 0;
}
