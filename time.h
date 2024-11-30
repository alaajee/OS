#ifndef HORLOGE_H
#define HORLOGE_H

#include <ecran.h>
#include <stdbool.h>
#include "segment.h"

#define IDT_len 256
#define IDT_ADDRESS 0x1000
#define QUARTZ 0x1234DD
#define CLOCKFREQ 50
#define KERNEL_CS 0x10
#define IMPULSION QUARTZ/CLOCKFREQ
#define NB_ITER CLOCKFREQ
// Prototypes des fonctions
extern void ecrit_temps(char* c);
extern void tic_PIT(void);
extern void configure_clock(void);
extern void init_traitant_IT(int32_t num_IT, void (*traitant)(void));
extern void masque_IRQ(uint32_t num_IRQ, bool masque);
extern unsigned int total_seconds;
#endif // HORLOGE_H
