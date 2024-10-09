#ifndef ECRAN_H
#define ECRAN_H

#include <cpu.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
// Variables globales déclarées avec 'extern'
extern uint8_t lig;
extern uint8_t col;
extern const uint32_t NB_COLS;
extern const uint32_t NB_LIGS;
extern uint16_t* mem_cga;
#define VIDEO_MEMORY 0xB8000

// Fonctions
void efface_ecran(void);
uint16_t *ptr_mem(uint32_t lig, uint32_t col);
void ecrit_car(uint32_t lig, uint32_t col, char c, uint8_t couleur_de_fond, uint8_t couleur_texte, uint8_t clignotement);
void console_putbytes(const char *s, int len);
void place_curseur(uint32_t lig, uint32_t col);
void traite_car(char c);
void defilement(void);

#endif // ECRAN_H
