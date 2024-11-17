#include "processus.h"
// Table des processus
Identification_processus* table_processus[MAX_TABLE];

// Définitions des processus
Identification_processus struct_idle;
Identification_processus struct_proc1;

void idle(void) {

    printf("[idle] je tente de passer la main a proc1...\n");
    // Appel à la fonction de commutation de contexte
    ctx_sw(&struct_idle.registre[0], &struct_proc1.registre[1]);
}

void proc1(void) {
    printf("[proc1] idle m’a donne la main\n");
    printf("[proc1] j’arrete le systeme\n");
}

