#include <ecran.h>

uint8_t lig = 0;
uint8_t col = 0;
const uint32_t NB_COLS = 80;
const uint32_t NB_LIGS = 25;
uint16_t* mem_cga = (uint16_t*) VIDEO_MEMORY;


uint16_t *ptr_mem(uint32_t lig, uint32_t col) {
  uint16_t *case_memoire = (uint16_t *)(0xB8000 + 2 * (lig * 80 + col));
  return case_memoire;
}

void ecrit_car(uint32_t lig, uint32_t col, char c, uint8_t couleur_de_fond,
               uint8_t couleur_texte, uint8_t clignotement) {
  uint16_t *adresse_memoire = ptr_mem(lig, col);
  uint8_t base = couleur_texte | couleur_de_fond << 4 | clignotement << 7;
  *(adresse_memoire++) = (uint16_t)c | (uint16_t)base << 8;
}

void efface_ecran(void) {
  for (int i = 0; i < 80; i++) {
    for (int j = 0; j < 25; j++) {
      ecrit_car(j, i,' ', 0, 0, 0);
    }
  }
}

void place_curseur(uint32_t lig, uint32_t col) {
  uint16_t pos = col + lig * 80;
  outb(0x0F, 0x3D4);
  uint8_t premier_bits = pos & 0xFF;
  outb(premier_bits, 0x3D5);
  outb(0x0E, 0x3D4);
  uint8_t dernier_bits = (pos >> 8) & 0xFF;
  outb(dernier_bits, 0x3D5);
}

void traite_car(char c) {
  if (c >= 32 && c <= 126) {
    ecrit_car(lig, col, c, 0, 15,
              0); // Affiche le caractère en blanc sur fond noi
    col++;
    if (col >= NB_COLS) {
      col = 0;
      lig++;
    }
    if (lig >= NB_LIGS) {
      lig = 0; // Pour simplifier, on revient en haut de l'écra
    }
    place_curseur(lig, col);
  } else {
    switch (c) {
    case '\b':
      if (col != 0) {
        col--;
      }
      place_curseur(lig, col);
      break;
    case '\t':
      lig = (col + 8) & ~7;
      if (col >= NB_COLS) {
        col = 0;
        lig++;
      }
      place_curseur(lig, col);
      break;
    case '\n':
      if (lig < NB_LIGS - 1) {
        lig++;
      } else {
        lig = 0;
      }
      col = 0;
      place_curseur(lig, col);
      break;
    case '\f':
      efface_ecran();
      place_curseur(0, 0);
      break;
    case '\r':
      col = 0;
      place_curseur(lig, col);
      break;
    default:
      break;
    }
  }
}

void defilement(void) {
  memmove((void *)mem_cga, (void *)(mem_cga + NB_COLS),
          (NB_LIGS - 1) * NB_COLS * 2);

  for (int i = 0; i < NB_COLS; i++) {
    mem_cga[(NB_LIGS - 1) * NB_COLS + i] = (uint16_t)' ' | (uint16_t)0x07
                                                                    << 8;
  }
}

void console_putbytes(const char *s, int len) {
  for (int i = 0; i < len; i++) {
    char c = s[i];
    traite_car(c); 
  }
}
