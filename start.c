#include <inttypes.h>
#include "time.h"
#include "processus.h"
#include <tinyalloc.h>
// External function declaration
extern void traitant_IT_32(void);
processus *TableProcessus[6];
processus* actif;
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
    // sti();

    // On teste les deux processus 
    processus* struct_idle = (processus * )malloc(sizeof(processus));
    struct_idle->pid = 0;
    snprintf(struct_idle->nom_processus, sizeof(struct_idle->nom_processus), "%s", "idle");
    struct_idle->etat = ELU;
    // struct_proc1->pid = 0;
    // snprintf(struct_proc1->nom_processus, sizeof(struct_proc1->nom_processus), "%s", "proc1");
    // struct_proc1->etat = ACTIVABLE;
    // struct_proc1->registre[1] = (int)&struct_proc1->pile[511];
    // struct_proc1->pile[511] = (int)proc1 ;
    // struct_idle->pile[511] = (int)idle;
    // struct_idle->registre[1] = (int)&struct_idle->pile[511];
    TableProcessus[0] = struct_idle;

    int nouveau = creer_processus(proc1,"proc1");
    TableProcessus[nouveau]->pid = nouveau;  

    nouveau = creer_processus(proc2,"proc2");
    TableProcessus[nouveau]->pid = nouveau; 
    
    nouveau = creer_processus(proc3,"proc3");
    TableProcessus[nouveau]->pid = nouveau; 
    
    nouveau = creer_processus(proc4,"proc4");
    TableProcessus[nouveau]->pid = nouveau; 

    nouveau = creer_processus(proc5,"proc5");
    TableProcessus[nouveau]->pid = nouveau; 

    actif = TableProcessus[0];
    idle();

    while (1) {
        hlt();
    }

}
