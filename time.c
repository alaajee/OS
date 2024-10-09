#include "time.h"
int tick_count = 0;

void ecrit_temps(char *c) {
  lig = 0;
  col = 70;
  int longueur = strlen(c);
  console_putbytes(c, longueur);
}
void tic_PIT(void) {
    outb(0x20, 0x20);
    tick_count += 1;
    unsigned int total_seconds = tick_count / CLOCKFREQ;
    unsigned int hours = total_seconds / 3600;
    unsigned int minutes = (total_seconds % 3600) / 60;
    unsigned int seconds = total_seconds % 60;
    char buffer[20];
    sprintf(buffer, "%02u:%02u:%02u", hours, minutes, seconds);
    ecrit_temps(buffer);
}

void configure_clock(void) {  
  outb(0x34, 0x43); 
  outb((QUARTZ / CLOCKFREQ) & 0xFF, 0x40); 
  outb((QUARTZ / CLOCKFREQ) >> 8 & 0xFF, 0x40); 
}


void init_traitant_IT(int32_t num_IT, void (*traitant)(void)) {
    uint32_t *Table_32 = (uint32_t *)(IDT_ADDRESS + num_IT * 8);
    uint32_t low = (uint32_t)KERNEL_CS | (uint32_t)traitant;
    uint32_t high = (uint32_t)(((uint32_t)traitant) & 0xFFFF0000) | 0x8E00;
    *Table_32  = low;
    *(Table_32 + 1) = high;
}

void masque_IRQ(uint32_t num_IRQ, bool masque) {
  uint8_t oct_lu = inb(0x21);
  if (masque) {
    oct_lu |= (1 << num_IRQ);
  } else {
    oct_lu &= ~(1 << num_IRQ);
  }
  outb(oct_lu, 0x21);
}
