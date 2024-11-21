#include <cpu.h>
#include <stdio.h>

#include "time.h"

extern void idle();
extern void proc1();
extern void proc2();
extern void proc3();
extern void proc4();
extern void proc5();
extern void ctx_sw(int * adresse_ancienne , int *  adresse_nouvelle);
extern int32_t creer_processus(void (*code)(void), char *nom);

typedef enum {
    ELU,ACTIVABLE,ENDORMI
}Etat_processus;

typedef struct {
    int pid;
    char nom_processus[20];
    Etat_processus etat;
    int registre[5];
    int pile[512];
    char tampon_sortie[256];
}processus;

extern processus* TableProcessus[6];
extern processus struct_pile;
extern processus struct_idle;
extern processus * actif ;
extern int32_t pid;