#include <stdio.h>
#include "time.h"

#define MAX_PILE 512
#define MAX_TABLE 2

typedef enum {
    ELU, 
    ACTIVABLE
} etat_proc;

typedef struct _Identification_processus {
    int pid;
    char nom[20];
    etat_proc etat;
    int registre[5];
    int pile[MAX_PILE];
} Identification_processus;

extern void idle(void);
extern void proc1(void);
extern void ctx_sw(int *adresse_ancien_proc, int *adresse_nouveau_proc);

extern Identification_processus *table_processus[MAX_TABLE];
extern Identification_processus struct_idle;
extern Identification_processus struct_proc1;
