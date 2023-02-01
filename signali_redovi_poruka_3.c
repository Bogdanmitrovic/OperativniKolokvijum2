// Korišćenjem programskog jezika C napisati UNIX/Linux program koji od 
// korisnika očekuje da sa tastature unese korisničko ime. Ukoliko korisnik ne 
// unese ništa u roku od 30 sekundi proces obaveštava korisnika da je vreme 
// za unos isteklo i izlazi.
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <signal.h>

void alarmHandler(int sig_num)
{
	printf("Nije uneto nista, dovidjenja\n");
	fflush(stdout);
	exit(0);
}
int main(int argc, char const *argv[])
{
	char ime[12];
	printf("Name: ");
	signal(SIGALRM,alarmHandler);
	alarm(30);
	fgets(ime,12,stdin);
	alarm(0);
	puts(ime);
	return 0;
}