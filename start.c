#include <cpu.h>
#include <inttypes.h>
#include "time.h"
#include "processus.h"

// External function declaration
extern void traitant_IT_32(void);


uint32_t fact(uint32_t n) {
    uint32_t res;
    if (n <= 1) {
        res = 1;
    } else {
        res = fact(n - 1) * n;
    }
    return res;
}

void kernel_start(void) {
    uint32_t x = fact(5);
    (void)x;  // Placeholder to suppress unused variable warning
    efface_ecran();

    // init_traitant_IT(32, traitant_IT_32);
    // configure_clock();
    // masque_IRQ(0, false); 
    
    // Prevent leaving kernel_start
    cli();

    memset(&struct_idle, 0, sizeof(struct_idle));  // Initialiser à zéro

    struct_idle.pid = 1;
    strcpy(struct_idle.nom, "idle");
    struct_idle.etat = ACTIVABLE;

    // Initialisation de la pile
    memset(struct_idle.pile, 0, sizeof(struct_idle.pile)); // Effacer la pile
    struct_idle.registre[1] = (int)&struct_idle.pile[MAX_PILE - 1]; // %esp pour idle

    memset(&struct_proc1, 0, sizeof(struct_proc1));
    struct_proc1.pid = 1;
    strcpy(struct_proc1.nom, "proc1");
    struct_proc1.etat = ACTIVABLE;
    struct_proc1.registre[1] = (int)(&struct_proc1.pile[511]); // %esp pour proc1
    struct_proc1.pile[511] = (int)proc1; // Adresse de retour pour le processus proc1
    // Use already-defined processes
    table_processus[0] = &struct_idle;
    table_processus[1] = &struct_proc1;

    idle();

    while (1) {
        hlt();
    }
}
