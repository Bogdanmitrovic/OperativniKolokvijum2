// prirucnik 2. kol

// SIGNALI

#include <signal.h>                     // library za signale

void signal_handler(int sig_num)        // funkcija koja "handluje" signal, moze kako god
{                                       // da se zove, trebalo bi da ima int sig_num
    signal(SIGINT, signal_handler);     // funkcija za postavljanje handler funkcije
    /*neki kod*/                        // signal(signal_koji_handluje, funkcija_koja_handluje)
                                        // kad se postavi, handler vazi jednom i zato se u
                                        // funkciji opet postavlja
                                        // CTRL C je SIGINT, CTRL Z je SIGTSTP, alarm je SIGALRM
}
alarm(30);                              // salje alarm signal za 30 sekundi
alarm(0);                               // iskljucuje postojeci alarm


// REDOVI PORUKA

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct poruka                           // uvek se pravi struktura za slanje poruka
{
    long mtype;                         // long tip, oznacava kog tipa je poruka (uvek > 0)
    char mtext[10];                     // moze char int char[] long bilo sta
};                                      // BITNO da se stavi odgovarajuci tip, sta se salje

int msg_queue_id = msgget(10104, 0666 | IPC_CREAT);
                                        // preko ovoga se pristupa redu poruka
                                        // 10104 moze bilo koji broj tako oko 10000
                                        // ako u 2 procesa bude isti broj, isti red koriste
                                        // IPC_CREAT | 0666 moze i za pravljenje i za uzimanje redova
                                        // ako 100% znas da imas napravljen vec red, moze i samo 0666

struct poruka buffer;                   // buffer u koji se upisuju podaci poruke pre slanja

msgsnd(msg_queue_id, &buffer, 10, 0);   // slanje poruke kroz red poruka
                                        // msg_queue_id je id reda poruka dobijen kroz msgget
                                        // &buffer je adresa poruke koju saljemo
                                        // 11 je ovde velicina polja koje se salje, moze da se koristi
                                        // i strlen(mtext)+1 ako je tekst, ako bas ne znas mozes
                                        // i sizeof(buffer) sto je malo vise nego sto treba ali ce uvek
                                        // da radi bez greske i ne moras da vodis racuna
                                        // 0 je flag, 0 znaci da blokira ako nema mesta u redu, stavis uvek 0

msgrcv(msg_queue_id, &buffer, 10, 0, 0) // citanje poruke iz reda
                                        // 10 je velicina, 0 je tip poruke koji cekas, druga 0 flag, uvek 0
                                        