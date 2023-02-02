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

msgctl(msg_queue_id, IPC_RMID, NULL);   // zatvaranje i brisanje message queue-a

// DELJENA MEMORIJA (+ semafori)

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>                    // za semafore, nije nuzno obavezno za deljenu memoriju ali buduci
#include <sys/shm.h>                    // da mora da se sinhronizuje nekako, skoro uvek treba i semafori

#define MEM_KEY 10004                   // kljuc deljene memorije, mora da bude isti u oba procesa
#define SEM_KEY 10002                   // kljuc semafora, oba kljuca mogu da budu neki proizvoljan broj tako oko 10k

union semun                             // obavezno za svaki semafor
{
  int val;
  struct semid_ds *buf;
  ushort *array;
  struct seminfo * __buf;
  void * __pad;
};

int memid = shmget(MEM_KEY, 10*sizeof(int), IPC_CREAT | 0666);
                                        // uzima postojecu ili kreira novu deljenu memoriju za dati kljuc
                                        // moze i samo 0666 ako sigurno vec postoji memorija za dati kljuc
                                        // 1. parametar kljuc, 2. parametar velicina memorije koja treba da se zauzme
                                        // ako vrati < 0 greska

int* shmem = shmat(memid, NULL, 0);     // attach-uje se deljena memorija za pointer
                                        // ukoliko cuvamo intove u deljenoj memoriji, treba int pointer
                                        // da je recimo bio tekst (niz karaktera), bio bi char* shmem
                                        // uvek ide mem. id i NULL, 0

shmdt(shmem);                           // detach, radi se kad nam vise ne treba pristup deljenoj memoriji

shmctl(memid, IPC_RMID, 0);             // uklanja deljenu memoriju

int semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
                                        // semafor sa zadatim kljucem, 1 je broj semafora u nizu, uglavnom treba 1
                                        // ako vrati < 0 greska

union semun opts;                       // instanca unije, opcioni parametri

opts.val = 1;                           // za mutex ide 1, ovde cemo da postavimo inicijalnu vrednost semafora
                                        // lockovanje spusta vrednost za 1, unlockovanje podize vrednost za 1
                                        // ako je semafor 0 i probas da spustis ispod nule, blokirace se proces

semctl(semid, 0, SETVAL, opts);         // semaforu semid, indeks 0 (obicno ima 1 semafor i indeks mu je 0), postavi opts parametre
                                        // koristi se na pocetku da stavis inicijalnu vrednost semafora

struct sembuf lock = {0, -1, 0};        // struktura koja se koristi za lockovanje, -1 jer smanjuje vrednost
struct sembuf unlock = {0, 1, 0};       // isto, samo za unlock, prva nula je indeks semafora na koji se odnosi (citaj uvek treba 0)

semop(semid, &lock, 1);                 // zakljucava jednom, moze i unlock umesto lock

semctl(semid, 0, IPC_RMID, 0);          // prva nula je indeks semafora, uglavnom uvek ide 0

// DATOTECNI SISTEM

#include <sys/types.h>
#include <dirent.h>

DIR* dp = opendir("putanja");           // vraca pointer na otvoren direktorijum, ako je NULL, greska

struct dirent* dirp = readdir(dp);      // DIRectorium ENTry, readdir vraca sledeci direktorijum iz dp, NULL ako nema vise

dirp->d_name                            // ime entry-ja, entry je inace direktorijum ili datoteka (ili jos neke nebitne stvari)

#include <sys/stat.h>                   // ukljucuje i stat, sadrzi neke informacije o dir

struct stat statbuf;
stat(path, &statbuf);                   // cita stat podatke s putanje, ako vrati -1, greska

S_ISDIR(statbuf.st_mode)                // vraca true ako je direktorijum, inace false
S_ISREG(statbuf.st_mode)                // vraca true ako je regularna datoteka, inace false
S_ISLNK(statbuf.st_mode)                // vraca true ako je link, inace false

statbuf.st_size                         // velicina u bajtovima

closedir(dp);                           // na kraju zatvori dp

// SRECNO