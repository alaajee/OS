#include "processus.h"
#include <tinyalloc.h>

// void idle(void)
// {
//   printf("[idle] je tente de passer la main a proc1...\n");
//   ctx_sw(TableProcessus[0].registre, TableProcessus[1].registre);
// }

// void proc1(void)
// {
//   printf("[proc1] idle m’a donne la main\n");
//   printf("[proc1] j’arrete le systeme\n");
//   hlt();
// }
 
// void idle(void)
// {
//   for (int i = 0; i < 3; i++) {
//     printf("[%s] pid = %i\n", mon_nom(), mon_pid());
//     printf("[idle] je tente de passer la main a proc1...\n");
//     ctx_sw(TableProcessus[0].registre,TableProcessus[1].registre);
//     printf("[idle] proc1 m’a redonne la main\n");}
//     printf("[idle] je bloque le systeme\n");
//     hlt();
// }

// void proc1(void)
// {
//   for (;;) {
//     printf("[proc1] idle m’a donne la main\n");
//     printf("[proc1] je tente de lui la redonner...\n");
//     ctx_sw(TableProcessus[1].registre, TableProcessus[0].registre);
//   }
// }

int32_t pid = 0;
int32_t mon_pid(){
  return actif->pid;
}

char *mon_nom(){
  return actif->nom_processus;
}

int trouver_indice(char *nom) {
    for (int i = 0; i < 7; i++) { // La table contient seulement 2 processus
        if (strcmp(TableProcessus[i]->nom_processus, nom) == 0) {
            return i; // Retourne l'indice si les chaînes sont identiques
        }
    }
    return -1; // Retourne -1 si aucun processus ne correspond
} // Pourquoi la derniere ne

void ordonnance(void){
    actif->etat = ACTIVABLE;
    int indice = trouver_indice(actif->nom_processus);
    int nouveau_indice = (1 + indice) % 4 ;
    actif = TableProcessus[nouveau_indice];
    actif->etat = ELU;
    ctx_sw(TableProcessus[indice]->registre, actif->registre);
}

void idle(void)
{
  for (int i = 0; i < 3; i++) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    printf("[idle] je tente de passer la main a proc1...\n");
    ordonnance();
    printf("[idle] proc1 m’a redonne la main\n");
    }
    printf("[idle] je bloque le systeme\n");
    hlt();
}

void proc1(void)
{
  for (;;) {
    printf("[%s] pid = %i\n", mon_nom(), mon_pid());
    printf("[proc1] idle m’a donne la main\n");
    printf("[proc1] je tente de lui la redonner...\n");
    ordonnance();
  }
}


void proc2(void)
{
  for (;;) {
    printf("[proc2] je donne la main à proc3\n");
    ordonnance();
  }
}

void proc3(void)
{
  for (;;) {
    printf("[proc3] je donne la main à proc4 \n");
    ordonnance();
  }
}

void proc4(void)
{
  for (;;) {
    printf("[proc4] je donne la main à idle\n");
    ordonnance();
  }
}

void proc5(void)
{
  for (;;) {
    printf("[proc5] je donne la main à idle\n");
    ordonnance();
  }
}

int32_t creer_processus(void (*code)(void), char *nom){
      processus* nomPro = (processus*) malloc(sizeof(processus));
      nomPro->etat = ACTIVABLE;
      snprintf(nomPro->nom_processus,sizeof(nomPro->nom_processus),"%s",nom);
      nomPro->registre[1] = (int)&nomPro->pile[511];
      nomPro->pile[511] = (int)code;
      pid += 1;
      TableProcessus[pid] = nomPro;
      return pid;
} 