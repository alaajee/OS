#include <cpu.h>
#include <stdio.h>
#include "time.h"

// Déclarations des fonctions externes
extern void idle();
extern void proc1();
extern void proc2();
extern void proc3();
extern void proc4();
extern void proc5();
extern void proc6();
extern void proc7();
extern void ctx_sw(int *adresse_ancienne, int *adresse_nouvelle);
extern int32_t creer_processus(void (*code)(void), char *nom);
extern void ordonnance();

// Définition des états possibles des processus
typedef enum {
    ELU, ACTIVABLE, ENDORMI
} Etat_processus;

// Définition de la structure processus
struct processus {
    int pid;
    char nom_processus[20];
    Etat_processus etat;
    int registre[5];
    int pile[512];
    char tampon_sortie[256];
    struct processus *suiv;
};

// Structure pour gérer la liste des activables
struct ListeActivables {
    struct processus *tete;  // Pointeur vers la tête de la liste
    struct processus *queue; // Pointeur vers la queue de la liste
};
// Structure pour gérer la liste des activables
struct ListeEndormis {
    struct processus *tete;  // Pointeur vers la tête de la liste
    struct processus *queue; // Pointeur vers la queue de la liste
};

// Déclarations globales externes
extern struct ListeActivables listeActivables;
extern struct ListeEndormis ListeEndormis;
extern struct processus struct_idle;
extern struct processus *actif;
extern int32_t pid;
