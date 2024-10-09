#include <cpu.h>
#include <inttypes.h>
#include "time.h"
// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display

extern void traitant_IT_32(void) ;
// une fonction bien connue
uint32_t fact(uint32_t n)
{
    uint32_t res;
    if (n <= 1) {
        res = 1;
    } else {
        res = fact(n - 1) * n;
    }
    return res;
}

void kernel_start(void)
{
    uint32_t x = fact(5);
    // quand on saura gerer l'ecran, on pourra afficher x
    (void)x;
    efface_ecran();

    
    init_traitant_IT(32,traitant_IT_32);
    configure_clock();
  
    masque_IRQ(0, false); 
    sti();
    // on ne doit jamais sortir de kernel_start
 
    while (1) {
        // cette fonction arrete le processeur
       
        hlt();
    }
}

