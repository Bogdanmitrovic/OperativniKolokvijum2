//Korišćenjem programskog jezika C napisati UNIX/Linux program koji može da
//se prekine samo ako se kombinacija tastera Ctrl-C pritisne pet puta za
//redom. Ukoliko korisnik pritisne kombinaciju tastera Ctrl-Z program na
//ekranu ispisuje koliko puta do sada je pritisnuto Ctrl-C.

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int ctrl_c_count;
int ctrl_c_threshold = 5;

void c_handler(int sig_num)
{
    signal(SIGINT, c_handler);
    ctrl_c_count++;
    if(ctrl_c_count>=ctrl_c_threshold)
    {
        printf("Da li hoces da prekines program? (y/N)\n");
        fflush(stdout);
        char c=getc(stdin);
        if(c=='y'||c=='Y')
        {
            exit(0);
        }
        else
        {
            ctrl_c_count=0;
        }
    }
}
void z_handler(int sig_num)
{
    signal(SIGTSTP,z_handler);
    printf("Ctrl c je pritisnuto %d puta",ctrl_c_count);
    fflush(stdout);
}


int main(int argc, char* argv[])
{
    ctrl_c_count=0;
    signal(SIGINT,c_handler);
    signal(SIGTSTP,z_handler);
    while(1);
}